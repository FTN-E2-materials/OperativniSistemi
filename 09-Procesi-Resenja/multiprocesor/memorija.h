#ifndef MEMORIJA_H_INCLUDED
#define MEMORIJA_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "dijagnostika.h"

using namespace std;
using namespace chrono;

enum Stanje {SLOBODAN, CITANJE, UPIS };

class Memorija {
private:
    const int VELICINA_RECI = 4;            // Veličina reči je 4 bajta
    Dijagnostika& dijagnostika;

    struct Pristup {                        // Struktura koja predstavlja opis trenutnih pristupa jednoj memorijskoj reči
        Stanje stanje;                      // Memorijska reč moze biti u stanjima: slobodna, čita se ili piše se
        int aktivnih_citaca;                // Broj aktivnih čitača na memorijskoj reči
        int citaca_ceka;                    // Broj čitača koji čekaju na čitanje date reči
        condition_variable citanje;         // CV u koju se uvezuju oni koji hoće da čitaju reč
        condition_variable upis;            // CV u koju se uvezuju oni koji hoće da pišu u datu reč
        Pristup() : stanje(SLOBODAN), aktivnih_citaca(0), citaca_ceka(0) {} // Na početku stanje slobodno i nema čitača i pisača
    };

    mutex m;
    vector<char> mem;           // Memorija je predstavljena vektorom karaktera. Tj. jedna mem. reč = jedan karakter
    vector<Pristup*> pristupi;  // Drugi vektor je vektor pokazivača na strukture koje beleže pristupe datoj reči

public:
    Memorija(Dijagnostika& d, int bajtova) : dijagnostika(d), mem(bajtova) {
        int reci = (bajtova + (VELICINA_RECI - 1)) / VELICINA_RECI;     // Broj reči se zaokružuje na viši celi broj
        for (int i = 0; i < reci; i++) {                                // Primer - 10 bajta = 3 reči. Generiše se 3 instance strukture Pristup.
            pristupi.push_back(new Pristup);
        }
    }

    ~Memorija() {               // Pošto su svi pristupi dinamički generisani moraju se u destruktoru i izbrisati
        for (auto it = pristupi.begin(); it != pristupi.end(); it++)
            delete *it;
    }

    // Metoda koju poziva nit koja simulira proces koji pristupa memoriji kako bi obavila čitanje iz nje
    //
    // rbp    - Redni broj procesa
    // adresa - Lokacija sa koje se čita vrednost prethodno upisana u memoriju
    //
    // Potrebno je pozvati dijagnostika.proces_ceka_citanje kada je memorijska reč zaključana i proces mora da čeka.
    // Potrebno je pozvati dijagnostika.proces_procitao onda kada je ostvaren pristup memoriji i kada je vrednost učitana iz nje.
    char citaj(int rbp, int adresa) {
        int rec = adresa / VELICINA_RECI;          // Izračunavanje pozicije reči koja se čita. Adresa je memorijska adresa u bajtima

        unique_lock<mutex> l(m);
        while (pristupi[rec]->stanje == UPIS) {     // Dok god je stanje date reči takvo da neko trenutno piše u nju:
            (pristupi[rec]->citaca_ceka)++;         // povećaj broj onih koji žele da čitaju iz reči
            dijagnostika.proces_ceka_citanje(rbp, adresa);
            pristupi[rec]->citanje.wait(l);         // čekaj na signal za dozvolu čitanja iz reči
            (pristupi[rec]->citaca_ceka)--;         // smanji broj onih koji žele da čitaju iz reči
        }

        (pristupi[rec]->aktivnih_citaca)++;         // Povecaj broj aktivnih čitaca
        pristupi[rec]->stanje = CITANJE;            // Prebaci stanje reči u to da se ona čita

        l.unlock();
        this_thread::sleep_for(seconds(1));         // Čitanje traje 1 sekundu
        l.lock();

        if (--(pristupi[rec]->aktivnih_citaca) == 0) {  // Ako nema više aktivnih čitača, TEK TADA može obavestiti pisače
            pristupi[rec]->upis.notify_one();           // Efektivno ovo daje veći prioritet čitačima
            pristupi[rec]->stanje = SLOBODAN;           // Oslobodi datu reč
        }

        dijagnostika.proces_procitao(rbp, adresa, mem[adresa]);

        return mem[adresa];                         // Vrati sadržaj sa pročitane memorijske adrese
    }

    // Metoda koju poziva nit koja simulira proces koji pristupa memoriji kako bi obavila upis u nju
    //
    // rbp      - Redni broj procesa
    // adresa   - Lokacija u koju se upisuje nova vrednost
    // vrednost - Vrednost koja se upisuje u memoriju
    //
    // Potrebno je pozvati dijagnostika.proces_ceka_upis kada je memorijska reč zaključana i proces mora da čeka.
    // Potrebno je pozvati dijagnostika.proces_upisao onda kada je ostvaren pristup memoriji i kada je vrednost upisana u nju.
    void pisi(int rbp, int adresa, char vrednost) {
        int rec = adresa / VELICINA_RECI;   // Izračunavanje pozicije reči koja se čita. Adresa je memorijska adresa u bajtima

        unique_lock<mutex> l(m);
        while (pristupi[rec]->stanje != SLOBODAN) { // Dok god stanje memorijske reči nije slobodno, čekaj da se oslobodi
            dijagnostika.proces_ceka_upis(rbp, adresa);
            pristupi[rec]->upis.wait(l);            // Ako ima čitača, svi čitači prvo čitaju pa tek onda pisač piše
        }

        pristupi[rec]->stanje = UPIS;               // Stanje se menja u upis

        l.unlock();
        this_thread::sleep_for(seconds(1));
        l.lock();

        pristupi[rec]->stanje = SLOBODAN;           // Stanje nakon upisa je slobodno
        mem[adresa] = vrednost;                     // Nova vrednost se upisuje u adresu. Obratiti pažnju da se ovo radi tek
                                                    // nakon sleep-a. Konzistentnost se ne narušava jer nit koja upisuje drži propusnicu

        dijagnostika.proces_upisao(rbp, adresa, vrednost);

        if (pristupi[rec]->citaca_ceka != 0)        // Dok god ima čitača koji čekaju, obavesti ih SVE da mogu da čitaju. Ovo je
            pristupi[rec]->citanje.notify_all();    // zato što više čitača smeju da čitaju istovremeno
        else
            pristupi[rec]->upis.notify_one();       // Ako nema čitača obavesti pisače da mogu vršiti upis (ako ih ima naravno)
    }
};

#endif // MEMORIJA_H_INCLUDED
