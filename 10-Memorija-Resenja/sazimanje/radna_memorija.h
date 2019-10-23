#ifndef RADNA_MEMORIJA_H_INCLUDED
#define RADNA_MEMORIJA_H_INCLUDED

#include <mutex>
#include <condition_variable>
#include <vector>

#include "dijagnostika.h"

using namespace std;
using namespace chrono;

class Radna_memorija {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    bool aktiviraj_sazimanje;
    vector<int> lokacije; // Indeksi su indeksi lokacija a elementi su identifikatori procesa koji ih koriste
    condition_variable cv_sazimanje, cv_slobodno;

    // br_lokacija_procesa - koliko lokacija je procesu potrebno da bi se izvršio
    // Metoda vraća indeks od kojeg počinju lokacije koje su dodeljene procesu ili -1 ako nema takvog bloka
    int pocetak_bloka(int br_lokacija_procesa) {
        int broj_uzastopnih = 0;
        int i = 0;
        // Tražimo dovoljan broj uzastopnih slobodnih okvira:
        for (auto it_lok = lokacije.begin(); it_lok != lokacije.end() ; it_lok++, i++) {
            if (*it_lok == -1) {    // Ako je okvir slobodan
                broj_uzastopnih++;  // broji se koliko ih ima uzastopnih slobodnih lokacija.
            } else { // Kad god se naiđe na zauzet okvir, resetuje se broj uzastopnih slobodnih okvira.
                broj_uzastopnih = 0;
            }
            // Ako je pronađen dovoljno veliki blok:
            if (broj_uzastopnih == br_lokacija_procesa)
                return i - br_lokacija_procesa + 1; // Ovo je početna lokacija bloka
        }
        // Ako nema dovoljno uzastopnih slobodnih okvira:
        return -1;
    }

public:
    Radna_memorija(Dijagnostika& d, int ukupno_lokacija) : dijagnostika(d) {
        aktiviraj_sazimanje = false;
        lokacije.resize(ukupno_lokacija, -1); // Inicijalno je svaka lokacija slobodna. Za slobodne okvire je oznaka procesa -1.
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa kako bi proces zauzeo potrebnu memoriju i krenuo sa radom
    //
    // id_procesa          - identifikator procesa
    // br_lokacija_procesa - ukupan broj lokacija memorije koje su neophodne za izvršenje procesa
    // trajanje            - koliko dugo se proces izvršava (u sekundama)
    //
    // Ukoliko trenutno nema na raspolaganju dovoljno slobodne memorije (moraju se zauzeti uzastopni okviri u memoriji!), potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da čeka dok se memorija ne oslobodi.
    // Kada proces uspe da dođe do potrebne radne memorije, treba da se pozove dijagnostika.proces_zauzeo_okvire.
    // Kada se proces završi, potrebno je pozvati metodu dijagnostika.proces_zavrsio.
    // Metodu dijagnostika.ispisi_memoriju potrebno je pozvati u sledećim momentima: nakon što proces zauzme memoriju i nakon što proces oslobodi memoriju.
    void koristi(int id_procesa, int br_lokacija_procesa, int trajanje) {
        unique_lock<mutex> l(m);
        int pocetak;

        while ((pocetak = pocetak_bloka(br_lokacija_procesa)) == -1) { // Ako nema dovoljno slobodnih uzastopnih okvira
            dijagnostika.proces_ceka(id_procesa);
            cv_slobodno.wait(l);
        }

        // Obeležavanje okvira - okviri koji počinju od indeksa "pocetak" će biti označeni ID-jem ovog procesa:
        for (int i = 0; i < br_lokacija_procesa; i++)
            lokacije[pocetak+i] = id_procesa;

        dijagnostika.proces_zauzeo_okvire(id_procesa, pocetak, pocetak + br_lokacija_procesa);
        dijagnostika.ispisi_memoriju(lokacije.begin(), lokacije.end());

        l.unlock();
        this_thread::sleep_for(seconds(trajanje));
        l.lock();

        // Ponovo se pronalazi gde je početak bloka ovog procesa jer je u međuvremenu možda došlo do sažimanja:
        for (pocetak = 0; pocetak < lokacije.size(); pocetak++) {
            if (lokacije[pocetak] == id_procesa)
                break;
        }
        // Okviri procesa se proglašavaju slobodnim:
        for (int i = 0; i < br_lokacija_procesa; i++)
            lokacije[pocetak+i] = -1;

        dijagnostika.proces_zavrsio(id_procesa);
        dijagnostika.ispisi_memoriju(lokacije.begin(), lokacije.end());

        aktiviraj_sazimanje = true;
        cv_sazimanje.notify_one(); // Aktivira se sažimanje
    }

    // Metoda koju poziva nit operativnog sistema koji se bavi sažimanjem memorije
    //
    // Nakon što je sažimanje obavljeno potrebno je pozvati metodu dijagnostika.sazimanje_obavljeno a nakon toga i metodu dijagnostika.ispisi_memoriju kako bi se prikazali efekti sažimanja memorije.
    void sazimanje() {
        unique_lock<mutex> l(m);
        while (!aktiviraj_sazimanje)
            cv_sazimanje.wait(l);

        aktiviraj_sazimanje = false;

        vector<int> kopija;    // Stvori novi vektor iste dužine kao stari
        kopija.resize(lokacije.size(), -1);
        int a = 0;
        for (int i = 0; i < lokacije.size(); i++) {  // Kopiraj korišćene lokacije iz starog u novi vektor, na početak
            if (lokacije[i] != -1)
                kopija[a++] = lokacije[i];
        }
        lokacije = kopija;   // Stari vektor dobija vrednost novog

        dijagnostika.sazimanje_obavljeno();
        dijagnostika.ispisi_memoriju(lokacije.begin(), lokacije.end());

        cv_slobodno.notify_all();  //javi svima jer je moguce da ima dovoljno memorije za vise procesa sada
    }
};

#endif // RADNA_MEMORIJA_H_INCLUDED
