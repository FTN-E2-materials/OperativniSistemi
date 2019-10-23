/*
Data je klasa Brojac.
U main funkciji napraviti objekat klase Brojac. Ispis vrednosti Brojaca radi posebna nit.
Napraviti nit koja vrši ispis i proslediti joj Brojac.
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

class Brojac {
   mutex m; // Propusnica pripada klasi i obezbeđuje sprečavanje štetnog preplitanja pri vršenju operacija nad objektima klase
   int broj; // Vrednost Brojaca
 public:
   Brojac() : broj(0) {} // Inicijalna vrednost je nula
   void inc() { unique_lock<mutex> l(m); ++broj; } // Operacija povećava Brojac, ali pre toga zaključava propusnicu i na taj način sprečava štetno preplitanje pri izmeni
   void dec() { unique_lock<mutex> l(m); --broj; }// Operacija smanjuje Brojac, ali pre toga zaključava propusnicu i na taj način sprečava štetno preplitanje pri izmeni
   friend ostream& operator<<(ostream& os, Brojac& b) { // Preklopljen operator za ispis objekta klase Brojac. Ispisuje se atribut "broj" iz objekta
      unique_lock<mutex> l(b.m); // Pre pristupa broju, zaključava se propusnica da bi se sprečilo štetno preplitanje
      os << b.broj <<endl;
      return os;
   }
};

/*void ispis(Brojac b) {  // Ne može se po vrednosti kopirati objekat koji sadrži mutex, jer mutex klasa nema copy konstruktor (obrisan pri implementaciji)
    cout << b << endl;
}*/

void ispis(Brojac& b) {
    cout << b << endl;
}


int main() {
   Brojac b;
   b.inc();
   // thread t(ispis, b); // Greška jer se ne može kopirati mutex ili objekat koji sadrži mutex
   thread t(ispis, ref(b));
   t.join();
}
