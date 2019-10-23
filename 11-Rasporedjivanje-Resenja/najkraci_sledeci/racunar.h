#ifndef RACUNAR_H_INCLUDED
#define RACUNAR_H_INCLUDED

#include <vector>
#include <mutex>
#include <condition_variable>

#include "dijagnostika.h"

using namespace std;
using namespace chrono;

//Struktura vezana za jedan proces. Sadrži običnu CV, trajanje procesa i id procesa:
struct Proces {
    condition_variable cv;
    int trajanje;
    int id;
    Proces(int t, int i) : trajanje(t), id(i) {}
};

class Racunar {
private:
    Dijagnostika& dijagnostika;
    vector<Proces*> procesi;        // Vektor pokazivača na strukture Proces. Mora dinamički jer CV ne može da se kopira.
    int aktivan_proces;				// Broj aktivnog procesa
    mutex m;
    bool rasporedjivac_aktivan;		// Flag za aktiviranje raspoređivača
    condition_variable r_cv;		// CV na kojoj raspoređivač čeka


public:
    Racunar(Dijagnostika& d) : dijagnostika(d) {
        rasporedjivac_aktivan = false;
        aktivan_proces = -1;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se proces izvršio
    //
    // id_procesa   - ID procesa
    // trajanje     - trajanje procesa (izraženo u stotinama milisekundi)
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Nakon što se proces izvršio, potrebno je pozvati dijagnostika.proces_zavrsio.
    void izvrsi(int id_procesa, int trajanje) {
        Proces * p = new Proces(trajanje, id_procesa);  // Čim krene da se izvršava, proces stvara strukturu Proces.
        unique_lock<mutex> l(m);
        procesi.push_back(p);						    // Data struktura se pod lock-om dodaje u vektor ovih struktura.

        // Ako je procesor slobodan, zauzima se bezuslovno:
        if (aktivan_proces == -1)
            aktivan_proces = id_procesa;

        // Ako neki proces drži procesor, čeka se na CV u okviru strukture na koju pokazuje p.
        while (aktivan_proces != id_procesa) {
            dijagnostika.proces_ceka(id_procesa);
            p->cv.wait(l);  // Treba imati u vidu da se pokazivač p nalazi takođe u vektoru procesi.
        }

        l.unlock();
        this_thread::sleep_for(milliseconds(100 * trajanje));   // Simulacija izvršenja procesa
        l.lock();

        dijagnostika.proces_zavrsio(id_procesa);

        for (auto it = procesi.begin(); it != procesi.end(); it++) {    // Nakon izvršavanja proces se briše iz vektora jer je završen
            if (*it == p) {
                procesi.erase(it);  // Prvo erase, pa delete (it pokazuje na kopiju p koja doduše pokazuje na istu adresu kao i p)
                delete p;			// tek nakon toga se briše memorija na koju pokazuje p
                break;
            }
        }
        rasporedjivac_aktivan = true;       // Diže se flag za aktivaciju raspoređivača
        r_cv.notify_one();					// Signal raspoređivaču
    }

    // Metoda koju poziva nit koja simulira raspoređivač operativnog sistema, kako bi se izabrao sledeći proces za izvršenje.
    //
    // Ukoliko još uvek ne postoji potreba da se raspoređivač aktivira, potrebno je pozvati metodu dijagnostika.rasporedjivac_ceka a nakon toga nit treba da pređe u stanje čekanja.
    // Nakon što se raspoređivač aktivirao i izabrao sledeći proces, potrebno je pozvati dijagnostika.rasporedjivac_izvrsio.
    void rasporedjuj() {
        unique_lock<mutex> l(m);
        while (true) {
            while (!rasporedjivac_aktivan) { // Raspoređivač čeka dok mu se ne postavi flag
                dijagnostika.rasporedjivac_ceka();
                r_cv.wait(l);
            }

            if (procesi.size() > 0) {			// Prolazi kroz vektor procesa u čekanju
                auto najkraci = procesi.begin();
                for (auto it = procesi.begin(); it != procesi.end(); it++) {    // Traži se proces sa najkraćim trajanjem
                    if ((*it)->trajanje < (*najkraci)->trajanje)
                        najkraci = it;
                }
                aktivan_proces = (*najkraci)->id;   // Sledeći proces je onaj sa najkraćim trajanjem
                (*najkraci)->cv.notify_one();
            } else {
                aktivan_proces = -1;
            }

            rasporedjivac_aktivan = false;  // Spuštanje flag-a raspoređivača za novo raspoređivanje kasnije
            dijagnostika.rasporedjivac_izvrsio(aktivan_proces);
        }
    }
};

#endif // RACUNAR_H_INCLUDED
