#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <vector>
#include "Carte.h"

class Biblioteca {
private:
    vector<Carte> carti;

public:
    void adaugaCarte();
    void afiseazaCarti();
    void salveazaInFisier();

    void incarcaDinFisier();
    void cautaCarte();
    void editeazaCarte();
    void stergeCarte();
};

#endif
