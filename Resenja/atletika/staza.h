#ifndef STAZA_H_INCLUDED
#define STAZA_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "povratna.h"
#include "takmicar.h"

using namespace std;
using namespace chrono;

class Atletska_staza {
private:
    Takmicar& takmicar;
    mutex m;
    bool slobodno;                          //Da li je staza zauzeta.
    condition_variable skakaci, bacaci;     //Red cekanja za skakace i bacace.
    int skakaca_ceka, bacaca_ceka;          //Broj skakaca i bacaca koji cekaju.
public:
    Atletska_staza(Takmicar& tak) : takmicar(tak) {
        skakaca_ceka = 0;
        bacaca_ceka = 0;
        slobodno = true;
    }

    // Metoda koju poziva nit koja simulira skakaca kako bi takmicar obavio skok.
    // Ukoliko je zaletiste zauzeto, ova metoda ce stajati blokirana dok se ono ne oslobodi i takmicar obavi nastup.
    //
    // rbr - Redni broj takmicara
    //
    // Potrebno je pozvati metodu takmicar.skakac_ceka kada skakac ne moze da stupi na zaletiste.
    // Potrebno je pozvati metodu takmicar.skakac_skace kada skakac stupi na zaletiste, pre skoka.
    // Potrebno je pozvati metodu takmicar.skakac_zavrsio kada je skakac zavrsio i kada treba prikazati njegov rezultat.
    Povratna_vrednost skaci(int rbr) {             //Funkcija skakanja. Zovu je niti skakaca_ceka.
        system_clock::time_point dosao = system_clock::now(); //Belezi se vreme dolaska.
        unique_lock<mutex> l(m);
        while (!slobodno) {
            skakaca_ceka++;                 //Povecavamo broj skakaca koji cekaju u redu. Neophodno ovde a ne posle waita.
            takmicar.skakac_ceka(rbr);
            skakaci.wait(l);                //Ukoliko je staza slobodna skaci, ako nije cekaj.
            skakaca_ceka--;                 //Posle waita se smanjuje broj skakaca koji cekaju u redu.
        }
        slobodno = false;                   //Zauzimanje staze.

        takmicar.skakac_skace(rbr);
        l.unlock();
        this_thread::sleep_for(seconds(1)); //1 sekunda mu treba da skoci
        system_clock::time_point zavrsio = system_clock::now();   //Belezimo vreme zavrsetka skakanja
        l.lock();

        slobodno = true;                    //Oslobadjanje staze.
        if (bacaca_ceka > 0)                //Provera da li ima bacaca koji cekaju. Ako ima obavesti ih da mogu bacati.
            bacaci.notify_one();
        else
            skakaci.notify_one();           //U suprotnom obavesti sledeceg skakaca koji cekaju da moze skociti.

        Povratna_vrednost pv;
        pv.rezultat = rand() % 10;          //Rezultat skoka je random broj do 10 metara
        pv.trajanje = zavrsio - dosao;

        takmicar.skakac_zavrsio(rbr, pv);
        return pv;
    }

    // Metoda koju poziva nit koja simulira bacaca kako bi takmicar obavio bacanje.
    // Ukoliko je zaletiste zauzeto, ova metoda ce stajati blokirana dok se ono ne oslobodi i takmicar obavi nastup.
    //
    // rbr - Redni broj takmicara
    //
    // Potrebno je pozvati metodu takmicar.bacac_ceka kada bacac ne moze da stupi na zaletiste.
    // Potrebno je pozvati metodu takmicar.bacac_skace kada bacac stupi na zaletiste, pre skoka.
    // Potrebno je pozvati metodu takmicar.bacac_zavrsio kada je bacac zavrsio i kada treba prikazati njegov rezultat.
    Povratna_vrednost baciKoplje(int rbr) {
        system_clock::time_point dosao = system_clock::now();
        unique_lock<mutex> l(m);

        while (!slobodno) {                 //Ukoliko je staza slobodna baci koplje, ako nije cekaj.
            bacaca_ceka++;                  //Povecavamo broj bacaca koji cekaju u redu. Neophodno ovde a ne posle waita.
            takmicar.bacac_ceka(rbr);
            bacaci.wait(l);
            bacaca_ceka--;                  //Posle waita se smanjuje broj bacaca koji cekaju u redu.
        }

        slobodno = false;                   //Ostatak funkcije je istovetan funkciji skaci()...

        takmicar.bacac_baca(rbr);
        l.unlock();
        this_thread::sleep_for(seconds(2)); //2 sekunde mu treba da baci koplje
        system_clock::time_point zavrsio = system_clock::now();
        l.lock();

        slobodno = true;
        if (skakaca_ceka > 0)
            skakaci.notify_one();
        else
            bacaci.notify_one();

        Povratna_vrednost pv;
        pv.rezultat = rand() % 101;           //Koplje leti do 100 metara.
        pv.trajanje = zavrsio - dosao;

        takmicar.bacac_zavrsio(rbr, pv);
        return pv;
    }
};

#endif // STAZA_H_INCLUDED
