#ifndef RASPOREDJIVAC_H_INCLUDED
#define RASPOREDJIVAC_H_INCLUDED

#include <vector>
#include <mutex>

#include "dijagnostika.h"
#include "red.h"

using namespace std;
using namespace chrono;

class Rasporedjivac {
private:
    Dijagnostika& dijagnostika;
	int aktivni_proces;     // ID aktivnog procesa
	vector<Red> redovi;     // Vektor redova čekanja pri čemu je svaki red vezan za određeni nivo prioriteta
	mutex m;

public:
    Rasporedjivac(Dijagnostika& d, int broj_nivoa_prioriteta) : dijagnostika(d) {
        aktivni_proces = -1;
        for (int i = 0; i < broj_nivoa_prioriteta; i++) // Stvaranje vektora redova prioriteta
            redovi.push_back(Red(i));
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se proces izvršio
    //
    // id_procesa   - ID procesa
    // broj_naredbi - ukupan broj naredbi koje proces treba da izvrši
    // prioritet    - prioritet procesa
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Nakon što je proces izvršio naredbu, potrebno je pozvati dijagnostika.izvrsio_naredbu.
	void izvrsi(int id_procesa, int broj_naredbi, int prioritet) {
        for (int i =0; i < broj_naredbi; ++i) {
            unique_lock<mutex> l(m);

            if (aktivni_proces == -1) {         // Ako niko ne drži procesor, zauzima se
                aktivni_proces = id_procesa;
            }

            while (aktivni_proces != id_procesa) {  // Ako neko već drži procesor, čeka se u redu svog prioriteta
                dijagnostika.proces_ceka(id_procesa);
                redovi[prioritet].dodaj_u_red(id_procesa, l);
            }

            l.unlock();
            this_thread::sleep_for(milliseconds(300));  // Simulacija izvršavanja naredbe
            l.lock();

            int obavesti_red = -1;
            // Nađe se prvi red u kojem neki proces čeka, počevši od najvišeg prioriteta pa idući na dole
            for (auto it = redovi.begin(); it != redovi.end(); it++) {
                if (!it->prazan()) {
                    obavesti_red = it->preuzmi_prioritet();
                    break;
                }
            }

            // Ako je nađeni red nižeg prioriteta od prioriteta trenutnog procesa, treba da nastavi trenutni proces ako ima još naredbi.
            // Ova situacija će se desiti kada je proces jedini u svom nivou prioriteta.
            if (obavesti_red != -1) {
                int sledeci;
                if (obavesti_red > prioritet &&	i < broj_naredbi-1) // Ako nema niko prioritetniji i ako proces ima još naredbi nastavlja
                    sledeci = id_procesa;
                else
                    sledeci = redovi[obavesti_red].izbaci_iz_reda();    // U suprotnom, obaveštava se taj prioritetniji ili sledeći
                aktivni_proces = sledeci;								// manje prioritetan ako trenutni proces nema više naredbi
            }
            else
                aktivni_proces = -1;    // Ako ništa od ovog nije slučaj to znači da niko ne čeka na procesor i on se oslobađa

            dijagnostika.izvrsio_naredbu(id_procesa, i);
        }
	}
};

#endif // RASPOREDJIVAC_H_INCLUDED
