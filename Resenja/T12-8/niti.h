#ifndef NITI_H
#define NITI_H

#include <thread>

using namespace std;

map<string, double> izracunaj(map<string, double>& a, map<string, double>& b)
{
    map<string, double> rez;
    thread t(nadjiPovoljno, a, b, ref(rez));
    t.join();

    return rez;
}

#endif // NITI_H
