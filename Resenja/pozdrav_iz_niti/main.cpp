/* 
Napraviti program koji kreira jednu nit i u okviru niti ispisuje proizvoljnu rečenicu.
*/

#include <iostream>
#include <thread>

using namespace std;

void kod_niti() {               // Funkcija niti.
   cout << "Pozdrav iz niti!" << endl;
}

int main() {
	thread t(kod_niti);         /* Stvaranje objekta niti (t) od funkcije kod_niti(), i pokretanje niti. 
	                              Objekat niti (t) efektivno predstavlja samu nit, pa ćemo ubuduće 
	                              objekat niti zvati samo nit radi lakšeg praćenja komentara.*/
	t.join();                   // Poziv operacije join() na niti t. Nit main čeka da se nit t završi.
}
