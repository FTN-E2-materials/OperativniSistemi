#ifndef POSTA_H_INCLUDED
#define POSTA_H_INCLUDED

#include "povratna.h"
#include "klijent.h"

using namespace std;
using namespace chrono;

class Posta {
private:
    Klijent& klijent;

public:
    Posta(Klijent& kl) : klijent(kl) {
        // Prosiriti po potrebi ...
    }

    // Metoda koju poziva nit koja simulira klijenta kada on uplacuje neki iznos.
    // Metoda je blokirajuca - ako je salter zauzet, metoda ce stajati blokirana dok se on ne oslobodi.
    //
    // rbr   - Redni broj klijenta
    // svota - Svota koju klijent zeli da uplati
    //
    // Potrebno je pozvati metodu klijent.ceka kada su salteri zauzeti i klijent mora da ceka.
    // Potrebno je pozvati metodu klijent.uplacuje kada klijent stupi na salter i vrsi svoje placanje.
    // Potrebno je pozvati metodu klijent.napusta kada klijent zavrsi placanje i napusta salter.
    void uplati(int rbr, int svota) {
        // Implementirati ...
    }
};

#endif // POSTA_H_INCLUDED
