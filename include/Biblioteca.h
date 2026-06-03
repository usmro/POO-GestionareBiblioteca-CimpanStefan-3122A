#pragma once
#include "Carte.h"
#include <sqlite3.h>
#include <vector>
#include <string>

class Biblioteca {
private:
    sqlite3* db;

public:
    Biblioteca();
    ~Biblioteca();

    // Functii Carti & Inventar
    void adaugaCarteFizica(const std::string& titlu, const std::string& autor, const std::string& sursa, const std::string& locatie);
    void stergeCarte(int id_carte); // NOU: Pentru bibliotecar
    std::vector<std::string> getCartiFormatate();
    std::vector<std::string> cautaCarti(const std::string& query); // NOU: Cautare

    // Functii Cititori & Imprumuturi
    void adaugaCititor(const std::string& nume);
    std::vector<std::string> getCititori();
    bool imprumutaCarte(int id_carte, const std::string& nume_cititor);
    bool returneazaCarte(int id_carte);
    bool rezervaCarte(int id_carte, const std::string& nume_cititor); // NOU: Rezervare
    bool prelungesteImprumut(int id_carte); // NOU: Prelungire
    bool anuleazaRezervare(int id_carte); // NOU: Anulare rezervare

    // AI
    std::string intreabaBiblioAI(const std::string& prompt);
};