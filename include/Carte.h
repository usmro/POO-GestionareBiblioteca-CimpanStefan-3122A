#ifndef CARTE_H
#define CARTE_H
#include <string>

struct Locatie { 
    std::string cladire; 
    int camera; 
};

class Carte {
protected:
    std::string titlu, autor;
    float pret;
public:
    Carte(std::string t, std::string a, float p);
    virtual ~Carte();
    virtual void afisareDetalii() const = 0; // Polimorfism
    std::string getTitlu() const;
};

class CarteFizica : public Carte {
    float greutate;
    Locatie loc;
public:
    CarteFizica(std::string t, std::string a, float p, float g, Locatie l);
    void afisareDetalii() const override;
};

class CarteDigitala : public Carte {
    std::string format; // pdf, audiobook
public:
    CarteDigitala(std::string t, std::string a, float p, std::string f);
    void afisareDetalii() const override;
};
#endif
