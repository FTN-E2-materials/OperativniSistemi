// Tekst zadatka je u PDF fajlu

#include <thread>
#include "procesor.h"

using namespace std;
using namespace chrono;

// Funkcija niti koje predstavljaju aktere simulacije (procese):
void proces(Procesor& p, int id_procesa, int kolicina_posla, int prioritet) {
    p.getDijagnostika().proces_kreiran(id_procesa, kolicina_posla, prioritet);
    p.izvrsi(id_procesa, kolicina_posla, prioritet);
}

// Funkcija niti koja vrši raspoređivanje niti:
void rasporedjivac(Procesor& p) {
    p.rasporedjuj();
}

const int NITI = 9;

void testirajSve() {
    Dijagnostika d;
    Procesor p(d);
    thread tr(rasporedjivac, ref(p));
    tr.detach();

    int kolicina_posla[NITI] = {7,8,1,3,1,0,2,4,6};
    int prioriteti[NITI]     = {0,0,1,0,1,1,0,0,0};

    thread procesi[NITI];
    for (int i = 0; i < NITI; i++) {
        procesi[i] = thread(proces, ref(p), i + 1, kolicina_posla[i] * 100, prioriteti[i]);
    }
    for (int i = 0; i < NITI; i++)
        procesi[i].join();

    // Signaliziranje klasi Procesor da su sve niti završene (kako bi se i detach-ovana nit završila):
    p.zavrsi();
}

int main() {
    testirajSve();

    exit(0);
}
