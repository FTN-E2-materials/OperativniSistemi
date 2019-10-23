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

public:
    Rasporedjivac(Dijagnostika& d) : dijagnostika(d) {
        // Proširiti po potrebi ...
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
        // Implementirati ...
	}
};

#endif // RASPOREDJIVAC_H_INCLUDED
