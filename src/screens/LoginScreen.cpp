#include "screens/LoginScreen.h"
#include "Theme.h"
#include <vector>
using namespace ftxui;

LoginScreen::LoginScreen(AppState& s) : state(s) {
    inp_user = Input(&state.inp_user_str, "Username");
    InputOption opt_pass; opt_pass.password = true;
    inp_pass = Input(&state.inp_pass_str, "Password", opt_pass);
    
    btn_login_exec = MakeSmallBtn(state.limba_selectata, "Intra", "Enter", [&]{
        // Verificăm în baza de date
        std::string rol = state.biblio.autentificaUtilizator(state.inp_user_str, state.inp_pass_str);
        
        // INDICII CORECTE:
        if(rol == "admin") { 
            state.ecran_activ = 6; // 6 = Admin
            state.user_curent = state.inp_user_str; 
            state.rol_curent = UserRole::ADMIN;
        } else if(rol == "bibliotecar") { 
            state.ecran_activ = 5; // 5 = Bibliotecar
            state.user_curent = state.inp_user_str; 
            state.rol_curent = UserRole::LIBRARIAN;
        } else if(rol == "user") { 
            state.ecran_activ = 4; // 4 = User (Cititor)
            state.user_curent = state.inp_user_str; 
            state.rol_curent = UserRole::USER;
        } else {
            state.mesaj_eroare = "Username sau parola gresita!";
        }
        state.inp_user_str = ""; 
        state.inp_pass_str = "";
    });
    
    btn_login_back = MakeSmallBtn(state.limba_selectata, "Inapoi", "Back", [&]{ 
        state.ecran_activ = 0; 
        state.mesaj_eroare = ""; 
    });

    std::vector<Component> comps;
    comps.push_back(inp_user); comps.push_back(inp_pass); comps.push_back(btn_login_exec); comps.push_back(btn_login_back);
    container = Container::Vertical(comps);
}

Element LoginScreen::Render() {
    Elements row1, row2, form_col;
    row1.push_back(text(" Username:   ") | bold); row1.push_back(inp_user->Render() | border | size(WIDTH, EQUAL, 30)); 
    row2.push_back(text(" Password:   ") | bold); row2.push_back(inp_pass->Render() | border | size(WIDTH, EQUAL, 30));
    
    form_col.push_back(hbox(row1) | center);
    form_col.push_back(hbox(row2) | center);
    form_col.push_back(separatorEmpty());
    form_col.push_back(btn_login_exec->Render() | color(Color::Green) | center);
    form_col.push_back(btn_login_back->Render() | dim | center);
    form_col.push_back(text(state.mesaj_eroare) | color(Color::Red) | bold | center);

    auto login_card = window(text(" INTRA IN CONT ") | bold | center, vbox(form_col));
    Elements main_col;
    main_col.push_back(filler()); main_col.push_back(login_card | center); main_col.push_back(filler());
    return vbox(main_col) | flex;
}