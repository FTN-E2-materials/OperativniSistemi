/*
Napraviti program koji ispituje ispravnost jednog od Marfijevih zakona, koji glasi: "red u kojem ste odlučili da čekate je najsporiji".

Program predstavlja simulaciju rada tri kase za naplatu robe. Svaka kasa ispred sebe ima red kupaca i svaka kasa je simulirana radom jedne programske niti. Potrebno je implementirati funkciju kasa, koja simulira rad jedne kase.

Program već sadrži implementiranu funkciju main koja radi sledeće:
- Pita korisnika u koji red želi da stane da čeka (redovi su označeni brojevima 0, 1 i 2).
- Kreira se tri niti, pri čemu svaka simulira funkcionisanje kase u odgovarajućem redu čekanja.
- Meri se ukupno vreme čekanje u svakom od redova (ispred svake od kasa).

Prilikom kreiranja, svaka nit dobija parametar koji predstavlja broj kupaca koje kasa treba da opsluži. Ovo je slučajan broj u opsegu 1..30, koji se generiše u funkciji main. Opsluživanje svakog kupca simulirati uspavljivanjem niti na slucajan vremenski period izmedju 1 i 100 ms. Takođe, funkcija kasa treba da meri ukupno vreme obrade svih kupaca i to vreme treba da vrati kroz parametar vreme_na_kasi.

Pošto se implementira funkcija kasa, potrebno je još implementirati u funkciji main:
- Naći koji je red (0, 1 ili 2) najduže obrađivan.
- Uporediti to sa redom koji je korisnik izabrao. Ako je korisnik izabrao najsporiji red, ispisati da je Marifijev zakon potvrđen, u suprotnom ispisati da je Marfi, u ovoj konkretnoj simulaciji, na žalost omanuo.
*/

#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;

const int UKUPNO_KASA = 3;
const int MAX_MILISEKUNDI_PO_KUPCU = 100;

void kasa(int broj_kupaca, duration<double, milli>& vreme_na_kasi) {
    steady_clock::time_point pocetak = steady_clock::now(); // Evidentiramo početak rada kase
    for (int i = 0; i < broj_kupaca; i++) // Simulacija kupovine. Svaki kupac provodi slučajan vremenski period na kasi
        this_thread::sleep_for(milliseconds(rand() % MAX_MILISEKUNDI_PO_KUPCU + 1));

    steady_clock::time_point kraj = steady_clock::now(); // Evidentiranje završetka rada kasa

    vreme_na_kasi = kraj - pocetak; // Upišemo koliko je trajala obrada svih kupaca
}

int main() {
    srand(time(NULL)); // Inicijalizacija generatora slučajnih brojeva da pri svakom pokretanju daje različite brojeve

    // Određivanje kase na koju klijent želi da ode:
    int moja_kasa;
    cout << "Unesite kasu na kojoj cete stati u red (0, 1 ili 2):" << endl;
    cin >> moja_kasa;

    // Niz sadrži vremena čekanja na svakoj od kasa
    duration<double, milli> vremena[UKUPNO_KASA];

    thread niti[UKUPNO_KASA];
    // Kasi se prosleđuje koliko kupaca dolazi i promenljiva u koju treba da upiše trajanje obrade svih kupaca:
    for (int i = 0; i < UKUPNO_KASA; ++i) {
        niti[i] = thread(kasa, rand()%30 + 1, ref(vremena[i]));
    }

    for (int i = 0; i < UKUPNO_KASA; ++i) {
        niti[i].join();
    }

    // Nakon završetka rada niti, u nizu "vremena" su evidentirana trajanja kupovine na svakoj od kasa, sada među tim vremenima treba naći najduže:
    duration<double, milli> max_trajanje = vremena[0];
    for (int i = 0; i < UKUPNO_KASA; i++) {
        cout << "Ukupno trajanje placanja na kasi " << i << " je: "
            << vremena[i].count() << " milisekundi. " << endl;

        if (vremena[i] > max_trajanje) {
            max_trajanje = vremena[i];
        }
    }

    // Ukoliko je najduže trajala kupovina na kasi koju je kupac izabrao, onda je Marfijev zakon potrvrđen:
    if (vremena[moja_kasa] == max_trajanje)
        cout << "Vas red je najsporiji. Marfijev zakon potvrdjen." << endl;
    else
        cout << "Marfijev zakon nije potvrdjen." << endl;
}
