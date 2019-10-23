#ifndef NITI_H
#define NITI_H

#include <thread>

using namespace std;

double izracunaj(list<string> kupljeno, map<string, double> cenovnik)
{
    double suma;
    thread t(obracunajUkupno, kupljeno, cenovnik, ref(suma));
    t.join();

    return suma;
}


#endif // NITI_H
