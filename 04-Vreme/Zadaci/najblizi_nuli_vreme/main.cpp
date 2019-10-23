/*
Napraviti konkurentni program koji pronalazi element najbliži broju 0 u zadatom vektoru brojeva.

Pretragu podeliti u 3 programske niti. Ulazni vektor brojeva treba da sadrži 900.000 slučajno generisanih brojeva.

Za svaku nit, evidentirati trenutke kada je izvršavanje počelo i kada se završilo. Na kraju programa, ispisati koliko je trajalo izvršavanje svake niti u milisekundama.
*/

#include <iostream>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono;

typedef vector<double>::const_iterator cit;

struct vreme {
    system_clock::time_point pocetak;
    system_clock::time_point kraj;
};

void f(cit pocetak, cit kraj, double & min, struct vreme * v) {
	// Implementirati...
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

	// Implementirati...
}

