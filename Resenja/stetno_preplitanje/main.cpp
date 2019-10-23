/*
Napraviti konkurentni program koji u funkciji visina pita korisnika koliko je visok.
Nakon toga korisnik unosi svoju visinu. Na kraju u funkciji se ispisuje uneta visina.

Kreirati 2 niti od date funkcije. Ispratiti ispis.
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

// Pri pristupu terminalu dešava se štetno preplitanje, obzirom da druga nit može da dobije pristup terminalu pre nego je prva nit završila posao:
void visina() {
   int v; 
   cout << "Koliko ste visoki [cm]?" <<endl;
   cin >> v;
   cout << "Vasa visina je " << v << " cm." << endl;
   cout << endl;
}

int main() {
   thread t1(visina);
   thread t2(visina);
   t1.join();
   t2.join();
}
