/* 
Napraviti funkciju:
	
vector<int> min(const vector<int>& a, const vector<int>& b);

Ova funkcija vraća vektor koji sadrži minimume korespodentnih elemenata vektora a i b.
Npr: r[0]=min(a[0],b[0]), r[1]=...
Podrazumeva se: a.size()==b.size()
*/

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// Parametri su vektori a i b (po referenci) a rezultat je novi vektor čiji elementi predstavljaju manji od korespodentnih elemenata a i b vektora:
vector<int> min(const vector<int>& a, const vector<int>& b) {
    vector<int> ret_val; // Povratna vrednost funkcije. Inicijalno je ovaj vektor prazan, tj. dužina mu je 0.

    vector<int>::const_iterator ita;
    vector<int>::const_iterator itb;
    
    // Prolazimo redom kroz sve elemente vektora a i b (u svakoj iteraciji "ita" pokazuje na trenutni element u vektoru "a", a "itb" na njegov korespodentni element u vektoru "b"):
    for (ita = a.begin(), itb = b.begin(); ita != a.end(); ita++, itb++)
        ret_val.push_back(min(*ita,*itb)); // U rezultat se ubacuje manji od dva korespodentna elementa (funkcija min iz zaglavlja algorithm vraća manji od 2 prosleđena elementa)
    
	return ret_val;
}

int main() {
	vector<int> a = {5, 9, 12, 7, 3, 1, 28, 16}; // Konstruisanje vektora i istovremeno ubacivanje elemenata
	vector<int> b = {7, 3, 6, 12, 9, 5, 42, 8};
	vector<int> c = min(a, b); // Vektor c je rezultat funkcije min, kojoj se prosleđuju vektori a i b:
	vector<int>::iterator it;
	for (it = c.begin(); it != c.end(); it++) // Iteriranje kroz ceo vektor c
		cout << *it << " "; // Ispis elementa na kojeg iterator pokazuje u ovoj iteraciji
}
