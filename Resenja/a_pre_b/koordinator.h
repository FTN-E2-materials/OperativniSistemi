#ifndef KOORDINATOR_H_INCLUDED
#define KOORDINATOR_H_INCLUDED

#include <mutex>
#include <condition_variable>

using namespace std;

class Koordinator {
    enum redosled {PRVI, DRUGI};         // Enumeracija koja pokazuje koja nit je na redu (tip uslova).
    redosled na_redu_je;                 // Eksplicitni uslov. U zavisnosti od vrednosti ovog uslova nit b će morati da čeka.
    mutex m;
    condition_variable c;                // Uslovna promenljiva. Koristi se za sinhronizaciju pristupa kritičnoj sekciji između niti a i b.
public:
    Koordinator() : na_redu_je(PRVI) {}  // Konstruktor klase Koordinator. Na početku je na redu PRVI (nit a).

    // Funkcija se poziva nakon što je prva nit završila svoj posao, kako bi signalizirala pokretanje druge niti.
    void prvi_zavrsio() {                // Funkcija koja se poziva kada se završi prva nit (nit a).
        unique_lock<mutex> lock(m);      // Zaključavanje mutex-a (ulazak u kritičnu sekciju).
        c.notify_one();                  // Notifikacija niti koja čeka (nit b).
        na_redu_je = DRUGI;              // Promena uslova (na redu je sada DRUGI). Linija iznad i ova su MOGLE BITI U OBRNUTOM
    }                                    // REDOSLEDU jer tek po izlasku iz kritične sekcije nit a otpušta propusnicu.

    // Blokirajuća funkcija koja se poziva na početku druge niti kako bi nit čekala dok se prva nit ne završi.
    void drugi_ceka() {                  // Funkcija koja se poziva na početku druge niti (nit b).
        unique_lock<mutex> lock(m);
        while( !(na_redu_je == DRUGI) )  // Provera uslova čekanja. Ukoliko je na_redu_je != DRUGI ->
            c.wait(lock);                // druga nit (nit b) ulazi u čekanje. Kod provere uslova MORA stojati while (ne if)
   }                                     // da bi se predupredila lažna buđenja (spurious wakeups).
};

#endif // KOORDINATOR_H_INCLUDED
