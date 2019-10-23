#ifndef MAGISTRALA_H_INCLUDED
#define MAGISTRALA_H_INCLUDED

#include "dijagnostika.h"

using namespace std;

// Klasa magistrale (sabirnice). Glavna deljena promenljiva za sinhronizaciju pristupa procesora memoriji i dma kontroleru.
class Magistrala {
private:
    Dijagnostika& dijagnostika;

public:
    // Stanja sabirnice - slobodna, čitanje iz memorije, pisanje u memoriju, dma transfer. Primetiti sličnost sa stanjima iz zadatka "multiprocesor":
    enum Stanje { SLOBODNA, MEM_CITAJ, MEM_PISI, DMA};

    // Struktura u koju se beleže parametri DMA transfera. Odakle, kome i koliko bajtova se čita ili piše.
    struct DMA_transfer {
        int odakle;
        int koliko;
        int kome;
    };

    Magistrala(Dijagnostika& d, Memorija& mem) : dijagnostika(d) {
        // Proširiti po potrebi ...
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    char citaj_memoriju(int adresa) {
        // Implementirati ...
    }

    void pisi_u_memoriju(int adresa, char vrednost) {
        // Implementirati ...
    }

    void dma(DMA_transfer transfer) {
        // Implementirati ...
    }

    DMA_transfer okidac_dma_kontrolera() {
        // Implementirati ...
    }
};

#endif // MAGISTRALA_H_INCLUDED
