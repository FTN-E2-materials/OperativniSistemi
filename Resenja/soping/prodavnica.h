#ifndef PRODAVNICA_H_INCLUDED
#define PRODAVNICA_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "povratna.h"
#include "kupac.h"

using namespace std;
using namespace chrono;

class Prodavnica {
private:
    Kupac& kupac;
    int slobodnih_kabina;               //Broj slobodnih kabina
    mutex m;
    condition_variable c;
public:
    Prodavnica(Kupac& k, int n): kupac(k), slobodnih_kabina(n) {}

    // Metoda koju poziva nit koja simulira kupca, kada kupac obavlja soping (procedura je opisana u zaglavlju main.cpp)
    // Metoda je blokirajuca - ako kupac mora da ceka da se oslobodi kabina za probu, izvrsenje ce blokirati dok se za to ne steknu uslovi.
    //
    // rbr - Redni broj kupca
    //
    // Potrebno je pozvati metodu kupac.ceka kada su kabine zauzete i kupac mora da ceka.
    // Potrebno je pozvati metodu kupac.proba_odecu kada kupac udje u kabinu da proba odecu.
    // Potrebno je pozvati metodu kupac.zavrsio kada je kupac zavrsio probu odece.
    povratna_vrednost kupi(int rbr) {
        povratna_vrednost pv;
        unique_lock<mutex> l(m);
        system_clock::time_point dosao = system_clock::now();
        while (slobodnih_kabina == 0) {              //Ako nema slobodnih kabina, cekaj.
            kupac.ceka(rbr);
            c.wait(l);
        }

        pv.cekao_na_kabinu = system_clock::now() - dosao;

        slobodnih_kabina--;
        kupac.proba_odecu(rbr);
        l.unlock();
        this_thread::sleep_for(seconds(1)); //proba odecu
        l.lock();

        pv.kupio = rand() % 2;        //Da li je kupio ili nije. Tj. 0 ili 1.
        slobodnih_kabina++;         //Povecava se broj slobodnih kabina.
        kupac.zavrsio(rbr, pv);
        c.notify_one();             //Obavesti sledeceg kupca da moze uci u kabinu.

        return pv;
    }
};

#endif // PRODAVNICA_H_INCLUDED
