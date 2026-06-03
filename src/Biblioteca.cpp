#include "Biblioteca.h"
#include <iostream>
#include <fstream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

std::string hashuiesteParola(const std::string& parola) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)parola.c_str(), parola.length(), hash);
    std::ostringstream oss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return oss.str();
}

void Biblioteca::initializeazaTabele() {
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Carti (id INTEGER PRIMARY KEY AUTOINCREMENT, titlu TEXT, autor TEXT, isbn TEXT, editura TEXT, an_publicare TEXT, locatie TEXT, stoc INTEGER DEFAULT 0);", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Cititori (id INTEGER PRIMARY KEY AUTOINCREMENT, nume TEXT UNIQUE, email TEXT UNIQUE, parola TEXT, rol TEXT DEFAULT 'user');", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "ALTER TABLE Cititori ADD COLUMN email TEXT;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "ALTER TABLE Cititori ADD COLUMN parola TEXT;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "ALTER TABLE Cititori ADD COLUMN rol TEXT DEFAULT 'user';", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Imprumuturi (id INTEGER PRIMARY KEY AUTOINCREMENT, carte_id INTEGER, cititor_nume TEXT, data_imprumut TEXT DEFAULT (date('now')), data_returnare TEXT DEFAULT '', FOREIGN KEY(carte_id) REFERENCES Carti(id));", nullptr, nullptr, nullptr);

    std::string admin_hash = hashuiesteParola("admin123");
    sqlite3_exec(db, ("INSERT OR IGNORE INTO Cititori (nume, parola, rol) VALUES ('admin', '" + admin_hash + "', 'admin');").c_str(), nullptr, nullptr, nullptr);
    std::string biblio_hash = hashuiesteParola("1234");
    sqlite3_exec(db, ("INSERT OR IGNORE INTO Cititori (nume, parola, rol) VALUES ('biblio', '" + biblio_hash + "', 'bibliotecar');").c_str(), nullptr, nullptr, nullptr);
}

Biblioteca::Biblioteca() {
    if (sqlite3_open("biblioteca.db", &db) != SQLITE_OK) { std::cerr << "EROARE DB!" << std::endl; exit(1); }
    initializeazaTabele();
}
Biblioteca::~Biblioteca() { if (db != nullptr) sqlite3_close(db); }

void Biblioteca::adaugaCarte(const std::string& titlu, const std::string& autor, const std::string& isbn, const std::string& editura, const std::string& an, const std::string& locatie, int stoc) {
    std::string q = "INSERT INTO Carti (titlu, autor, isbn, editura, an_publicare, locatie, stoc) VALUES ('" + titlu + "', '" + autor + "', '" + isbn + "', '" + editura + "', '" + an + "', '" + locatie + "', " + std::to_string(stoc) + ");";
    sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr);
}

void Biblioteca::stergeCarte(int id) { sqlite3_exec(db, ("DELETE FROM Carti WHERE id = " + std::to_string(id) + ";").c_str(), nullptr, nullptr, nullptr); }

std::vector<std::string> Biblioteca::getToateCartile() {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT id, titlu, autor, stoc, locatie FROM Carti ORDER BY locatie, titlu;", -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string t = (const char*)sqlite3_column_text(stmt, 1);
            std::string a = (const char*)sqlite3_column_text(stmt, 2);
            int stoc = sqlite3_column_int(stmt, 3);
            std::string loc = (const char*)sqlite3_column_text(stmt, 4);
            lista.push_back("ID " + std::to_string(id) + " | [" + loc + "] " + t + " - " + a + " | Stoc: " + std::to_string(stoc));
        }
    }
    sqlite3_finalize(stmt);
    return lista;
}

std::vector<std::string> Biblioteca::getCartiDisponibile() {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT id, titlu, autor, stoc, locatie FROM Carti WHERE stoc > 0 ORDER BY locatie, titlu;", -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string t = (const char*)sqlite3_column_text(stmt, 1);
            std::string a = (const char*)sqlite3_column_text(stmt, 2);
            int stoc = sqlite3_column_int(stmt, 3);
            std::string loc = (const char*)sqlite3_column_text(stmt, 4);
            lista.push_back("ID " + std::to_string(id) + " | [" + loc + "] " + t + " - " + a + " | Stoc: " + std::to_string(stoc));
        }
    }
    sqlite3_finalize(stmt);
    return lista;
}

