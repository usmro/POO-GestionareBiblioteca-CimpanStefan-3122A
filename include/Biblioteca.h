#pragma once
#include "Carte.h"
#include <sqlite3.h>
#include <vector>
#include <memory>
#include <string>

class Biblioteca {
private:
    sqlite3* db;

public:
    Biblioteca();
    ~Biblioteca();

    // Functii Carti
    void adaugaCarteFizica(const std::string& titlu, const std::string& autor, const std::string& sursa, const std::string& locatie);
    std::vector<std::string> getCartiFormatate();

    // Functii Cititori & Imprumut
    void adaugaCititor(const std::string& nume);
    std::vector<std::string> getCititori();
    bool imprumutaCarte(int id_carte, const std::string& nume_cititor);
    bool returneazaCarte(int id_carte);

    // AI
    std::string intreabaBiblioAI(const std::string& prompt);
};