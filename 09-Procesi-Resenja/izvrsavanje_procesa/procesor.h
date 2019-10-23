#ifndef PROCESOR_H_INCLUDED
#define PROCESOR_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "dijagnostika.h"

using namespace std;
using namespace chrono;

class Procesor {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    bool prekid, procesor_zauzet;
    condition_variable cv_obradjivac_prekida;   // Red čekanja obrađivača prekida na procesor
    condition_variable cv_procesi;              // Red čekanja procesa na procesor

public:
    Procesor(Dijagnostika& d) : dijagnostika(d) {
        prekid = false;
        procesor_zauzet = false;
    }

    // Pomoćna metoda koja vraća referencu na objekat Dijagnostika asociran sa ovim objektom
    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira proces, kako bi se izvršile sve naredbe procesa.
    //
    // id           - id procesa
    // broj_naredbi - ukupan broj naredbi koje ovaj proces izvršava
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Kada proces uspe da zauzme procesor, pre izvršenja naredbe treba da se pozove dijagnostika.proces_izvrsava.
    // Nakon što je proces izvršio naredbu, potrebno je pozvati dijagnostika.proces_zavrsio.
    void izvrsi_proces(int id, int broj_naredbi) {
        for (int i = 0; i < broj_naredbi; ++i) {
            unique_lock<mutex> l(m);
            while (procesor_zauzet) {                   // Čekanje na procesor u redu spremnih dok god neki drugi proces radi
                dijagnostika.proces_ceka(id);
                cv_procesi.wait(l);
            }

            dijagnostika.proces_izvrsava(id, i);
            procesor_zauzet = true;                      // Zauzimanje procesora

            l.unlock();
            this_thread::sleep_for(seconds(1));
            l.lock();

            dijagnostika.proces_zavrsio(id, i);

            // Ako se desio prekid, notificira obrađivača prekida. U suprotnom, notificira naredni proces, jer obrada prekida ima prioritet:
            procesor_zauzet = false;
            if (prekid)
                cv_obradjivac_prekida.notify_one();
            else
                cv_procesi.notify_one();

            // Odlazi u cooldown, mora se osloboditi propusnica kako bi druge niti mogle pokušati zauzeti procesor:
            l.unlock();
            this_thread::sleep_for(milliseconds(100));
        }
    }

    // Metoda koju poziva nit koja simulira obrađivač U/I prekida kako bi se inicirao i obradio prekid.
    //
    // Ukoliko je procesor već zauzet i ne može se obaviti obrađivanje prekida, potrebno je pozvati metodu dijagnostika.obradjivac_ceka a nakon toga obrađivač treba da pređe u stanje čekanja, dok se procesor ne oslobodi.
    // Kada obrađivač uspe da zauzme procesor, pre obrade prekida treba da se pozove dijagnostika.obradjivac_izvrsava.
    // Nakon što je obrađivač završio obradu, potrebno je pozvati dijagnostika.obradjivac_zavrsio.
    void prekini() {
        unique_lock<mutex> l(m);
        prekid = true;                                  // Postavljanje flag-a prekida kako bi procesor mogao da se preključi na obradu
        while (procesor_zauzet) {
            dijagnostika.obradjivac_ceka();
            cv_obradjivac_prekida.wait(l);              // Dok god je procesor zauzet, čeka se da ga prethodni proces oslobodi
        }

        dijagnostika.obradjivac_izvrsava();
        procesor_zauzet = true;                         // Kreće obrada prekida
        l.unlock();
        this_thread::sleep_for(milliseconds(300));      // Simulacija obrade prekida
        l.lock();
        procesor_zauzet = false;                        // Kraj obrade prekida, oslobađanje procesora, spuštanje flag-a prekida i
        dijagnostika.obradjivac_zavrsio();

        prekid = false;                                 // obaveštavanje sledećeg procesa u redu spremnih da preuzme procesor
        cv_procesi.notify_one();
    }
};

#endif // PROCESOR_H_INCLUDED
