#ifndef RACUNAR_H_INCLUDED
#define RACUNAR_H_INCLUDED

#include <vector>

#include "dijagnostika.h"

using namespace std;

class Racunar {
private:
    Dijagnostika& dijagnostika;

public:
    Racunar(Dijagnostika& d) : dijagnostika(d) {
        // Proširiti po potrebi ...
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
        // Implementirati ...
    }

    // Metoda koju poziva nit koja simulira raspoređivač operativnog sistema, kako bi se izabrao sledeći proces za izvršenje.
    //
    // Ukoliko još uvek ne postoji potreba da se raspoređivač aktivira, potrebno je pozvati metodu dijagnostika.rasporedjivac_ceka a nakon toga nit treba da pređe u stanje čekanja.
    // Nakon što se raspoređivač aktivirao i izabrao sledeći proces, potrebno je pozvati dijagnostika.rasporedjivac_izvrsio.
    void rasporedjuj() {
        // Implementirati ...
    }
};

#endif // RACUNAR_H_INCLUDED
