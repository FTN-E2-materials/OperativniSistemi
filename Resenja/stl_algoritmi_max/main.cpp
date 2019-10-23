/*
Korisnik treba da unese 5 celih brojeva koristeći tastaturu (standardni ulaz).
Korišćenjem STL algoritma, među tih 5 brojeva potrebno je pronaći najveći.
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    vector<int> brojevi;  //STL vektor koji sadrži brojeve koji treba da se sortiraju
    int broj;  // Privremena promenljiva u koju se ubacuje broj unesen sa tastature
    cout << "Unesite 5 celih brojeva:" << endl;
    for (int i = 0; i < 5; i++) {
        cin >> broj;  // Zahtev za unos sa tastature - unesena vrednost se upisuje u promenljivu broj
        brojevi.push_back(broj); // Uneseni broj se ubacuje na kraj STL kontejnera vektor 
    } 

    // Pronalaženje najvećeg elementa u kontejneru korišćenjem funkcije max_element iz zaglavlja algorithm:
    // Prvi parametar je iterator na element od kojeg se počinje traženje.
    // Drugi parametar je iterator iza poslednjeg elementa (metoda end() vraća iterator iza poslednjeg elementa).
    // Funkcija max_element vraća iterator na najveći element. Dereferenciranjem ovog iteratora pristupa se vrednosti najvećeg elementa.
    
    int najveci = *max_element(brojevi.begin(), brojevi.end());
    cout << "Najveci broj je: " << najveci << endl;
}
