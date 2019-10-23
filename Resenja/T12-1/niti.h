#ifndef NITI_H
#define NITI_H

#include <thread>
#include <vector>

using namespace std;

// Implementacija sabiranja korespondentnih elemenata celih vektora A i B pokretanjem onoliko niti koliko ima elemenata u vektorima
//
// a - Vektor A
// b - Vektor B
//
// Rezultat izvršavanja treba da bude vector<int> koji sadrži rezultat sabiranja korespondentnih elemenata.
//
vector<int> izracunaj(vector<int>& a, vector<int>& b)
{
    int velicina = a.size();
    thread niti[velicina];
    vector<int> rezultat(velicina);

    for (int pozicija = 0; pozicija < a.size(); pozicija++) {
        niti[pozicija] = thread(saberiPar, a.begin() + pozicija, b.begin() + pozicija, rezultat.begin() + pozicija);
    }

    for (int pozicija = 0; pozicija < a.size(); pozicija++) {
        niti[pozicija].join();
    }

    return rezultat;
}

#endif // NITI_H