std::vector<std::string> Biblioteca::getCartiImprumutateDeUser(const std::string& nume) {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    std::string q = "SELECT c.id, c.titlu, c.autor, i.data_imprumut FROM Imprumuturi i JOIN Carti c ON i.carte_id = c.id WHERE i.cititor_nume = ? AND i.data_returnare = '';";
    if (sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, nume.c_str(), -1, SQLITE_STATIC);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string t = (const char*)sqlite3_column_text(stmt, 1);
            std::string a = (const char*)sqlite3_column_text(stmt, 2);
            std::string data = (const char*)sqlite3_column_text(stmt, 3);
            lista.push_back("ID " + std::to_string(id) + " | " + t + " de " + a + " (Din: " + data + ")");
        }
    }
    sqlite3_finalize(stmt);
    return lista;
}

std::vector<std::string> Biblioteca::getToateImprumuturile() {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    std::string q = "SELECT i.id, c.titlu, i.cititor_nume, i.data_imprumut FROM Imprumuturi i JOIN Carti c ON i.carte_id = c.id WHERE i.data_returnare = '';";
    if (sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string t = (const char*)sqlite3_column_text(stmt, 1);
            std::string u = (const char*)sqlite3_column_text(stmt, 2);
            std::string data = (const char*)sqlite3_column_text(stmt, 3);
            lista.push_back("ID Imprumut #" + std::to_string(id) + " | " + t + " -> " + u + " (Din: " + data + ")");
        }
    }
    sqlite3_finalize(stmt);
    return lista;
}

std::vector<std::string> Biblioteca::cautaCarti(const std::string& query) {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    std::string search = "%" + query + "%";
    if (sqlite3_prepare_v2(db, "SELECT id, titlu, autor, stoc, locatie FROM Carti WHERE titlu LIKE ? OR autor LIKE ?;", -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, search.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, search.c_str(), -1, SQLITE_STATIC);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string t = (const char*)sqlite3_column_text(stmt, 1);
            std::string a = (const char*)sqlite3_column_text(stmt, 2);
            int stoc = sqlite3_column_int(stmt, 3);
            std::string loc = (const char*)sqlite3_column_text(stmt, 4);
            lista.push_back("ID " + std::to_string(id) + " | [" + loc + "] " + t + " - " + a + " | Stoc: " + std::to_string(stoc));
        }
    }
    sqlite3_finalize(stmt);
    return lista;
}

void Biblioteca::adaugaCititor(const std::string& nume) { sqlite3_exec(db, ("INSERT OR IGNORE INTO Cititori (nume) VALUES ('" + nume + "');").c_str(), nullptr, nullptr, nullptr); }

std::vector<std::string> Biblioteca::getCititori() {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT nume FROM Cititori;", -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) lista.push_back((const char*)sqlite3_column_text(stmt, 0));
    }
    sqlite3_finalize(stmt);
    return lista;
}

bool Biblioteca::inregistreazaUtilizator(const std::string& nume, const std::string& email, const std::string& parola) {
    std::string hash = hashuiesteParola(parola);
    std::string q = "INSERT INTO Cititori (nume, email, parola, rol) VALUES ('" + nume + "', '" + email + "', '" + hash + "', 'user');";
    return sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
}

std::string Biblioteca::autentificaUtilizator(const std::string& nume, const std::string& parola) {
    std::string hash = hashuiesteParola(parola);
    sqlite3_stmt* stmt;
    std::string rol = "";
    if (sqlite3_prepare_v2(db, "SELECT rol FROM Cititori WHERE nume = ? AND parola = ?;", -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, nume.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, hash.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) { const char* text = (const char*)sqlite3_column_text(stmt, 0); if (text) rol = text; }
    }
    sqlite3_finalize(stmt);
    return rol;
}

