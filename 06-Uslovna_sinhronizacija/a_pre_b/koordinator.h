#ifndef KOORDINATOR_H_INCLUDED
#define KOORDINATOR_H_INCLUDED

using namespace std;

class Koordinator {
public:
    // Funkcija se poziva nakon što je prva nit završila svoj posao, kako bi signalizirala pokretanje druge niti.
    void prvi_zavrsio() {
        // Implementirati ...
    }

    // Blokirajuća funkcija koja se poziva na početku druge niti kako bi nit čekala dok se prva nit ne završi.
    void drugi_ceka() {
        // Implementirati ...
    }
};

#endif // KOORDINATOR_H_INCLUDED
