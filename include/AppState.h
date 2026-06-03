#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <ftxui/dom/elements.hpp>
#include "Biblioteca.h"

enum class UserRole {
    GUEST,
    USER,
    LIBRARIAN,
    ADMIN
};

struct AppState {
    int ecran_activ = 0;
    std::string user_curent = "";
    UserRole rol_curent = UserRole::GUEST;
    std::string mesaj_actiune = "";
    int limba_selectata = 0;
    std::vector<std::string> optiuni_limba = {"RO", "EN"};
    Biblioteca& biblio;

    // Login
    std::string inp_user_str = "";
    std::string inp_pass_str = "";
    std::string mesaj_eroare = "";
    
    // Register
    std::string reg_user = "";
    std::string reg_pass = "";
    std::string mesaj_reg = "";
    
    // User/Cititor
    std::string id_carte_str = "";
    std::string query_cautare = "";           // <--- ADĂUGAT
    std::vector<std::string> lista_cautare;   // <--- ADĂUGAT
    bool afiseaza_rezultate_cautare = false;  // <--- ADĂUGAT
    
    // Bibliotecar
    std::string titlu_nou = "";
    std::string autor_nou = "";
    std::string locatie_nou = "";
    
    // Admin
    std::string cititor_nou = "";

    // AI
    bool mod_ai_activ = false;
    std::string ai_prompt = "";
    std::string ai_raspuns = "";
    std::string ai_raspuns_afisat = "";
    bool ai_scrie = false;
    size_t ai_index_curent = 0;
    std::chrono::steady_clock::time_point ai_timp_ultimul_cadru;

    AppState(Biblioteca& b) : biblio(b) {}

    std::string L(const std::string& ro, const std::string& en) {
        return (limba_selectata == 1) ? en : ro;
    }
    
    ftxui::Element elem_txt(const std::string& ro, const std::string& en) {
        return ftxui::text(L(ro, en));
    }
};