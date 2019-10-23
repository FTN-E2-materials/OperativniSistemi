/*
    Prosiriti zadatak najbilzi_nuli_vreme.cpp ispisom u sekundama i minutima.
*/

#include <iostream>
#include <thread>
#include <cmath>
#include <vector>

using namespace std;
using namespace chrono;

typedef vector<double>::const_iterator cit;

struct vreme {
    system_clock::time_point pocetak;
    system_clock::time_point kraj;
};

void f(cit pocetak, cit kraj, double & min, struct vreme * v) {
    v->pocetak = system_clock::now(); //evidentiramo trenutak kada je nit pocela da se izvrsava i upisemo u prosledjenu promenljivu v
    min = *pocetak; //proglasimo nulti element najblizim nuli
    for (auto it = pocetak + 1; it != kraj; it++) { //prodjemo kroz sve ostale elemente
        if (abs(*it) < abs(min)) //ako je i-ti element blizi nuli od trenutno najblizeg
            min = *it; //tada imamo novi najblizi element
    }

    v->kraj = system_clock::now();//trenutak kada je trazenje minimuma zavrseno upisujemo u prosledjenu promenljivu v
}

const int BROJ_NITI = 3;
const int DUZINA = 900000;

int main() {
    int SEGMENT = DUZINA / BROJ_NITI;

    srand(time(NULL));   // Postavljamo seme random generatora na trenutno vreme, što dovodi do kvalitetnijeg izbora slučajnih brojeva.

    vector<double> v(DUZINA);
    // Puni se vektor pseudo-slučajnim brojevima
    for (int i = 0; i < DUZINA; i++)
	v[i] = rand();

    double minimumi[BROJ_NITI];  // Niz elemenata najbližih nuli - svaka programska nit će dati svoj međurezltat
    struct vreme vremena[BROJ_NITI]; // Niz struktura u koji će biti upisani podaci o trajanju izvršavanja svake niti

    thread niti[BROJ_NITI];

    // Iteratori na pocetak i kraj segmenta koji obrađuje svaka od niti, ovi iteratori se pomeraju kako se prolazi kroz različite niti:
    cit pocetak = v.begin();
    cit kraj = v.begin() + SEGMENT;

    for (int i = 0; i < BROJ_NITI; ++i) {
        niti[i] = thread(f, pocetak, kraj, ref(minimumi[i]), &vremena[i]);
        pocetak += SEGMENT;
        kraj += SEGMENT;
    }

    for (int i = 0; i < BROJ_NITI; ++i) {
        niti[i].join();
    }

    // Kada su sve niti završile, imamo tri najbliža elementa u svakoj trećini niza.
    // Sada medju ta tri elementa, treba naći najbliži element:
    double rez = minimumi[0];
    for (int i = 1; i < BROJ_NITI; i++) {
       if (abs(minimumi[i]) < abs(rez))
            rez = minimumi[i];
    }

    cout << "Najblizi element nuli je: " << rez << endl;

    // Ispis trajanja izvršenja za svaku od niti:
    for (int i = 0; i < BROJ_NITI; i++) {
        duration<double, milli> trajanje_mili = vremena[i].kraj - vremena[i].pocetak;
        duration<double> trajanje_sek = vremena[i].kraj - vremena[i].pocetak;
        duration<double, ratio<60>> trajanje_min = vremena[i].kraj - vremena[i].pocetak;
        cout << "Nit " << i+1 << " se izvrsavala: " << endl
        	  << trajanje_mili.count() << " milisekundi. " << endl
        	  << trajanje_sek.count() << " sekundi. " << endl
        	  << trajanje_min.count() << " minuta. " << endl << endl;
    }
}

