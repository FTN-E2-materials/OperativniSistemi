#ifndef POSTA_H_INCLUDED
#define POSTA_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "povratna.h"
#include "klijent.h"

using namespace std;
using namespace chrono;

class Posta {
private:
    Klijent& klijent;
    int uplaceno[2]; //prvi element niza se odnosi na kolicinu novca uplacenu na prvom salteru, a drugi element na drugom
    condition_variable red; //uslovna promenljiva za uvezivanje niti u red cekanja na uslov
    bool salter_zauzet[2]; //prvi element niza se odnosi na to da li je prvi salter zauzet, a drugi element da li je drugi zauzet
    mutex m; //propusnica za iskljucivi region pri pristupu deljenim resursima

public:
    Posta(Klijent& kl) : klijent(kl) { //inicijalno su oba saltera slobodna i nista na njima nije uplaceno
        uplaceno[0] = 0; uplaceno[1]=0;
        salter_zauzet[0] = false; salter_zauzet[1] = false;
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
    void uplati(int rbr, int svota) { //parametar je svota koju klijent uplacuje
        unique_lock<mutex> l(m); //zakljucavanje propusnice zbog pristupa deljenim resursima
        while(salter_zauzet[0] && salter_zauzet[1]) { //dok god su oba saltera zauzeta, klijent mora da saceka
            klijent.ceka(rbr, svota);
            red.wait(l);
        }
        //jedan salter se oslobodio. Naredna linija utvrdjuje koji je to salter i upamti redni broj saltera u promenljivu "salter".
        int salter = salter_zauzet[0] ? 1 : 0;
        salter_zauzet[salter] = true; //klijent zauzima salter koji se oslobodio
        klijent.uplacuje(rbr, salter, svota);
        l.unlock(); //placanje ce da potraje. Za to vreme treba pustiti druge klijente da rade svoj posao, nema razloga da drzimo propusnicu
        this_thread::sleep_for(seconds(svota)); //simuliranje trajanja placanja. Placanje traje broj sekundi proporcionalan svoti koja se uplacuje
        l.lock(); //ponovo trazimo propusnicu jer cemo pristupati deljenim resursima
        uplaceno[salter] += svota; //sada je na ovom salteru ukupna uplacena kolicina novca povecana za sumu koju je klijent upravo uplatio
        klijent.napusta(rbr, salter, uplaceno[salter]);
        salter_zauzet[salter] = false; //klijent oslobadja salter na kojem je izvrsio uplatu
        red.notify_one(); //javlja nekom od onih koji cekaju da se jedan salter oslobodio koliko ukupno do sada
    }
};

#endif // POSTA_H_INCLUDED
