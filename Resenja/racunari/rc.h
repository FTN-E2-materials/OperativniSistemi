#ifndef RC_H_INCLUDED
#define RC_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "student.h"

using namespace std;

#define MAX 50 //maksimalni broj racunara u ucionici

class RC {
private:
    Student& student;
	bool slobodni[MAX]; //za svaki racunar evidentira se podatak da li se trenutno koristi
	condition_variable c; //red cekanja na bilo koji slobodan racunar (svi korisnici cekaju u jednom zajednickom redu)
	int broj_slobodnih; //broj trenutno slobodnih racunara
    int broj_racunara; //ukupan broj racunara u ucionici
	mutex m; //propusnica za sprecavanje stetnog preplitanja prilikom pristupa atributima klase
public:
	RC(Student& st, int br) : student(st) {
        broj_racunara = br; //ukupan broj racunara je prosledjen spolja pri instanciranju
        broj_slobodnih = broj_racunara; //inicijalno su svi racunari slobodni
        for (int i = 0; i < broj_slobodnih; ++i) //za svaki racunar se evidentira da je slobodan
            slobodni[i] = true;
    }

    // Metoda koju poziva nit koja simulira studenta kako bi student zauzeo mesto za racunarom.
    // Ova metoda je blokirajuca - ako nema slobodnih mesta, ona ce cekati dok se neko ne oslobodi.
    //
    // rbr - Redni broj studenta
    //
    // Potrebno je pozvati metodu student.ceka kada su racunari zauzeti i student mora da ceka.
    // Potrebno je pozvati metodu student.zauzeo kada student zauzme racunar.
    int zauzmi(int rbr) {
        unique_lock<mutex> l(m); //zauzimanje propusnice pre pristupa deljenim promenljivim
        while (broj_slobodnih == 0) { //dok god nijedan racunar nije slobodan, korisnik mora da saceka
            student.ceka(rbr);
            c.wait(l);
        }

        int retVal; //povratna vrednost metode - broj racunara koji je zauzet
        for (int i = 0; i < broj_racunara; ++i) { //trazimo racunar koji se oslobodio
            if (slobodni[i]) {
                slobodni[i] = false; //zauzimamo racunar koji smo pronasli kao slobodan
                retVal = i + 1; //upamtimo indeks racunara (dodamo 1 da bi brojevi isli od 1, a ne od 0 kao indeksi)
                break; //nema potrebe da se dalje pretrazuje kada je racunar pronadjen
            }
        }
        student.zauzeo(rbr, retVal);
        --broj_slobodnih; //nakon zauzimanja racunara, ima jedan slobodan racunar manje
        return retVal;
    }

    // Metoda koju poziva nit koja simulira studenta kako bi oslobodio racunar koji je prethodno zauzeo.
    //
    // rbr         - Redni broj studenta
    // id_racunara - Redni broj racunara kojeg student oslobadja (prethodno je zauzeo taj racunar)
    //
    // Potrebno je pozvati metodu student.oslobodio kada student oslobodi racunar.
    void oslobodi(int rbr, int id_racunara) {
        unique_lock<mutex> l(m);
        ++broj_slobodnih; //oslobadjanjem racunara, ima jedan slobodan racunar vise
        student.oslobodio(rbr, id_racunara);
        slobodni[id_racunara-1] = true; //oznacimo da je racunar sa prosledjenim brojem slobodan (umanjujemo prosledjenu vrednost za 1, jer indeksi u nizu idu od 0, a brojevi racunara od 1)
        c.notify_one(); //javimo nekom od korisnika koji cekaju da se pojavio slobodan racunar
    }
};

#endif // RC_H_INCLUDED
