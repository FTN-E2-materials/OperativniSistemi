#ifndef RASPOREDJIVAC_H_INCLUDED
#define RASPOREDJIVAC_H_INCLUDED

#include "dijagnostika.h"

using namespace std;

class Rasporedjivac {
private:
    Dijagnostika& dijagnostika;

public:
	Rasporedjivac(Dijagnostika& d, int broj_nivoa_prioriteta) : dijagnostika(d) {
        // Proširiti po potrebi ...
	}

	Dijagnostika& getDijagnostika() {
        return dijagnostika;
	}

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se proces izvršio
    //
    // id_procesa   - ID procesa
    // broj_naredbi - ukupan broj naredbi koje proces treba da izvrši
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Nakon što je proces izvršio naredbu, potrebno je pozvati dijagnostika.izvrsio_naredbu.
	void izvrsi(int id_procesa, int broj_naredbi) {
        // Implementirati ...
	}
};

#endif // RASPOREDJIVAC_H_INCLUDED
