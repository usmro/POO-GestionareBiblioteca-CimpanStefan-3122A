#include "Biblioteca.h"
#include <iostream>

Biblioteca::Biblioteca() {
    // Deschide/Creează baza de date
    if (sqlite3_open("biblioteca.db", &db) != SQLITE_OK) {
        std::cerr << "Eroare DB\n";
    }

    // Creează tabela dacă nu există
    std::string query = "CREATE TABLE IF NOT EXISTS Carti ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "titlu TEXT, autor TEXT, sursa TEXT, locatie TEXT);";
    sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
}

Biblioteca::~Biblioteca() {
    sqlite3_close(db);
}

void Biblioteca::adaugaCarteFizica(const std::string& titlu, const std::string& autor, const std::string& sursa, const std::string& locatie) {
    std::string query = "INSERT INTO Carti (titlu, autor, sursa, locatie) VALUES ('" + 
                        titlu + "', '" + autor + "', '" + sursa + "', '" + locatie + "');";
    sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
}

std::vector<std::unique_ptr<Carte>> Biblioteca::getToateCartile() {
    std::vector<std::unique_ptr<Carte>> carti;
    sqlite3_stmt* stmt;
    std::string query = "SELECT * FROM Carti;";

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string t = (const char*)sqlite3_column_text(stmt, 1);
            std::string a = (const char*)sqlite3_column_text(stmt, 2);
            std::string s = (const char*)sqlite3_column_text(stmt, 3);
            std::string l = (const char*)sqlite3_column_text(stmt, 4);
            
            carti.push_back(std::make_unique<CarteFizica>(id, t, a, s, l));
        }
    }
    sqlite3_finalize(stmt);
    return carti;
}