#pragma once
#include <string>

// Clasa de Bază
class Carte {
protected:
    int id;
    std::string titlu;
    std::string autor;
    std::string sursa; // Donatie, Anticariat, Noua

public:
    Carte(int id, std::string t, std::string a, std::string s);
    virtual ~Carte() = default;

    virtual std::string getDetaliiComplete() const = 0; // Polimorfism
    
    // Getteri
    int getId() const;
    std::string getTitlu() const;
    std::string getAutor() const;
    std::string getSursa() const;
};

// Clasa Derivată: Cărți Fizice
class CarteFizica : public Carte {
private:
    std::string locatie; // ex: Raionul A1

public:
    CarteFizica(int id, std::string t, std::string a, std::string s, std::string loc);
    std::string getDetaliiComplete() const override;
};