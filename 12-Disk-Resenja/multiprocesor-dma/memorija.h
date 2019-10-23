#ifndef MEMORIJA_H_INCLUDED
#define MEMORIJA_H_INCLUDED

#include <vector>
#include <mutex>

using namespace std;

class Memorija {
private:
    mutex m;
    vector<char> mem;                           //Vektor memorijskih lokacija. Svaka lokacija jedan char.

public:
    Memorija(int bajtova) {
        mem.resize(bajtova, 0);
    }

    // Funkcija čitanja iz memorije. Zaključava se svako čitanje radi čuvanja konzistentnosti memorijske lokacije.
    char citaj(int adresa) {
        unique_lock<mutex> l(m);
        this_thread::sleep_for(chrono::milliseconds(300));
        return mem[adresa];
    }

    // Funkcija pisanja u memoriju. Zaključava se svako pisanje u memoriju.
    void pisi(int adresa, char vrednost) {
        unique_lock<mutex> l(m);                           // Radi očuvanja konzistentnosti memorijske lokacije. Obratiti
        this_thread::sleep_for(chrono::milliseconds(300));  // pažnju na razliku u odnosu na rešenje u zadatku "multiprocesor" --
        mem[adresa] = vrednost;                             // Ovde nema provere stanja, brojača... Sva sinhronizacija se vrši
                                                            // na nivou Magistrala-a (magistrale tj. sabirnice).
    }
};

#endif // MEMORIJA_H_INCLUDED
