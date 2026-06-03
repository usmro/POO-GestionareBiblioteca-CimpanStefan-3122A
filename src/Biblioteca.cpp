#include "Biblioteca.h"
#include <iostream>
#include <fstream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Biblioteca::Biblioteca() {
    if (sqlite3_open("biblioteca.db", &db) != SQLITE_OK) {
        std::cerr << "EROARE CRITICĂ: Nu s-a putut deschide baza de date!" << std::endl;
        exit(1);
    }
    
    // Creăm tabelele de bază
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Carti (id INTEGER PRIMARY KEY AUTOINCREMENT, titlu TEXT, autor TEXT, sursa TEXT, locatie TEXT, imprumutat_la TEXT DEFAULT '', rezervat_de TEXT DEFAULT '');", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Cititori (id INTEGER PRIMARY KEY AUTOINCREMENT, nume TEXT UNIQUE);", nullptr, nullptr, nullptr);
    
    // Încercăm să adăugăm coloana 'rezervat_de' dacă nu există (pentru bazele de date vechi)
    sqlite3_exec(db, "ALTER TABLE Carti ADD COLUMN rezervat_de TEXT DEFAULT '';", nullptr, nullptr, nullptr);
}

Biblioteca::~Biblioteca() { if (db != nullptr) sqlite3_close(db); }

void Biblioteca::adaugaCarteFizica(const std::string& titlu, const std::string& autor, const std::string& sursa, const std::string& locatie) {
    std::string q = "INSERT INTO Carti (titlu, autor, sursa, locatie) VALUES ('" + titlu + "', '" + autor + "', '" + sursa + "', '" + locatie + "');";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
}

void Biblioteca::stergeCarte(int id_carte) {
    std::string q = "DELETE FROM Carti WHERE id = " + std::to_string(id_carte) + ";";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
}

std::vector<std::string> Biblioteca::getCartiFormatate() {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT id, titlu, autor, imprumutat_la, rezervat_de FROM Carti;", -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string t = (const char*)sqlite3_column_text(stmt, 1);
            std::string a = (const char*)sqlite3_column_text(stmt, 2);
            std::string imp = (const char*)sqlite3_column_text(stmt, 3);
            std::string rez = (const char*)sqlite3_column_text(stmt, 4);
            
            std::string status = "[DISPONIBIL]";
            if (!imp.empty()) status = "[IMPRUMUTAT LA: " + imp + "]";
            else if (!rez.empty()) status = "[REZERVAT: " + rez + "]";
            
            lista.push_back("ID " + std::to_string(id) + " | " + t + " de " + a + " " + status);
        }
    }
    sqlite3_finalize(stmt);
    return lista;
}

std::vector<std::string> Biblioteca::cautaCarti(const std::string& query) {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    std::string search = "%" + query + "%";
    if (sqlite3_prepare_v2(db, "SELECT id, titlu, autor, imprumutat_la, rezervat_de FROM Carti WHERE titlu LIKE ? OR autor LIKE ?;", -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, search.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, search.c_str(), -1, SQLITE_STATIC);
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string t = (const char*)sqlite3_column_text(stmt, 1);
            std::string a = (const char*)sqlite3_column_text(stmt, 2);
            std::string imp = (const char*)sqlite3_column_text(stmt, 3);
            std::string rez = (const char*)sqlite3_column_text(stmt, 4);
            
            std::string status = "[DISPONIBIL]";
            if (!imp.empty()) status = "[IMPRUMUTAT LA: " + imp + "]";
            else if (!rez.empty()) status = "[REZERVAT: " + rez + "]";
            
            lista.push_back("ID " + std::to_string(id) + " | " + t + " de " + a + " " + status);
        }
    }
    sqlite3_finalize(stmt);
    return lista;
}

void Biblioteca::adaugaCititor(const std::string& nume) {
    std::string q = "INSERT OR IGNORE INTO Cititori (nume) VALUES ('" + nume + "');";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
}

std::vector<std::string> Biblioteca::getCititori() {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT nume FROM Cititori;", -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) lista.push_back((const char*)sqlite3_column_text(stmt, 0));
    }
    sqlite3_finalize(stmt);
    return lista;
}

bool Biblioteca::imprumutaCarte(int id_carte, const std::string& nume_cititor) {
    // Verificăm să nu fie deja rezervată de altcineva
    std::string q = "UPDATE Carti SET imprumutat_la = '" + nume_cititor + "', rezervat_de = '' WHERE id = " + std::to_string(id_carte) + " AND imprumutat_la = '';";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
    return sqlite3_changes(db) > 0;
}

bool Biblioteca::returneazaCarte(int id_carte) {
    std::string q = "UPDATE Carti SET imprumutat_la = '' WHERE id = " + std::to_string(id_carte) + ";";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
    return sqlite3_changes(db) > 0;
}

bool Biblioteca::rezervaCarte(int id_carte, const std::string& nume_cititor) {
    std::string q = "UPDATE Carti SET rezervat_de = '" + nume_cititor + "' WHERE id = " + std::to_string(id_carte) + " AND imprumutat_la = '' AND rezervat_de = '';";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
    return sqlite3_changes(db) > 0;
}

bool Biblioteca::prelungesteImprumut(int id_carte) {
    // Într-o aplicație reală am actualiza o coloană 'termen_limita'. 
    // Aici simulăm succesul dacă cartea este împrumutată.
    std::string q = "UPDATE Carti SET termen_limita = 'prelungit' WHERE id = " + std::to_string(id_carte) + " AND imprumutat_la != '';";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
    return sqlite3_changes(db) > 0;
}

bool Biblioteca::anuleazaRezervare(int id_carte) {
    std::string q = "UPDATE Carti SET rezervat_de = '' WHERE id = " + std::to_string(id_carte) + ";";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
    return sqlite3_changes(db) > 0;
}

std::string Biblioteca::intreabaBiblioAI(const std::string& prompt) {
    std::string apiKey = "";
    std::ifstream envFile(".env");
    if (envFile.is_open()) {
        std::string linie;
        while (std::getline(envFile, linie)) {
            if (linie.find("GEMINI_API_KEY=") == 0) {
                apiKey = linie.substr(15); break;
            }
        }
        envFile.close();
    }
    if (apiKey.empty()) return "EROARE: Fisierul .env nu a fost gasit!";

    std::string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + apiKey;
    json body = {{"contents", json::array({{{"parts", json::array({{{"text", "Esti BiblioAI, un bibliotecar robot politicos. Raspunde scurt: " + prompt}}})}}})}};
    
    auto r = cpr::Post(cpr::Url{url}, cpr::Header{{"Content-Type", "application/json"}}, cpr::Body{body.dump()});
    if (r.status_code == 200) {
        try { return json::parse(r.text)["candidates"][0]["content"]["parts"][0]["text"]; } 
        catch (...) { return "Eroare la procesarea raspunsului AI."; }
    }
    return "Eroare de retea.";
}