#include "Carte.h"

Carte::Carte(int id, std::string t, std::string a, std::string s)
    : id(id), titlu(t), autor(a), sursa(s) {}

int Carte::getId() const { return id; }
std::string Carte::getTitlu() const { return titlu; }
std::string Carte::getAutor() const { return autor; }
std::string Carte::getSursa() const { return sursa; }

CarteFizica::CarteFizica(int id, std::string t, std::string a, std::string s, std::string loc)
    : Carte(id, t, a, s), locatie(loc) {}

std::string CarteFizica::getDetaliiComplete() const {
    return titlu + " | Autor: " + autor + " | Locatie: " + locatie + " (" + sursa + ")";
}