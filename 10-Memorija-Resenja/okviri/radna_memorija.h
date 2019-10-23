#ifndef RADNA_MEMORIJA_H_INCLUDED
#define RADNA_MEMORIJA_H_INCLUDED

#include <vector>
#include <mutex>
#include <condition_variable>

#include "dijagnostika.h"

using namespace std;

class Radna_memorija {
private:
    Dijagnostika& dijagnostika;
    vector<int> okviri; // Evidencija svih okvira u memoriji. Za svaki okvir evidentira se ID procesa koji ga trenutno zauzima ili se postavlja -1 u elementu ako je taj okvir slobodan
    int slobodnih_okvira; // Koliko trenutno ima slobodnih_okvira
    mutex m;
    condition_variable slobodno;

public:
    // dijagnostika  - referenca na instancu klase Dijagnostika
	// ukupno_okvira - ukupan broj okvira u memoriji
    Radna_memorija(Dijagnostika& d, int ukupno_okvira) : dijagnostika(d) {
        okviri.resize(ukupno_okvira, -1);   // Kreiranje vektora dužine ukupno_okvira i inicijalizacija svih elemenata na -1
        slobodnih_okvira = ukupno_okvira;   // Svi okviri su slobodni
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa kako bi pokušala da učita proces u radnu memoriju
    //
    // broj_stranica - ukupan broj stranica koje proces sadrži
    // id_procesa    - identifikator procesa
    //
    // Ukoliko trenutno nema na raspolaganju dovoljno memorije (broj stranica u procesu je veći od broja raspoloživih okvira memorije), potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da čeka dok se memorija ne oslobodi.
    // Kada proces uspe da dođe do potrebne radne memorije, treba da se pozove dijagnostika.proces_izvrsava. Nakon toga,
    // kada proces zauzme okvire radne memorije, potrebno je pozvati dijagnostika.ispisi_okvire kako bi se prikazalo trenutno zauzece svih okvira (podrazumeva se da zelimo da prikazemo sliku svih okvira, tako da ce se videti i okviri koje su zauzeli drugi procesi).
    void ucitaj(int broj_stranica, int id_procesa) {
        unique_lock<mutex> l(m);
        while (slobodnih_okvira < broj_stranica) {
            dijagnostika.proces_ceka(id_procesa);
            slobodno.wait(l);
        }
        // Ima dovoljno slobodnih okvira
        // Tražimo slobodne okvire da ih zauzmemo:
        dijagnostika.proces_se_izvrsava(id_procesa);
        int proces_zauzeo_okvira = 0;

        for (auto it = okviri.begin(); it != okviri.end(); it++) {
            if (*it == -1)  { // Ako ni jedan proces ne koristi ovaj okvir
                *it = id_procesa; // Evidentiramo da se ovaj proces smešta u okvir
                slobodnih_okvira--; // Sada je jedan manje slobodan okvir
                if (++proces_zauzeo_okvira == broj_stranica)  // Ako je zauzeto okvira koliko procesu treba, zauzimanje okvira se prekida
                    break;
            }
        }

        dijagnostika.ispisi_okvire(okviri.begin(), okviri.end());
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa kako bi oslobodila radnu memoriju koju je koristila tokom izvršenja
    //
    // id_procesa - identifikator procesa koji oslobađa memoriju
    void oslobodi(int id_procesa) {
        // Prolazi se kroz sve okvire i oslobađaju oni koje je zauzeo ovaj proces:
        unique_lock<mutex> l(m);
        for (auto it = okviri.begin(); it != okviri.end(); ++it) {
            if (*it == id_procesa) { //Ako je okvir zauzet od strane procesa sa brojem id_procesa
                *it = -1; // proglašava se slobodnim
                slobodnih_okvira++;
            }
        }
        slobodno.notify_all(); // Notifikuju se svi koji čekaju, jer se ne zna koliki broj slobodnih okvira treba procesima.
        // Probudiće se svi iz čekanja ali će nastaviti da rade samo oni za koje ima dovoljno slobodnih okvira. Ostali će se vratiti u čekanje. Na ovom mestu je ovakva notifikacija dobra jer odjednom može biti odblokirano više od jednog procesa.
    }
};

#endif // RADNA_MEMORIJA_H_INCLUDED
