/*
Definisati funkciju: void increment(int& a);
koja povećava (za jedan) vrednost argumenta.

Napraviti program koji:
a) poziva funkciju increment()
b) stvara nit od funkcije increment()

Napomena: dodati ref(b) kao drugi parametar niti da bi program ispravno radio
*/

#include <thread>
#include <iostream>

using namespace std;

void increment(int& a) {        // Funkcija niti (prima parametar po referenci)
   ++a;
}

int main() {
   int a=0;
   int b=0;
   increment(a);                // Poziv funkcije increment (standardno prosleđivanje parametra po referenci).
   thread t(increment, b);      // Stvaranje niti i pokretanje niti. Prosleđivanje parametra niti po referenci kao običnoj funkciji (ne radi). Pogledati napomenu.
   t.join();                    
   cout << "a=" << a << endl;   // Ispis vrednosti a (uspešno izmenjena).
   cout << "b=" << b << endl;   // Ispis prednosti b (ostala ista -> greška). Pogledati napomenu.
}


