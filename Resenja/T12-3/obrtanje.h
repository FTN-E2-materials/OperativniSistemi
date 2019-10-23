#ifndef OBRTANJE_H
#define OBRTANJE_H

#include <list>

using namespace std;

// Implementacija obrtanja redosleda elemenata u listi
// Ova funkcija treba da bude iskorišćena za funkciju niti koje ovaj program pokreće.
//
// ulazna  - Ulazna lista, koja se obrađuje
// izlazna - Rezultujuća lista (biće popunjena elementima u obrnutom redosledu)
//
void obrniRedosled(const list<int>& ulazna, list<int>& izlazna)
{
    // Pošto nije dozvoljeno menjanje liste (referenca je data na const listu), onda se kreira i konstantni iterator:
    for (list<int>::const_iterator it = ulazna.begin(); it != ulazna.end(); it++) {
    	izlazna.push_front(*it);    // Stavljanje elementa iz ulazne liste na početak izlazne liste.
                                    // Ovo efektivno slaže elemente ulazne liste u obrnutom redosledu u izlaznoj listi.
	}
}

#endif // OBRTANJE_H
