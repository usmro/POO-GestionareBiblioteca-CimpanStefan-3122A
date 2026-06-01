#include "Biblioteca.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// --- 1. CONSTRUCTOR & DESTRUCTOR (Sigur, cu verificare erori) ---
Biblioteca::Biblioteca() {
    // Verificăm dacă baza de date se deschide cu succes
    if (sqlite3_open("biblioteca.db", &db) != SQLITE_OK) {
        std::cerr << "EROARE CRITICĂ: Nu s-a putut deschide baza de date!" << std::endl;
        std::cerr << "Motiv: " << (db ? sqlite3_errmsg(db) : "Eroare necunoscută") << std::endl;
        exit(1); // Oprește programul controlat, evitând Segmentation Fault
    }
    
    // Inițializăm tabelele doar dacă db este valid
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Carti (id INTEGER PRIMARY KEY AUTOINCREMENT, titlu TEXT, autor TEXT, sursa TEXT, locatie TEXT, imprumutat_la TEXT DEFAULT '');", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Cititori (id INTEGER PRIMARY KEY AUTOINCREMENT, nume TEXT UNIQUE);", nullptr, nullptr, nullptr);
}

Biblioteca::~Biblioteca() { 
    if (db != nullptr) {
        sqlite3_close(db); 
    }
}

// --- 2. GESTIUNE CĂRȚI ---
void Biblioteca::adaugaCarteFizica(const std::string& titlu, const std::string& autor, const std::string& sursa, const std::string& locatie) {
    std::string q = "INSERT INTO Carti (titlu, autor, sursa, locatie) VALUES ('" + titlu + "', '" + autor + "', '" + sursa + "', '" + locatie + "');";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
}

std::vector<std::string> Biblioteca::getCartiFormatate() {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT id, titlu, autor, imprumutat_la FROM Carti;", -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string id = std::to_string(sqlite3_column_int(stmt, 0));
            std::string t = (const char*)sqlite3_column_text(stmt, 1);
            std::string a = (const char*)sqlite3_column_text(stmt, 2);
            std::string imp = (const char*)sqlite3_column_text(stmt, 3);
            
            std::string status = imp.empty() ? "[DISPONIBIL]" : "[IMPRUMUTAT LA: " + imp + "]";
            lista.push_back("ID " + id + " | " + t + " de " + a + " " + status);
        }
    }
    sqlite3_finalize(stmt);
    return lista;
}

// --- 3. GESTIUNE CITITORI & ÎMPRUMUTURI ---
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
    std::string q = "UPDATE Carti SET imprumutat_la = '" + nume_cititor + "' WHERE id = " + std::to_string(id_carte) + " AND imprumutat_la = '';";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
    return sqlite3_changes(db) > 0;
}

bool Biblioteca::returneazaCarte(int id_carte) {
    std::string q = "UPDATE Carti SET imprumutat_la = '' WHERE id = " + std::to_string(id_carte) + ";";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
    return sqlite3_changes(db) > 0;
}

// --- 4. ASISTENT AI GEMINI ---
std::string Biblioteca::intreabaBiblioAI(const std::string& prompt) {
    std::string apiKey = "";
    
    // Citim cheia manual din fisierul .env creat de tine
    std::ifstream envFile(".env");
    if (envFile.is_open()) {
        std::string linie;
        while (std::getline(envFile, linie)) {
            if (linie.find("GEMINI_API_KEY=") == 0) {
                apiKey = linie.substr(15); 
                break;
            }
        }
        envFile.close();
    }

    if (apiKey.empty()) {
        return "EROARE: Fisierul .env nu a fost gasit sau GEMINI_API_KEY lipseste!";
    }

    std::string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + apiKey;
    json body = {{"contents", json::array({{{"parts", json::array({{{"text", "Esti BiblioAI, un bibliotecar robot politicos din terminal. Raspunde scurt si util: " + prompt}}})}}})}};
    
    auto r = cpr::Post(cpr::Url{url}, cpr::Header{{"Content-Type", "application/json"}}, cpr::Body{body.dump()});

    if (r.status_code == 200) {
        try { return json::parse(r.text)["candidates"][0]["content"]["parts"][0]["text"]; } 
        catch (...) { return "Eroare la procesarea raspunsului AI."; }
    }
    return "Eroare de retea. Cod: " + std::to_string(r.status_code);
}