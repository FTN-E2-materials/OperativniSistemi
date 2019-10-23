#ifndef NITI_H_INCLUDED
#define NITI_H_INCLUDED

#include <vector>
#include <thread>

#include "suma_vektora.h"

using namespace std;

// v - vektor čije elemente treba sumirati
// povratna vrednost - suma svih elemenata vektora, izračunata pokretanjem 2 niti (svaka treba da obradi jednu polovinu elemenata)
double sumiraj(vector<double> v) {
   double zbir1, zbir2;
   vector<double>::const_iterator polovina = v.begin() + v.size()/2;            // Određivanje iteratora (pokazivača) na sredinu vektora. Sredina se nalazi na v.begin() + polovina dužine vektora.
   thread t1(f,  v.begin(), polovina,  ref(zbir1)); // Stvaranje i pokretanje niti t1. Nit t1 sabira prvu polovinu vektora, tj. od iteratora v.begin() do iteratora polovina.
   thread t2(f,  polovina,  v.end(),   ref(zbir2)); // Stvaranje i pokretanje niti t2. Nit t2 sabira drugu polovinu vektora, tj. od iteratora polovina do iteratora v.end().
   t1.join();
   t2.join();

   cout << "zbir1 = " << zbir1 << endl;               // Ispis zbira može da se izvrši tek nakon završetka obe niti! (nakon oba join-a).
   cout << "zbir2 = " << zbir2 << endl;
   return zbir1+zbir2;
}

#endif // NITI_H_INCLUDED
