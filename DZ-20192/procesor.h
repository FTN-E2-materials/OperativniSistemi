#ifndef PROCESOR_H_INCLUDED
#define PROCESOR_H_INCLUDED

#include "dijagnostika.h"
#include <deque>
#include <iostream>


struct Proces{
    condition_variable cv;
    int idProcesa;
    int kolicinaPosla;
    int prioritetProcesa;
    bool obradjen;
    Proces(int p,int kolPosl,int prio): idProcesa(p),kolicinaPosla(kolPosl),prioritetProcesa(prio),obradjen(false){}
};

using namespace std;
using namespace chrono;

class Procesor {
private:
    Dijagnostika& dijagnostika;
    bool zavrsen;
    int aktivanProces;                                  //promenljiva koja nam govori koji proces je u procesoru i izvrsava se
    mutex m;

    deque<Proces*> P0;                                  //jedan dek,dovoljan,jer ce u njemu biti procesi,koji imaju u sebi
                                                        //i info kog su prioriteta

    bool imaZahtev;                                     //promenljiva koja signalizira da se izvrsio proces i da moze rasporedjivanje nastupiti
    condition_variable cvR;                             //signalizator

public:
    Procesor(Dijagnostika& d) : dijagnostika(d), zavrsen(false) {
        // Proširiti po potrebi ...
        aktivanProces=-1;
        imaZahtev=false;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    void zavrsi() {
        zavrsen = true;
        // Proširiti po potrebi ...
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se proces izvršio
    //
    // id_procesa - Unikatni broj (identifikator) procesa
    // kolicina_posla - Koliko posla nit treba da izvrši (vreme izraženo u milisekundama)
    //
    // Ukoliko je procesor već zauzet i ne može se izvršavati proces, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Nakon što se proces u celosti izvršio, treba pozvati metodu dijagnostika.proces_zavrsio.
    void izvrsi(int id_procesa, int kolicina_posla, int prioritet) {
        // Implementirati ...
        unique_lock<mutex> l(m);

        if(aktivanProces==-1){
            aktivanProces=id_procesa;                       //ako je procesor slobodan,zauzima ga tekuci proces
        }

        while(aktivanProces!=id_procesa){                   //ako neko drugi vec koristi procesor,cekamo u svom redu
            dijagnostika.proces_ceka(id_procesa);
            Proces *x = new Proces(id_procesa,kolicina_posla,prioritet);    //napravimo proces
            P0.push_back(x);                                                //stavimo ga u dek
            x->cv.wait(l);

            delete x;
        }


        l.unlock();
        this_thread::sleep_for(milliseconds(kolicina_posla));
        l.lock();

        dijagnostika.proces_zavrsio(id_procesa);

        imaZahtev=true;
        cvR.notify_one();

    }

    // Metoda koju poziva nit koja simulira raspoređivač procesa
    //
    // Raspoređivač niti radi u posebnoj niti, te on treba da čeka dok god se ne pojavi zahtev za
    // novim raspoređivanjem procesa. Ovaj zahtev mu upućuju niti koje simuliraju procese. Kada raspoređivač
    // nema aktiviran zahtev, on treba da pozove dijagnostičku metodu dijagnostika.rasporedjivac_ceka.
    // Kada se raspoređivač aktivira i izabere proces koji treba da se izvrši sledeći (prema pravilima opisanim u
    // tekstu zadatka), potrebno je pozvati dijagnostičku metodu dijagnostika.rasporedjivac_izabrao.
    void rasporedjuj() {
        while (!zavrsen) {
            // Implementirati ...
            unique_lock<mutex> l(m);
            while(!imaZahtev){
                //dok nema zahtev ceka
                dijagnostika.rasporedjivac_ceka();
                cvR.wait(l);
            }
/*DEBAG
            cout <<"DEK:"<<endl;
            for(auto it=P0.begin();it != P0.end();it++){
                cout << (*it)->kolicinaPosla << endl;;
            }
*/
            //algoritam koji nalazi koji je sledeci
            //prvo nadje u tom deku,ko ima prioritet 0
            //kad naidje na takav,on je trenutno najmanji(najmanju kolicinu posla ima trenutno)
            //ovim algoritmom obezbedujemo da se prvo odrade procesi sa prioritetom 0, pa onda oni sa prioriteom 1
            auto najmanja=P0.begin();
            for(auto it=P0.begin();it != P0.end();it++){
                if((*it)->prioritetProcesa == 0){
                    najmanja=it;
                    //ako ima neko u deku prioritet 0,on ce biti onaj sa kojim cemo se porediti dalje
                    break;
                }
            }
            auto it=P0.begin();
            for(it;it != P0.end();it++){
                if( (*it)->kolicinaPosla < (*najmanja)->kolicinaPosla && (*it)->prioritetProcesa <= (*najmanja)->prioritetProcesa){
                    najmanja=it;
                }
            }

            aktivanProces=(*najmanja)->idProcesa;               //kazemo koji je sledeci aktivanProces,tj ko ide u procesor
            (*najmanja)->cv.notify_one();                       //notificiramo BAS NJEGA,tj kao opet ga pokrenemo
            P0.erase(najmanja);                                 //uklonimo ga iz deka spremnih procesa na cpu


            imaZahtev=false;
            dijagnostika.rasporedjivac_izabrao(aktivanProces);

        }
    }

};

#endif
