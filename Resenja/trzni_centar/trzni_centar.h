#ifndef TRZNI_CENTAR_H_INCLUDED
#define TRZNI_CENTAR_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "kupac.h"

using namespace std;
using namespace chrono;

class Trzni_centar {
private:
    Kupac& kupac;
    mutex m; //propusnica za sprecavanje stetnog preplitanja pri pristupu atributima klase
    bool kasa_slobodna[2]; //da li su kase slobodne - prvi element niza se odnosi na prvu kasu, a drugi na drugu
    condition_variable redovi[2]; //uslovne promenljive koje omogucuju zaustavljanje niti u cekanju na kasi - 2 promenljive za 2 kase
    int cekaju[2]; //broj ljudi koji cekaju u redovima na kasama - prvi element niza se odnosi na ljude na prvoj kasi, a drugi na drugoj
public:
    Trzni_centar(Kupac& k) : kupac(k) {
        kasa_slobodna[0] = true; //inicijalno su obe kase slobodne
        kasa_slobodna[1] = true;
        cekaju[0]  = 0; //inicijalno na obe kase niko ne ceka
        cekaju[1]  = 0;
    }

    // Metoda koju poziva nit koja simulira kupca kako bi se obavila kupovina.
    // Metoda je blokirajuca - ako su sve kase zauzete, onda se izvrsenje blokira dok se neka ne oslobodi.
    //
    // rbr           - Redni broj kupca
    // broj_artikala - Broj artikala koje kupac zeli da kupi
    //
    // Potrebno je pozvati metodu kupac.ceka kada je izabrana kasa zauzeta i kupac mora da ceka.
    // Potrebno je pozvati metodu kupac.kupuje kada se kasa oslobodi i kupac pocinje kupovinu.
    // Potrebno je pozvati metodu kupac.zavrsio kada je kupac zavrsio kupovinu.
    int kupi(int rbr, int broj_artikala) {
        unique_lock<mutex> l(m);
        int moja_kasa; //kasa na kojoj kupac kupuje ili staje u red, ako ima ispred  njega drugih kupaca
        if (cekaju[0] <= cekaju[1]) //kupac bira da stane na kasu na kojoj manje ljudi ceka
            moja_kasa = 0;
        else
            moja_kasa = 1;

        while (!kasa_slobodna[moja_kasa]) //dok god kasa u kojoj je stao u red nije slobodna, mora da saceka
        {
            cekaju[moja_kasa]++; //staje u red, sada je jedan vise koji ceka na toj kasi
            kupac.ceka(rbr, moja_kasa);
            redovi[moja_kasa].wait(l);
            cekaju[moja_kasa]--; //ne ceka vise u redu
        }

        kasa_slobodna[moja_kasa] = false; //kasa se oslobodila, ne mora vise da ceka, staje na kasu i zauzima je
        kupac.kupuje(rbr, moja_kasa, broj_artikala);
        l.unlock(); //dok nit odlazi u spavanje, ne treba drugima onemogucavati da rade sa atributima ove klase, zato otkljucamo propusnicu
        this_thread::sleep_for(seconds(broj_artikala)); //simuliranje trajanje kupovine - traje onoliko sekundi koliko ima proizvoda
        l.lock(); //ponovo je potreban pristup deljenim promenljivima, pa se mora zatraziti propusnica pre nego se pristupi ovim promenljivim
        kasa_slobodna[moja_kasa] = true; //oslobadja kasu
        kupac.zavrsio(rbr);
        redovi[moja_kasa].notify_one(); //javlja narednom kupcu koji ceka u redu
        return moja_kasa+1; //rezultat metode je broj kase na kojoj je kupovina izvrsena (posto indeksi u nizu idu od 0, dodaje se 1 da bi korisnik dobio rezultat 1 ili 2)
    }
};


#endif // TRZNI_CENTAR_H_INCLUDED
