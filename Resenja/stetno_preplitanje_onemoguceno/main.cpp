/*
Napraviti konkurentni program koji u funkciji visina pita korisnika koliko je visok.
Nakon toga korisnik unosi svoju visinu. Na kraju u funkciji se ispisuje uneta visina.

Sprečiti štetno preplitanje na terminalu korišćenjem klase mutex.

Kreirati 2 niti od date funkcije. Ispratiti ispis.
*/


#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex m; // Propusnica za pristup deljenom resursu (u ovom slučaju to je terminal). Pošto je globalna promenljiva, obe niti koriste istu propusnicu

void visina() {
   int v; 
   // Tražimo propusnicu za pristup terminalu
   // u praksi se izbegava ovaj način zaključavanja, jer propusnica može ostati zaključana. Programer mora da vodi računa da otključa propusnicu u svakom scenariju izvršavanja
   m.lock();	
   cout << "Koliko ste visoki [cm]?" <<endl;
   cin >> v;
   cout << "Vasa visina je " << v << " cm." << endl;
   cout << endl;
   m.unlock(); // Otključavanje propusnice da bi naredna nit mogla da pristupi terminalu
}

int main() {
   thread t1(visina);
   thread t2(visina);
   t1.join();
   t2.join();
}
