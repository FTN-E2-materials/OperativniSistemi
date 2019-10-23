/* 
Napisati program koji omogućava evidentiranje fakultetskih učionica.

Za svaku učionicu potrebno je evidentirati naziv učionice, kvadraturu i sve računare koji se nalaze u učionici. 
Za svaki računar evidentiraju se podaci o hard disku i procesoru.

U okviru main funkcije, definisati jednu učionicu koja sadrži dva računara.
Zatim je potrebno na konzolu ispisati hard disk i procesor svakog računara u učionici.
*/

#include <iostream>
#include <string>
#include <list>

#define MAX 100

using namespace std;

// Klasa modeluje računar za koji se evidentira kapacitet diska i naziv procesora
class Racunar {
    private:
        string disk;
        string procesor;
    public:
        Racunar() {
        }
    	Racunar(string d, string p) {
    		disk = d;
    		procesor = p;
    	}
        void setDisk(string d) {
            disk = d;
        }
        string getDisk() {
            return disk;
        }
        void setProcesor(string p) {
            procesor = p;
        }
        string getProcesor() {
            return procesor;
        }
};

// Klasa modeluje učionicu za koju se evidentira naziv, kvadratura i računari koji se nalaze u učionici
class Ucionica {
    private:
        string naziv;
        double kvadratura;
        //Racunar racunari[MAX]; // prva varijanta - računari u učionici su predstavljeni nizom
        list<Racunar> racunari; // druga varijanta - računari u učionici su elementi STL kontejnera lista
    public:
    	Ucionica (string n, double kv) {
    		naziv = n;
    		kvadratura = kv;
    	}
        void setNaziv(string br) {
            naziv = br;
        }
        string getNaziv() {
            return naziv;
        }
        void setKvadratura(double kv) {
            kvadratura = kv;
        }
        double getKvadratura() {
            return kvadratura;
        }
        //Sa nizom:
        /*void dodajRacunar(Racunar r, int i) {
            racunari[i] = r;
        }
        Racunar preuzmiRacunar(int i) {
            return racunari[i];
        }*/
        void dodajRacunar(Racunar r) {
            racunari.push_back(r); // Ubacivanje objekta klase Racunar na kraj STL kontejnera lista
        }
        list<Racunar> getRacunari() {
            return racunari;
        }
};


int main()
{
    // Kreiranje jednog objekta klase Ucionica:
    Ucionica u("MIA2-3", 150);

    // Kreiranje dva objekta klase Racunar:
    Racunar r1("Seagate", "I5");
    Racunar r2("WD", "I7");

    // Prva varijanta - rad sa nizom:
    /*u.dodajRacunar(r1,0);
    u.dodajRacunar(r2,1);

    for (int i = 0; i < 2; i++) {
        cout << "Disk: " << u.preuzmiRacunar(i).getDisk() << endl;
    }*/

    // Druga varijanta - rad sa STL kontejnerom:
    u.dodajRacunar(r1);
    u.dodajRacunar(r2);

    list<Racunar> racunari = u.getRacunari(); // Preuzimanje liste svih računara
    list<Racunar>::iterator it; // Deklaracija iteratora na element STL kontejnera
    // Iterator inicijalno pokazuje na prvi element kontejnera. Na kraju svake iteracije se pomeri na naredni element.
    // U petlji se ostaje dok god se ne dođe do kraja kontejnera (metoda end() vraća iterator koji pokazuje "nakon" poslednjeg elementa).
    // Pošto elementi u listi mogu biti na proizvoljnim memorijskim lokacijama, pri utvrđivanju uslova izlaska iz petlje ne možemo porediti operatorom "<", jer ako posmatramo dva elementa, 
    // element sa nižim indeksom u kontejneru može biti na višoj adresi u memoriji.
    int i = 0;
    for (it = racunari.begin(); it != racunari.end(); it++, i++) { // Ispisujemo podatke o hard diskovima i procesorima pojedinačnih
        cout << "Racunar [" << i << "]:" << endl;                  // računara prolaskom iteratorom kroz listu i preuzimanjem odgovarajućih
        cout << "Hard disk: " << it->getDisk() << endl;            // polja
        cout << "Procesor: " << it->getProcesor() << endl << endl;
    }
}
