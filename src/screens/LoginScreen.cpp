#include "screens/LoginScreen.h"
#include "Theme.h"

using namespace ftxui;

LoginScreen::LoginScreen(AppState& s) : state(s) {
    inp_user = Input(&state.inp_user_str, "Username");
    InputOption opt_pass; 
    opt_pass.password = true;
    inp_pass = Input(&state.inp_pass_str, "Password", opt_pass);
    
    btn_login_exec = MakeSmallBtn(state.limba_selectata, "Intra", "Enter", [&]{
    // Admin (index 4 în tab_comps)
    if(state.inp_user_str == "admin" && state.inp_pass_str == "admin123") { 
        state.ecran_activ = 4;  // <--- SCHIMBAT DIN 5 ÎN 4
        state.user_curent = "admin";
        state.rol_curent = UserRole::ADMIN;
    } 
    // Bibliotecar (index 3 în tab_comps)
    else if(state.inp_user_str == "bibliotecar" && state.inp_pass_str == "1") { 
        state.ecran_activ = 3;  // <--- SCHIMBAT DIN 4 ÎN 3
        state.user_curent = "bibliotecar";
        state.rol_curent = UserRole::LIBRARIAN;
    } 
    // User (index 2 în tab_comps)
    else if(!state.inp_user_str.empty() && !state.inp_pass_str.empty()) { 
        state.ecran_activ = 2;  // <--- SCHIMBAT DIN 3 ÎN 2
        state.user_curent = state.inp_user_str;
        state.rol_curent = UserRole::USER;
        state.biblio.adaugaCititor(state.user_curent);
    } 
    else {
        state.mesaj_eroare = state.L("Date Invalide!", "Invalid Credentials!");
    }
    state.inp_user_str = ""; 
    state.inp_pass_str = "";
    });
    
    btn_login_back = MakeSmallBtn(state.limba_selectata, "Inapoi", "Back", [&]{ 
        state.ecran_activ = 0; 
        state.mesaj_eroare = ""; 
    });

    container = Container::Vertical({inp_user, inp_pass, btn_login_exec, btn_login_back});
}

Element LoginScreen::Render() {
    Elements row1, row2, form_col;
    row1.push_back(state.elem_txt(" Utilizator: ", " Username:   ") | bold); 
    row1.push_back(inp_user->Render() | border | size(WIDTH, EQUAL, 30)); 
    
    row2.push_back(state.elem_txt(" Parola:     ", " Password:   ") | bold); 
    row2.push_back(inp_pass->Render() | border | size(WIDTH, EQUAL, 30));
    
    form_col.push_back(hbox(row1) | center);
    form_col.push_back(hbox(row2) | center);
    form_col.push_back(separatorEmpty());
    form_col.push_back(btn_login_exec->Render() | color(Color::Green));
    form_col.push_back(btn_login_back->Render() | dim);
    form_col.push_back(text(state.mesaj_eroare) | color(Color::Red) | bold | center);
    
    // Adaugă instrucțiuni
    form_col.push_back(separatorEmpty());
    form_col.push_back(text(state.L("--- Credentiale ---", "--- Credentials ---")) | dim);
    form_col.push_back(text(state.L("stefan/stefan = User", "stefan/stefan = User")) | dim);
    form_col.push_back(text(state.L("bibliotecar/1 = Bibliotecar", "bibliotecar/1 = Librarian")) | dim);
    form_col.push_back(text(state.L("admin/admin123 = Admin", "admin/admin123 = Admin")) | dim);
    
    auto login_card = window(
        state.elem_txt(" INTRA IN CONT ", " LOG IN ") | bold | center, 
        vbox(form_col)
    );
    
    Elements main_col;
    main_col.push_back(filler());
    main_col.push_back(login_card | center);
    main_col.push_back(filler());
    return vbox(main_col) | flex;
}