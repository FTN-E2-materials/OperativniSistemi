#ifndef RASPOREDJIVAC_H_INCLUDED
#define RASPOREDJIVAC_H_INCLUDED

#include <vector>
#include <map>
#include <mutex>
#include <condition_variable>

#include "dijagnostika.h"
#include "cv_tag.h"

using namespace std;

struct Proces {
	int id;
	int broj_naredbi;
	vector<int> tiketi;
	Proces(int i, int b, vector<int> t) : id(i), broj_naredbi(b), tiketi(t) {}
};

class Rasporedjivac {
private:
    Dijagnostika& dijagnostika;
   	int aktivni_proces;					// ID aktivnog procesa
	cv_tag red;							// CV sa privescima. Omogućava aktivno obaveštavanje po privesku.
	mutex m;
	map<int,int> svi_tiketi;            // Mapa koja sadrži parove (tiket, id_procesa)

public:
    Rasporedjivac(Dijagnostika& d) : dijagnostika(d) {
        aktivni_proces = -1;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se proces izvršio
    //
    // p - struktura koja sadrži ID procesa, broj naredbi koje treba da izvrši kao i sve tikete koji pripadaju tom procesu.
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Nakon što je proces izvršio naredbu, potrebno je pozvati dijagnostika.izvrsio_naredbu.
	void izvrsi(Proces p) {
        // Dodaju se tiketi ovog procesa u vektor svih tiketa (thread-safe operacija):
        {
            unique_lock<mutex> l(m);
            for (auto it = p.tiketi.begin(); it != p.tiketi.end(); it++) {
                svi_tiketi[*it] = p.id;     // Dodavanje tiketa u mapu svih tiketa - ključ je broj tiketa, vrednost je id procesa.
            }
        }								// Tokom generisanja tiketa se vodi računa da se isti tiketi (ključevi) ne ponove.

        for (int i = 0; i < p.broj_naredbi; ++i) {  // Kreće izvršavanje naredbi tek NAKON dodavanja tiketa u mapu svih tiketa.
            unique_lock<mutex> l(m);
            // Ako je procesor slobodan, zauzima se:
            if (aktivni_proces == -1) {
                aktivni_proces = p.id;
            }

            // Ako neko drži procesor, čeka se u cv_tag uvezujući privezak koji ima vrednost id procesa:
            while (aktivni_proces != p.id) {
                dijagnostika.proces_ceka(p.id);
                red.wait(l, p.id);
            }

            l.unlock();
            this_thread::sleep_for(chrono::milliseconds(300));	// Simulacija izvršenja naredbe
            l.lock();

            // Ako je ovo poslednja instrukcija procesa, njegovi tiketi se izbacuju liste svih tiketa:
            if (i == p.broj_naredbi - 1) {
                for (auto it = p.tiketi.begin(); it != p.tiketi.end(); it++) {
                    auto za_brisanje = svi_tiketi.find(*it);
                    svi_tiketi.erase(za_brisanje);
                }
            }

            // Indeks sledećeg tiketa u spisku tiketa se bira na slučajan način (što neko ima više tiketa, veća je šansa da bude izabran):
            if (!svi_tiketi.empty()) {
                int poz_tiketa = rand() % svi_tiketi.size();
                // Naredni proces je onaj koji drži tiket:
                map<int, int>::iterator it = svi_tiketi.begin();
                for (int k = 0; k < poz_tiketa; k++, it++);  // Pomeranje iteratora do pozicije tiketa
                aktivni_proces = it->second;
                // Budi se proces kojem tiket pripada. id procesa je privezak u cv_tag promenljivoj red:
                red.notify(aktivni_proces);
            } else {
                aktivni_proces = -1;
            }

            dijagnostika.izvrsio_naredbu(p.id, i, aktivni_proces);
        }
	}
};

#endif // RASPOREDJIVAC_H_INCLUDED
