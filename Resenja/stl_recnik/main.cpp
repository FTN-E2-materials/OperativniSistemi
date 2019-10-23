/*
Napraviti jednostavan program za prevođenje reči sa engleskog na srpski jezik.
Nakon unosa reči sa standardnog ulaza, ispisuje se prevod (ukoliko unesena reč postoji u rečniku). 

Uneti 3 engleske reči i njihove prevode na srpski.
*/

#include <iostream>
#include <string>
#include <map>

using namespace std;

int main()
{
	// STL mapa za čuvanje parova {engleska reč, srpska reč} - ključ u mapi su engleske reči, vrednosti su srpske reči:
	map<string, string> recnik;

    recnik["black"] = "crna"; // Ubacivanje u mapu para {black, crna}
    recnik["white"] = "bela";
    recnik["red"] = "crvena";

    string engleska_rec; // Promenljiva u koju će se upisati unesena reč sa konzole
    // Korisnik unosi red sa konzole. Unos se vrši dok se ne pritisne ENTER. Tada se unesena slova ubacuju u promenljivu engleska_rec.
    while (getline(cin, engleska_rec))  // Petlja će se izvršavati dok se ne pritisne ctrl+d (tada funkcija getline vraća false)
        cout << "Prevod: " << recnik[engleska_rec] << endl; // U mapi se za uneseni ključ (engleska_rec) pronalazi vrednost (odgovarajuća srpska reč)
}
