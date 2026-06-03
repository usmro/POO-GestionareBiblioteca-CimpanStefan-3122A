#pragma once
#include <sqlite3.h>
#include <vector>
#include <string>

class Biblioteca {
private:
    sqlite3* db;
    void initializeazaTabele();

public:
    Biblioteca();
    ~Biblioteca();

    void adaugaCarte(const std::string& titlu, const std::string& autor, const std::string& isbn, const std::string& editura, const std::string& an, const std::string& locatie, int stoc);
    void stergeCarte(int id);
    std::vector<std::string> getToateCartile();
    std::vector<std::string> getCartiDisponibile();
    std::vector<std::string> getCartiImprumutateDeUser(const std::string& nume);
    std::vector<std::string> getToateImprumuturile(); // NOU - pentru bibliotecar
    std::vector<std::string> cautaCarti(const std::string& query);
    
    void adaugaCititor(const std::string& nume);
    std::vector<std::string> getCititori();
    bool inregistreazaUtilizator(const std::string& nume, const std::string& email, const std::string& parola);
    std::string autentificaUtilizator(const std::string& nume, const std::string& parola);

    bool imprumutaCarte(int id_carte, const std::string& nume_cititor);
    bool returneazaCarte(int id_carte, const std::string& nume_cititor);
    bool returneazaFortat(int id_carte); // NOU - pentru bibliotecar
    bool rezervaCarte(int id_carte, const std::string& nume_cititor);
    bool prelungesteImprumut(int id_carte);

    std::string intreabaBiblioAI(const std::string& prompt);
    std::vector<std::string> iaTendinteOnline();

    // Ascundere stoc carti
    std::vector<std::string> getCartiPentruUser();
    std::vector<std::string> getCartiDisponibilePentruUser();
};