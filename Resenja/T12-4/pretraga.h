#ifndef PRETRAGA_H
#define PRETRAGA_H

#include <cmath>
#include <vector>

using namespace std;

// Funkcija koja će biti iskorišćena kao funkcija niti - pretražuje ulaznu kolekciju (zadati su iteratori koji definišu granice pretrage) tražeći broj najbliži nuli.
//
// pocetak  - Iterator koji pokazuje na prvi element kolekcije koji učestvuje u pretrazi
// kraj     - Iterator iza poslednjeg elementa koji učestvuje u pretrazi
// najblizi - Nađeni element, najbliži nuli od svih elemenata koji su obuhvaćeni pretragom (izvršenjem funkcije ovaj broj se popuni)
//
void pretrazi(vector<double>::const_iterator pocetak, vector<double>::const_iterator kraj, double& najblizi)
{
    najblizi = *pocetak;    // Na početku se podrazumeva da je prvi element obuhvaćen pretragom upravo onaj koji se traži - najbliži nuli (analogno algoritmu za nalaženje najmanjeg člana kolekcije)
    for (vector<double>::const_iterator it = pocetak + 1; it != kraj; it++) {  // Prolazi se kroz ceo opseg zadat parametrima pocetak i kraj (preskače se prvi element, pošto je on već uzet za trenutni najbliži
        if (abs(*it) < abs(najblizi)) {  // Ako se pronađe element koji je po apsolutnoj vrednosti manji od vrednosti "najblizi", onda je taj element bliži nuli, pa se on proglašava tekućim najbližim
            najblizi = *it;
        }
    }
}

#endif // PRETRAGA_H
