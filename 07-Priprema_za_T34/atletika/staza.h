#ifndef STAZA_H_INCLUDED
#define STAZA_H_INCLUDED

#include "povratna.h"
#include "takmicar.h"

using namespace std;
using namespace chrono;

class Atletska_staza {
private:
    Takmicar& takmicar;
public:
    Atletska_staza(Takmicar& tak) : takmicar(tak) {
        // Prosiriti po potrebi ...
    }

    // Metoda koju poziva nit koja simulira skakaca kako bi takmicar obavio skok.
    // Ukoliko je zaletiste zauzeto, ova metoda ce stajati blokirana dok se ono ne oslobodi i takmicar obavi nastup.
    //
    // rbr - Redni broj takmicara
    //
    // Potrebno je pozvati metodu takmicar.skakac_ceka kada skakac ne moze da stupi na zaletiste.
    // Potrebno je pozvati metodu takmicar.skakac_skace kada skakac stupi na zaletiste, pre skoka.
    // Potrebno je pozvati metodu takmicar.skakac_zavrsio kada je skakac zavrsio i kada treba prikazati njegov rezultat.
    Povratna_vrednost skaci(int rbr) {
        // Implementirati ...
    }

    // Metoda koju poziva nit koja simulira bacaca kako bi takmicar obavio bacanje.
    // Ukoliko je zaletiste zauzeto, ova metoda ce stajati blokirana dok se ono ne oslobodi i takmicar obavi nastup.
    //
    // rbr - Redni broj takmicara
    //
    // Potrebno je pozvati metodu takmicar.bacac_ceka kada bacac ne moze da stupi na zaletiste.
    // Potrebno je pozvati metodu takmicar.bacac_skace kada bacac stupi na zaletiste, pre skoka.
    // Potrebno je pozvati metodu takmicar.bacac_zavrsio kada je bacac zavrsio i kada treba prikazati njegov rezultat.
    Povratna_vrednost baciKoplje(int rbr) {
        // Implementirati ...
    }
};

#endif // STAZA_H_INCLUDED
