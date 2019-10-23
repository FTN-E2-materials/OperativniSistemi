#ifndef PARKING_H_INCLUDED
#define PARKING_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "automobil.h"

using namespace std;

class Parking {
private:
    Automobil& automobil;
    int slobodnih_mesta;                        // Broj slobodnih mesta. Slično kao kod zadatka a_i_b_pre_c, ovde je upotrebljen brojač.
    mutex m;                                    // Mutex i uslovna promenljiva neophodni za uslovnu sinhronizaciju.
    condition_variable slobodan;

public:
    Parking(Automobil& a, int kapacitet) : automobil(a), slobodnih_mesta(kapacitet) {}

    // Metoda koju poziva nit koja simulira kretanje automobila kako bi automobil pokušao da se parkira.
    // Ako je parking zauzet, izvršenje ove metode će trajati dok se parking ne oslobodi i auto se parkira.
    //
    // rbr - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.ceka kada je parking zauzet i auto ne može da se parkira.
    // Potrebno je pozvati metodu automobil.parkira kada auto uspe da se parkira.
    void udji(int rbr) {
        unique_lock<mutex> l(m);
        while (slobodnih_mesta == 0) {              // Dok god NEMA slobodnih mesta na parkingu svaki novi automobil mora da čeka
            automobil.ceka(rbr);                    // da se oslobodi. NEMA busy waitinga tj. konstantne provere ispunjenosti uslova.
            slobodan.wait(l);
        }
        --slobodnih_mesta;                          // Ukoliko je uspeo da uđe na Parking, automobil smanjuje broj slobodnih mesta.
        automobil.parkira(rbr);
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kada auto izlazi sa parkinga (nakon što je bio parkiran).
    //
    // rbr - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.napusta kada auto napušta parking mesto.
    void izadji(int rbr) {
        unique_lock<mutex> l(m);
        slobodan.notify_one();                      // Kada izlazi, automobil povećava broj slobodnih mesta i obaveštava jednog od
        ++slobodnih_mesta;                          // automobila koji čekaju u redu.
        automobil.napusta(rbr);
    }
};

#endif // PARKING_H_INCLUDED