bool Biblioteca::imprumutaCarte(int id_carte, const std::string& nume) {
    sqlite3_stmt* stmt;
    int stoc_curent = 0;
    if (sqlite3_prepare_v2(db, "SELECT stoc FROM Carti WHERE id = ?;", -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id_carte);
        if (sqlite3_step(stmt) == SQLITE_ROW) stoc_curent = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    if (stoc_curent <= 0) return false;

    sqlite3_exec(db, ("UPDATE Carti SET stoc = stoc - 1 WHERE id = " + std::to_string(id_carte) + ";").c_str(), nullptr, nullptr, nullptr);
    std::string q = "INSERT INTO Imprumuturi (carte_id, cititor_nume, data_imprumut) VALUES (" + std::to_string(id_carte) + ", '" + nume + "', date('now'));";
    return sqlite3_exec(db, q.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
}

bool Biblioteca::returneazaCarte(int id_carte, const std::string& nume) {
    sqlite3_stmt* stmt;
    int imprumut_id = -1;
    std::string q = "SELECT id FROM Imprumuturi WHERE carte_id = ? AND cititor_nume = ? AND data_returnare = '' LIMIT 1;";
    if (sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id_carte);
        sqlite3_bind_text(stmt, 2, nume.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) imprumut_id = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    if (imprumut_id == -1) return false;

    sqlite3_exec(db, ("UPDATE Imprumuturi SET data_returnare = date('now') WHERE id = " + std::to_string(imprumut_id) + ";").c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, ("UPDATE Carti SET stoc = stoc + 1 WHERE id = " + std::to_string(id_carte) + ";").c_str(), nullptr, nullptr, nullptr);
    return true;
}

bool Biblioteca::returneazaFortat(int id_carte) {
    sqlite3_stmt* stmt;
    int imprumut_id = -1;
    std::string q = "SELECT id, cititor_nume FROM Imprumuturi WHERE carte_id = ? AND data_returnare = '' LIMIT 1;";
    if (sqlite3_prepare_v2(db, q.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id_carte);
        if (sqlite3_step(stmt) == SQLITE_ROW) imprumut_id = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    if (imprumut_id == -1) return false;

    sqlite3_exec(db, ("UPDATE Imprumuturi SET data_returnare = date('now') WHERE id = " + std::to_string(imprumut_id) + ";").c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, ("UPDATE Carti SET stoc = stoc + 1 WHERE id = " + std::to_string(id_carte) + ";").c_str(), nullptr, nullptr, nullptr);
    return true;
}

bool Biblioteca::rezervaCarte(int id_carte, const std::string& nume) { return false; }
bool Biblioteca::prelungesteImprumut(int id_carte) { return true; }

std::vector<std::string> Biblioteca::iaTendinteOnline() {
    std::vector<std::string> rezultate;
    std::string url = "https://openlibrary.org/search.json?q=bestseller&limit=5&sort=rating"; 
    auto r = cpr::Get(cpr::Url{url}, cpr::Timeout{3000});
    if (r.status_code == 200) {
        try {
            auto j = nlohmann::json::parse(r.text);
            if (j.contains("docs")) {
                for (auto& carte : j["docs"]) {
                    std::string titlu = carte.value("title", "Titlu necunoscut");
                    std::string autor = "Autor necunoscut";
                    if (carte.contains("author_name") && !carte["author_name"].empty()) autor = carte["author_name"][0].get<std::string>();
                    rezultate.push_back(titlu + " - " + autor);
                }
            }
        } catch (...) { rezultate.push_back("Eroare JSON."); }
    } else { rezultate.push_back("Eroare conexiune."); }
    return rezultate;
}

std::vector<std::string> Biblioteca::getCartiPentruUser() {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT id, titlu, autor, locatie FROM Carti ORDER BY locatie, titlu;", -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string t = (const char*)sqlite3_column_text(stmt, 1);
            std::string a = (const char*)sqlite3_column_text(stmt, 2);
            std::string loc = (const char*)sqlite3_column_text(stmt, 3);
            lista.push_back("ID " + std::to_string(id) + " | [" + loc + "] " + t + " - " + a);
        }
    }
    sqlite3_finalize(stmt);
    return lista;
}

std::vector<std::string> Biblioteca::getCartiDisponibilePentruUser() {
    std::vector<std::string> lista;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT id, titlu, autor, locatie FROM Carti WHERE stoc > 0 ORDER BY locatie, titlu;", -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string t = (const char*)sqlite3_column_text(stmt, 1);
            std::string a = (const char*)sqlite3_column_text(stmt, 2);
            std::string loc = (const char*)sqlite3_column_text(stmt, 3);
            lista.push_back("ID " + std::to_string(id) + " | [" + loc + "] " + t + " - " + a + " [DISPONIBIL]");
        }
    }
    sqlite3_finalize(stmt);
    return lista;
}

std::string Biblioteca::intreabaBiblioAI(const std::string& prompt) {
    std::string apiKey = "";
    std::ifstream envFile(".env");
    if (envFile.is_open()) {
        std::string linie;
        while (std::getline(envFile, linie)) {
            if (linie.find("GEMINI_API_KEY=") == 0) { apiKey = linie.substr(15); break; }
        }
        envFile.close();
    }
    if (apiKey.empty()) return "EROARE: Fisierul .env nu a fost gasit!";
    std::string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + apiKey;
    json body = {{"contents", json::array({{{"parts", json::array({{{"text", "Esti BiblioAI, un bibliotecar robot politicos. Raspunde scurt: " + prompt}}})}}})}};
    auto r = cpr::Post(cpr::Url{url}, cpr::Header{{"Content-Type", "application/json"}}, cpr::Body{body.dump()});
    if (r.status_code == 200) { try { return json::parse(r.text)["candidates"][0]["content"]["parts"][0]["text"]; } catch (...) { return "Eroare AI."; } }
    return "Eroare de retea.";
}