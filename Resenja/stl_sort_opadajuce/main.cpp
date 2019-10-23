/*
Uneti 5 celih brojeva sa standardnog ulaza.
Korišćenjem STL algoritma sortirati brojeve u opadajućem redosledu.
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Funkcija dobija 2 broja i vraća informaciju o tome da li je prvi broj veći od drugog
bool poredi(int a, int b) { return a > b; }

int main() {
    vector<int> brojevi; // STL vektor koji sadrži brojeve koji treba da se sortiraju
    int broj; // Privremena promenljiva u koju se ubacuje broj unesen sa tastature
    cout << "Unesite 5 celih brojeva:" << endl;
    for (int i = 0; i < 5; i++) {
        cin >> broj; // Zahtev za unos sa tastature - unesena vrednost se upisuje u promenljivu broj
        brojevi.push_back(broj); // Uneseni broj se ubacuje na kraj STL kontejnera
    }

    // Nakon unosa elemenata, vrši se sortiranje:
    
    // Funkcija sort je definisana u zaglavlju algorithm.
    // Prvi parametar je iterator na element od kojeg se počinje sortiranje.
    // Drugi parametar je iterator iza elementa kojim se završava sortiranje.
    // Treći parametar je funkcija koja ce međusobno porediti elemente i odlučiti da li se elementima zamenjuju mesta u kontejneru.
    // Funkcija poredi će se automatski pozvati potreban broj puta (kada god algoritam sortiranja treba da uporedi elemente kontejnera).		
    // Rezultat funkcije "poredi" (true ili false) odrediće da li algoritam treba da zameni mesta elementima koje je funkcija poredila		
    sort(brojevi.begin(), brojevi.end(), poredi);

    // Ispis elemenata kontejnera nakon sortiranja:
    cout << "Sortirani brojevi: " << endl;
    vector<int>::const_iterator it;
    // Prolazak kroz vektor od početka do kraja:
    for (it = brojevi.begin(); it != brojevi.end(); ++it) {
        cout << *it << endl; // Ispis elementa na kojeg iterator pokazuje u trenutnoj iteraciji
    }
}
