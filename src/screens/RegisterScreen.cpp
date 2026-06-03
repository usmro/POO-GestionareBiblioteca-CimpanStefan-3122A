#include "screens/RegisterScreen.h"
#include "Theme.h"
#include <vector>
using namespace ftxui;

RegisterScreen::RegisterScreen(AppState& s) : state(s) {
    inp_user = Input(&state.reg_user, "Username");
    inp_email = Input(&state.reg_email, "Email");
    
    InputOption opt_pass; opt_pass.password = true;
    inp_pass = Input(&state.reg_pass, "Parola", opt_pass);
    inp_confirm = Input(&state.reg_pass_confirm, "Confirma Parola", opt_pass);
    // AM SCOS inp_cod

    btn_exec = MakeSmallBtn(state.limba_selectata, "Inregistrare", "Sign Up", [&]{
        state.mesaj_reg = "";
        if(state.reg_user.empty() || state.reg_email.empty() || state.reg_pass.empty()) {
            state.mesaj_reg = "Completeaza toate campurile!"; return;
        }
        if(state.reg_pass != state.reg_pass_confirm) {
            state.mesaj_reg = "Parolele nu se potrivesc!"; return;
        }

        // Acum înregistrăm doar ca 'user'
        if(state.biblio.inregistreazaUtilizator(state.reg_user, state.reg_email, state.reg_pass)) {
            state.mesaj_reg = "Cont creat cu succes! (Rol: User)";
            state.reg_user = ""; state.reg_email = ""; state.reg_pass = ""; state.reg_pass_confirm = "";
        } else {
            state.mesaj_reg = "Username sau Email deja existent!";
        }
    });

    btn_back = MakeSmallBtn(state.limba_selectata, "Inapoi", "Back", [&]{ state.ecran_activ = 0; state.mesaj_reg = ""; });

    std::vector<Component> comps;
    comps.push_back(inp_user); comps.push_back(inp_email); comps.push_back(inp_pass);
    comps.push_back(inp_confirm); 
    comps.push_back(btn_exec); comps.push_back(btn_back);
    container = Container::Vertical(comps);
}

Element RegisterScreen::Render() {
    Elements row1, row2, row3, row4, form_col;
    
    row1.push_back(text(" Username:   ") | bold); row1.push_back(inp_user->Render() | border | size(WIDTH, EQUAL, 30));
    row2.push_back(text(" Email:      ") | bold); row2.push_back(inp_email->Render() | border | size(WIDTH, EQUAL, 30));
    row3.push_back(text(" Parola:     ") | bold); row3.push_back(inp_pass->Render() | border | size(WIDTH, EQUAL, 30));
    row4.push_back(text(" Confirma:   ") | bold); row4.push_back(inp_confirm->Render() | border | size(WIDTH, EQUAL, 30));
    // AM SCOS row5 pentru Cod Acces

    form_col.push_back(hbox(row1) | center);
    form_col.push_back(hbox(row2) | center);
    form_col.push_back(hbox(row3) | center);
    form_col.push_back(hbox(row4) | center);
    form_col.push_back(separatorEmpty());
    form_col.push_back(btn_exec->Render() | color(Color::Green) | center);
    form_col.push_back(btn_back->Render() | dim | center);
    form_col.push_back(text(state.mesaj_reg) | color(Color::Yellow) | bold | center);
    // AM SCOS textul cu BIBLIO2026

    auto reg_card = window(text(" CREARE CONT NOU ") | bold | center, vbox(form_col));
    Elements main_col;
    main_col.push_back(filler()); main_col.push_back(reg_card | center); main_col.push_back(filler());
    return vbox(main_col) | flex;
}