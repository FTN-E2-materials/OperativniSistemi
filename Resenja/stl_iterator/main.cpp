/*
Napraviti program koji prihvata proizvoljan broj linija sa standardnog ulaza i zatim ih ispisuje u redosledu kojim su uneti.
*/

#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<string> v; // STL vektor koji sadrži linije unesene sa tastature
    string s; // Privremena promenljiva za prihvat vrednosti unesene sa tastature
    cout << "Unesite stringove (ctrl-d za kraj)" << endl;

    // Korisnik unosi red sa konzole. Unos se vrši dok se ne pritisne ENTER. Tada se unesena slova ubacuju u promenljivu s:
    while(getline(cin,s)) { // Petlja će se izvršavati dok se ne pritisne ctrl+d (tada funkcija getline vraća false)
        v.push_back(s); // Vrednost unesena sa tastature se ubacuje na kraj STL kontejnera 
    }

    cout << "Uneti elementi su: " << endl;
    
    // Deklaracija iteratora koji pokazuje na element kontejnera.
    // const_iterator ne može da vrši izmenu elementa na koji pokazuje.
    // Koristimo const_iterator jer je potrebno elemente samo ispisivati, a ne i menjati ih.      
    vector<string>::const_iterator it;

    // Iterator inicijalno pokazuje na prvi element u kontejneru i dalje u svakoj iteraciji, iterator se pomera da pokazuje na naredni element. 
    // U petlji se ostaje dok se ne prođu svi elementi.
    for (it = v.begin(); it != v.end(); ++it) {
        cout << *it << endl; // Ispis sadržaja elementa na koji iterator pokazuje u ovoj iteraciji
    }
}
