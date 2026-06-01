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

    void adaugaCarteFizica(const std::string& titlu, const std::string& autor, const std::string& sursa, const std::string& locatie);
    std::vector<std::unique_ptr<Carte>> getToateCartile();
};