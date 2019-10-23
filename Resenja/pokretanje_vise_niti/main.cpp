/*
Napraviti konkurentni program koji pokreće više niti u petlji. Svakoj niti proslediti njen redni broj prilikom kreiranja i svaka nit treba da ispiše sopstveni redni broj u okviru tela niti.
*/


#include <thread>
#include <iostream>

using namespace std;

void f(int rbr) {    //funkcija niti
   cout << rbr;
}

const int BROJ_NITI = 10;

int main() {
   thread t[BROJ_NITI];             //Deklaracija niza niti.
   for(int i=0; i<BROJ_NITI; ++i)	//Kreiranje niza niti u petlji pozivom konstruktora klase thread.
      t[i] = thread(f, i);
   for(int i=0; i<BROJ_NITI; ++i)	//Main ceka sve niti da se zavrse. Mora u odvojenoj petlji jer bi u suprotnom bio sekvencijalni program.
      t[i].join();
   cout << endl;
}
