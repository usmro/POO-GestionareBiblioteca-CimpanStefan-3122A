#include "Biblioteca.h"
#include <iostream>
#include <fstream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

// Funcție helper pentru SHA-256
std::string hashuiesteParola(const std::string& parola) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)parola.c_str(), parola.length(), hash);
    std::ostringstream oss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return oss.str();
}

Biblioteca::Biblioteca() {
    if (sqlite3_open("biblioteca.db", &db) != SQLITE_OK) {
        std::cerr << "EROARE DB!" << std::endl;
        exit(1);
    }
    
    // Tabele
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Carti (id INTEGER PRIMARY KEY AUTOINCREMENT, titlu TEXT, autor TEXT, sursa TEXT, locatie TEXT, imprumutat_la TEXT DEFAULT '', rezervat_de TEXT DEFAULT '');", nullptr, nullptr, nullptr);

    sqlite3_exec(db, "ALTER TABLE Carti ADD COLUMN isbn TEXT DEFAULT 'N/A';", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "ALTER TABLE Carti ADD COLUMN editura TEXT DEFAULT 'N/A';", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "ALTER TABLE Carti ADD COLUMN an_publicare TEXT DEFAULT 'N/A';", nullptr, nullptr, nullptr);

    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Cititori (id INTEGER PRIMARY KEY AUTOINCREMENT, nume TEXT UNIQUE, email TEXT UNIQUE, parola TEXT, rol TEXT DEFAULT 'user');", nullptr, nullptr, nullptr);
    
    // Actualizări tabele vechi
    sqlite3_exec(db, "ALTER TABLE Cititori ADD COLUMN email TEXT;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "ALTER TABLE Cititori ADD COLUMN parola TEXT;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "ALTER TABLE Cititori ADD COLUMN rol TEXT DEFAULT 'user';", nullptr, nullptr, nullptr);

    // Conturi implicite
    std::string admin_pass_hash = hashuiesteParola("admin123");
    sqlite3_exec(db, ("INSERT OR IGNORE INTO Cititori (nume, parola, rol) VALUES ('admin', '" + admin_pass_hash + "', 'admin');").c_str(), nullptr, nullptr, nullptr);

    std::string biblio_pass_hash = hashuiesteParola("1234");
    sqlite3_exec(db, ("INSERT OR IGNORE INTO Cititori (nume, parola, rol) VALUES ('biblio', '" + biblio_pass_hash + "', 'bibliotecar');").c_str(), nullptr, nullptr, nullptr);
}

Biblioteca::~Biblioteca() { 
    if (db != nullptr) sqlite3_close(db); 
}

// --- AUTENTIFICARE SECURIZATĂ ---
std::string Biblioteca::autentificaUtilizator(const std::string& nume, const std::string& parola) {
    std::string parola_hash = hashuiesteParola(parola);
    sqlite3_stmt* stmt;
    std::string rol = "";
    
    std::string q = "SELECT rol FROM Cititori WHERE nume = ? AND parola = ?;";
    if (sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, nume.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, parola_hash.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* text = (const char*)sqlite3_column_text(stmt, 0);
            if (text) rol = text;
        }
    }
    sqlite3_finalize(stmt);
    return rol;
}

// --- ÎNREGISTRARE SECURIZATĂ ---
bool Biblioteca::inregistreazaUtilizator(const std::string& nume, const std::string& email, const std::string& parola) {
    std::string parola_hash = hashuiesteParola(parola);
    std::string q = "INSERT INTO Cititori (nume, email, parola, rol) VALUES ('" + nume + "', '" + email + "', '" + parola_hash + "', 'user');";
    int res = sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
    return res == SQLITE_OK;
}

// --- GESTIUNE CĂRȚI ---
void Biblioteca::adaugaCarteFizica(const std::string& titlu, const std::string& autor, const std::string& isbn, const std::string& editura, const std::string& an, const std::string& locatie) {
    std::string q = "INSERT INTO Carti (titlu, autor, isbn, editura, an_publicare, locatie) VALUES ('" + 
                    titlu + "', '" + autor + "', '" + isbn + "', '" + editura + "', '" + an + "', '" + locatie + "');";
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

// --- GESTIUNE CITITORI & ÎMPRUMUTURI ---
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
    std::string q = "UPDATE Carti SET termen_limita = 'prelungit' WHERE id = " + std::to_string(id_carte) + " AND imprumutat_la != '';";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
    return sqlite3_changes(db) > 0;
}

bool Biblioteca::anuleazaRezervare(int id_carte) {
    std::string q = "UPDATE Carti SET rezervat_de = '' WHERE id = " + std::to_string(id_carte) + ";";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
    return sqlite3_changes(db) > 0;
}

std::vector<std::string> Biblioteca::getCartiImprumutate() {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT id, titlu, imprumutat_la FROM Carti WHERE imprumutat_la != '';", -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string t = (const char*)sqlite3_column_text(stmt, 1);
            std::string u = (const char*)sqlite3_column_text(stmt, 2);
            lista.push_back("ID " + std::to_string(id) + " | " + t + " -> la: " + u);
        }
    }
    sqlite3_finalize(stmt);
    return lista;
}

// --- AI GEMINI ---
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