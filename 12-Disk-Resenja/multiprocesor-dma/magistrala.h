#ifndef MAGISTRALA_H_INCLUDED
#define MAGISTRALA_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "dijagnostika.h"

using namespace std;
using namespace chrono;

// Klasa magistrale (sabirnice). Glavna deljena promenljiva za sinhronizaciju pristupa procesora memoriji i dma kontroleru.
class Magistrala {
public:
    // Stanja sabirnice - slobodna, čitanje iz memorije, pisanje u memoriju, dma transfer. Primetiti sličnost sa stanjima iz zadatka "multiprocesor":
    enum Stanje { SLOBODNA, MEM_CITAJ, MEM_PISI, DMA};

    // Struktura u koju se beleže parametri DMA transfera. Odakle, kome i koliko bajtova se čita ili piše.
    struct DMA_transfer {
        int odakle;
        int koliko;
        int kome;
    };

private:
    Dijagnostika& dijagnostika;
    Memorija& memorija;    // Referenca (ne kopija) na memorijski blok
    mutex m;
    Stanje stanje;         // Stanje magistrale
    condition_variable magistrala_slobodna;     // CV na kojoj procesori čekaju magistralu
    condition_variable DMA_kontroler;           // CV na kojoj DMA kontroler čeka signal (okidac_dma_kontrolera signal)
    DMA_transfer transfer;                      // Polje za komunikaciju izmedju dma() i okidac_dma_kontrolera(), struktura

public:
    // Konstruktor magistrale. Prosleđuje se referenca na memoriju:
    Magistrala(Dijagnostika& d, Memorija& mem) : dijagnostika(d), memorija(mem) {
        stanje = SLOBODNA;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Funkcija čitanja memorije. Poziva funkciju citaj klase Memorija implicitno:
    char citaj_memoriju(int adresa) {
        unique_lock<mutex> l(m);
        while(stanje != SLOBODNA)           // Čeka se dok god stanje magistrale nije slobodno. Drugim rečima, nema mogućnosti za više čitača istovremeno kao što je bilo u zadatku "multiprocesor".
            magistrala_slobodna.wait(l);

        this_thread::sleep_for(milliseconds(700));  // Zaključavanje magistrale traje 700ms.
        stanje = MEM_CITAJ;                         // Stanje magistrale postaje da se čita memorija.
        l.unlock();                                 // Otključava se KS. Magistralu i dalje niko ne može da koristi ali barem
                                                    // može da se uveže u čekanje.
        // Pozivi blokirajućih operacija treba (ako je moguće) da su van kritične sekcije (tako je efikasnije).
        char vrednost = memorija.citaj(adresa);     // Čitanje iz memorije. Obratiti pažnju da memorija ima svoj poseban muteks.

        l.lock();
        stanje = SLOBODNA;                          // Stanje magistrale ukazuje da je slobodna.
        magistrala_slobodna.notify_one();           // Notifikacija niti koje čekaju na čitanje iz memorije, upis ili dma, da mogu
        return vrednost;                            // koristiti magistralu.
    }

    //Funkcija upisa u memoriju. Poziva funkciju pisi klase Memorija implicitno:
    void pisi_u_memoriju(int adresa, char vrednost) {
        unique_lock<mutex> l(m);
        while(stanje != SLOBODNA)                   // Čeka se dok god stanje magistrale nije slobodno.
            magistrala_slobodna.wait(l);

        this_thread::sleep_for(milliseconds(700));  // Zaključavanje magistrale traje 700ms.
        stanje = MEM_PISI;                          // Stanje magistrale postaje da se piše u memoriju.
        l.unlock();                                 // Otključava se KS. Magistralu i dalje niko ne može da koristi ali barem
                                                    // može da se uveže u čekanje.
        memorija.pisi(adresa, vrednost);            // Pisanje u memoriju.

        l.lock();
        stanje = SLOBODNA;                          // Stanje magistrale ukazuje da je slobodna.
        magistrala_slobodna.notify_one();           // Notifikacija niti koje čekaju na čitanje iz memorije, upis ili dma.
    }


    // Treća funkcija koju može zvati procesor (osim citaj_memoriju i pisi_u_memoriju). Ova funkcija označava DMA transfer. Prosleđuje se struktura transfer.
    void dma(DMA_transfer t) {
        unique_lock<mutex> l(m);
        while(stanje != SLOBODNA)                   // Kao i kod prethodnih metoda, potrebno je sačekati da se magistrala oslobodi.
            magistrala_slobodna.wait(l);

        this_thread::sleep_for(milliseconds(700));  // Zaključavanje magistrale traje 700ms.
        stanje = DMA;                               // Stanje se menja u DMA. Magistrala je namenjena samo za DMA.
        transfer = t;                               // Transfer deljeno polje dobija vrednost t. Posmatrati kao razmenu podataka između dve niti (procesor i DMA kontroler).
        DMA_kontroler.notify_one();
    }

    DMA_transfer okidac_dma_kontrolera() {
        unique_lock<mutex> l(m);

        while(stanje != DMA)                        // Kontroler čeka da jedan od procesora zatraži DMA prenos.
            DMA_kontroler.wait(l);

        l.unlock();

        for(int i = 0; i < transfer.koliko; i++) {  // Tokom DMA transfera, direktno se čita i piše u memoriju, bez posredovanja procesora.
            char vrednost = memorija.citaj(transfer.odakle + i);
            memorija.pisi(transfer.kome + i, vrednost);
        }

        l.lock();
        stanje = SLOBODNA;                          // DMA transfer je završen, magistrala se oslobađa.
        magistrala_slobodna.notify_one();

        return transfer;
    }
};

#endif // MAGISTRALA_H_INCLUDED
