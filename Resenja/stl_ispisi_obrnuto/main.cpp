/*
Napraviti program koji prihvata proizvoljan broj linija sa standardnog ulaza i zatim ih ispisuje u obrnutom redosledu 
(poslednja prihvaćena linija se ispisuje prva).
*/

#include <iostream>
#include <vector>

using namespace std;

// Sledi tip iteratora koji pokazuje na element kontejnera.
// const_iterator služi za read-only iteraciju kroz elemente (ne može da vrši izmenu elementa na koji pokazuje). Pošto se u zadatku samo prikazuju vrednosti elemenata, const_iterator je adekvatan.
// const_reverse_iterator je const_iterator koji se pomera unazad kroz elemente kontejnera (inkrementiranjem iteratora prelazi se na prethodni element, a ne sledeći).
typedef vector<string>::const_reverse_iterator Vcri;  // Ovo je praktično alijas, na dalje se može koristiti Vcri umesto dužeg zapisa ovog tipa podataka.

int main() {
   vector<string> v; // STL vektor koja sadrži linije unesene sa tastature
   string s; // Privremena promenljiva za prihvat vrednosti unešene sa tastature
   cout << "Unesite stringove (ctrl-d za kraj)" << endl;
   
   // Korisnik unosi red sa konzole. Unos se vrši dok se ne pritisne taster ENTER. Tada se unesena slova ubacuju u promenljivu s:
   while(getline(cin,s)) { // Petlja će se izvršavati dok se ne pritisne ctrl+d (tada funkcija getline vraća false)
	v.push_back(s); // Vrednost unesena sa tastature se ubacuje na kraj STL kontejnera
   }   
    
   cout << "Uneti stringovi u obrnutom redosledu:" << endl;

   // Iterator inicijalno pokazuje na poslednji element u kontejneru (metoda rbegin() vraća iterator na poslednji element).
   // U svakoj iteraciji iterator se pomera da pokazuje na prethodni element. U petlji se ostaje dok se ne prođu svi elementi 
   // tj. dok se ne prođe prvi element (metoda rend() vraća iterator na lokaciju pre prvog elementa).
   for (Vcri it = v.rbegin(); it != v.rend(); ++it) 
     cout << *it << endl; // Ispis sadržaja elementa na koji iterator pokazuje u ovoj iteraciji
}
