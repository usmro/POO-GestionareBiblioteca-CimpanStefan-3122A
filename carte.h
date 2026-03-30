#ifndef CARTE_H
#define CARTE_H

#include <string>
using namespace std;

// Clasa de baza
class Item {
protected:
    int id;
    string titlu;

public:
    Item();
    Item(int id, string titlu);

    virtual void afisare();
};

// Clasa derivata
class Carte : public Item {
private:
    string autor;

public:
    Carte();
    Carte(int id, string titlu, string autor);

    void afisare() override;

    string getTitlu();
    string getAutor();
};

#endif
