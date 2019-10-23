#ifndef BIBLIOTEKA_H_INCLUDED
#define BIBLIOTEKA_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "primerak.h"

using namespace std;

class Biblioteka {
private:
    Primerak& primerak;
    mutex m; //propusnica za sprecavanje stetnog preplitanja
    condition_variable c; //red cekanja na knjigu
    int slobodnih_primeraka; //trenutni broj slobodnih primeraka knjige
public:
    Biblioteka(Primerak& pr, int br) : primerak(pr), slobodnih_primeraka(br) {} //inicijalno su svi primerci slobodni

    // Metoda koju poziva nit koja simulira korisnika biblioteke kada on zeli da iznajmi knjigu.
    // Metoda je blokirajuca - ukoliko nema slobodnih primeraka knjige, stajace u stanju cekanja dok se neki ne oslobodi.
    //
    // rbr - Redni broj clana
    //
    // Potrebno je pozvati metodu primerak.ceka kada nema slobodnih primeraka knjige.
    // Potrebno je pozvati metodu primerak.iznajmljen kada nema slobodnih primeraka knjige.
    void iznajmi(int rbr) {
        unique_lock<mutex> l(m); //zakljucavanje propusnice pre pristupa deljenim resursima
        while (slobodnih_primeraka == 0) { //dok god nijedan primerak nije slobodan, treba da saceka
            primerak.ceka(rbr);
            c.wait(l);
        }
        primerak.iznajmljen(rbr);
        slobodnih_primeraka--; //uzima knjigu, jedan primerak manje
    }

    // Metoda koju poziva nit koja simulira korisnika biblioteke kada on zeli da vrati knjigu koju je prethodno iznajmio.
    //
    // rbr - Redni broj clana
    //
    // Potrebno je pozvati metodu primerak.vracen kada je primerak vracen u biblioteku.
    void vrati(int rbr) {
        unique_lock<mutex> l(m);
        primerak.vracen(rbr);
        slobodnih_primeraka++; //vraca knjigu, jedan primerak vise u biblioteci
        c.notify_one(); //javlja nekom od onih koji cekaju na knjigu da se pojavio u biblioteci slobodan primerak
    }
};

#endif // BIBLIOTEKA_H_INCLUDED
