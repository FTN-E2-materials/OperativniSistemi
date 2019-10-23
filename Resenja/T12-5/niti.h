#ifndef NITI_H
#define NITI_H

#include <thread>
#include <map>

using namespace std;

// Implementacija pokretanja dodatne niti koja treba da napravi novi rečnik (koristeći funkciju "napraviRecnik")
//
// brojNiti - Broj niti koje je potrebno startovati
map<string, string> izracunaj(map<string, string> engSrp)
{
    map<string,string> srpEng;

    thread t(napraviRecnik, ref(engSrp), ref(srpEng));  // Kreiranje niti koja vrši stvaranje srpsko-engleskog rečnika preko funkcije "napraviRecnik"
	t.join();

	return srpEng;
}

#endif // NITI_H
