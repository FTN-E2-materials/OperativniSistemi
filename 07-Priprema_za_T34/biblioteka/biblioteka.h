#ifndef BIBLIOTEKA_H_INCLUDED
#define BIBLIOTEKA_H_INCLUDED

#include "primerak.h"

using namespace std;

class Biblioteka {
private:
    Primerak& primerak;
public:
    Biblioteka(Primerak& pr, int br) : primerak(pr) {
        // Prosiriti po potrebi ...
    }

    // Metoda koju poziva nit koja simulira korisnika biblioteke kada on zeli da iznajmi knjigu.
    // Metoda je blokirajuca - ukoliko nema slobodnih primeraka knjige, stajace u stanju cekanja dok se neki ne oslobodi.
    //
    // rbr - Redni broj clana
    //
    // Potrebno je pozvati metodu primerak.ceka kada nema slobodnih primeraka knjige.
    // Potrebno je pozvati metodu primerak.iznajmljen kada nema slobodnih primeraka knjige.
    void iznajmi(int rbr) {
        // Implementirati ...
    }

    // Metoda koju poziva nit koja simulira korisnika biblioteke kada on zeli da vrati knjigu koju je prethodno iznajmio.
    //
    // rbr - Redni broj clana
    //
    // Potrebno je pozvati metodu primerak.vracen kada je primerak vracen u biblioteku.
    void vrati(int rbr) {
        // Implementirati ...
    }
};

#endif // BIBLIOTEKA_H_INCLUDED
