/*
Napraviti konkurentni program koji stvara nit iz koje pita korisnika za visinu u centimetrima i potom ispisuje: “Vasa visina je <uneta_visina> cm.”

Testirati program tako što će main nit, koristeći objekat koji predstavlja novu nit, da pozove metodu join. Potom promeniti taj poziv u poziv metode detach.
*/

#include <iostream>
#include <thread>

using namespace std;

void kod_niti() {               // Funkcija niti.
  int visina;
  cout << "Unesite vasu visinu u cm:" << endl;
  cin >> visina;
  cout << "Vasa visina je: " << visina << " cm" << endl;
}
int main() {
   thread nit(kod_niti);       // Stvaranje niti i pokretanje niti.
   nit.join();                 // Cekanje niti main da se zavrsi nit t.
   //nit.detach();             // Nit main NE ceka da se zavrsi nit t (detach se koristi kod niti koje sadrze beskonacne petlje - deamon niti).
}




