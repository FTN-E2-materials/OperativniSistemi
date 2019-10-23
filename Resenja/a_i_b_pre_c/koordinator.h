#ifndef KOORDINATOR_H_INCLUDED
#define KOORDINATOR_H_INCLUDED

#include <mutex>
#include <condition_variable>

using namespace std;

class Koordinator {
private:
    const int cekam_niti;     // Konstanta koja govori koliko niti se čeka na završetak pre nego što se aktivira poslednja.
    int zavrseno_niti;        // Brojač koji govori koliko je niti završeno. Upoređuje se sa konstantom cekam_niti.
    mutex m;                  // Mutex i uslovna promenljiva. Neophodni UVEK za eksplicitnu sinhronizaciju između niti.
    condition_variable c;
public:
    Koordinator(int cn) : cekam_niti(cn), zavrseno_niti(0) {} // Konstruktor. Čeka se onoliko niti koliko se prosledi u parametru cn.

    // Metoda koju na svom KRAJU zovu niti (a i b) koje ne čekaju (izvršavaju se odmah).
    void zavrsio() {
        unique_lock<mutex> lock(m);
        if(++zavrseno_niti == cekam_niti) // Provera brojača. Ukoliko je završeno niti == cekam_niti, obaveštavaju se sve niti (c)
            c.notify_all();               // koje čekaju da se prethodne niti (a i b) završe. Ovo se naziva i barijera.
    }

    // Metodu koju na svom POČETKU zovu niti (c) koje čekaju (ne izvršavaju se odmah).
    void cekam() {
        unique_lock<mutex> lock(m);
        while(!(zavrseno_niti == cekam_niti))  // Provera brojača. Ukoliko zavrseno_niti != cekam_niti, nit pozivaoc (c) ulazi
            c.wait(lock);                      // u čekanje ispunjenosti uslova. Mora WHILE. Još jedan tip uslova osim enumeracije
    }                                          // jeste brojač (kao ovde). Takođe mogući su i bool uslovi.
};

#endif // KOORDINATOR_H_INCLUDED
