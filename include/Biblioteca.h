#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H
#include "Carte.h"
#include <vector>

class Biblioteca {
private:
    std::vector<Carte*> colectie; // STL Container
public:
    void adaugaCarte(Carte* c);
    void afisareToate() const;
    void cautaDupaTitlu(std::string titlu) const; // Motorul de cautare din notite
    ~Biblioteca();
};
#endif
