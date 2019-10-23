/*
Uneti 5 celih brojeva sa standardnog ulaza.
Korišćenjem STL algoritma sortirati brojeve u rastućem redosledu.
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    vector<int> brojevi; // STL vektor koji sadrži brojeve koji treba da se sortiraju
    int broj; // Privremena promenljiva u koju se ubacuje broj unesen sa tastature
    cout << "Unesite 5 celih brojeva:" << endl;
    for (int i = 0; i < 5; i++) {
        cin >> broj; // Zahtev za unos sa tastature - unesena vrednost se upisuje u promenljivu broj
        brojevi.push_back(broj); // Uneseni broj se ubacuje na kraj STL kontejnera
    }

    // Nakon unosa elemenata, vrši se sortiranje:
    // Prvi parametar je iterator na element od kojeg se počinje sortiranje.
    // Drugi parametar je iterator na element pre kojeg treba zavrsiti sortiranje.	
    // Pošto nije navedena funkcija (kao treći parametar) koja opisuje poređenje elemenata, primenjuje se default poređenje. Zato će se sortiranje izvršiti u rastućem redosledu.
    sort(brojevi.begin(), brojevi.end());

    // Ispis elemenata kontejnera nakon sortiranja:
    cout << "Sortirani brojevi: " << endl;
    vector<int>::const_iterator it;
    // Prolazak kroz vektor od početka do kraja:
    for (it = brojevi.begin(); it != brojevi.end(); ++it) {
        cout << *it << endl; // Ispis elementa na kojeg iterator pokazuje u trenutnoj iteraciji
    }
}
