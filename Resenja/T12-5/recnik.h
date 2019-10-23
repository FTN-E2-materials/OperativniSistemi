#ifndef RECNIK_H
#define RECNIK_H

#include <map>
#include <string>

using namespace std;

// Funkcija koja će biti iskorišćena kao funkcija niti - od zadatog englesko-srpskog rečnika ona pravi obrnuti rečnik - srpsko-engleski
//
// engSrp - Englesko-srpski rečnik (ključ je reč na engleskom, podatak je njen prevod na srpski)
// srpEng - Srpsko-engleski rečnik (ključ je reč na srpskom, podatak je njen prevod na engleski) - ovaj parametar treba da se popuni izvršenjem ove funkcije
//
void napraviRecnik(map<string, string> engSrp, map<string, string>& srpEng)
{
	for (map<string, string>::iterator it = engSrp.begin(); it != engSrp.end(); it++) { // Prolazak iteratorom kroz čitav englesko-srpski rečnik
     	srpEng[it->second] = it->first;  // Zamena ključeva i vrednosti jednog i drugog rečnika (swap), što efektivno dovodi do stvaranja srpsko-engelskog rečnika
	}
}


#endif // RECNIK_H
