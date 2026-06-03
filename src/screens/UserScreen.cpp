#include "screens/UserScreen.h"
#include "Theme.h"

using namespace ftxui;

UserScreen::UserScreen(AppState& s) : state(s) {
    inp_id = Input(&state.id_carte_str, "ID Carte");
    
    btn_imprumuta = MakeSmallBtn(state.limba_selectata, "Imprumuta", "Borrow", [&]{
        try {
            if(!state.id_carte_str.empty()) {
                if(state.biblio.imprumutaCarte(std::stoi(state.id_carte_str), state.user_curent)) {
                    state.mesaj_actiune = state.L("Carte imprumutata!", "Book borrowed!");
                } else {
                    state.mesaj_actiune = state.L("Carte indisponibila!", "Book unavailable!");
                }
                state.id_carte_str = "";
            }
        } catch(...) { 
            state.mesaj_actiune = state.L("ID invalid!", "Invalid ID!"); 
        }
    });

    btn_returneaza = MakeSmallBtn(state.limba_selectata, "Returneaza", "Return", [&]{
        try {
            if(!state.id_carte_str.empty()) {
                if(state.biblio.returneazaCarte(std::stoi(state.id_carte_str))) {
                    state.mesaj_actiune = state.L("Carte returnata!", "Book returned!");
                } else {
                    state.mesaj_actiune = state.L("Eroare la returnare!", "Return error!");
                }
                state.id_carte_str = "";
            }
        } catch(...) { 
            state.mesaj_actiune = state.L("ID invalid!", "Invalid ID!"); 
        }
    });
    
    btn_back = MakeSmallBtn(state.limba_selectata, "Logout", "Logout", [&]{ 
    state.ecran_activ = 0; 
    state.user_curent = "";
    state.mesaj_actiune = "";
    });

    container = Container::Vertical({inp_id, btn_imprumuta, btn_returneaza, btn_back});
}

Element UserScreen::Render() {
    Elements u_col, lista_c;
    for(auto& c : state.biblio.getCartiFormatate()) {
        lista_c.push_back(text(" 📖 " + c));
    }
    
    u_col.push_back(hbox({
        state.elem_txt("Bine ai venit, ", "Welcome, ") | bold | color(Color::Cyan), 
        text(state.user_curent) | bold
    }));
    
    u_col.push_back(separator());
    u_col.push_back(vbox(lista_c) | border | size(HEIGHT, LESS_THAN, 8) | flex);
    u_col.push_back(hbox({
        inp_id->Render() | size(WIDTH, EQUAL, 15), 
        btn_imprumuta->Render(), 
        btn_returneaza->Render()
    }));
    u_col.push_back(text(state.mesaj_actiune) | color(Color::Yellow) | center);
    u_col.push_back(btn_back->Render() | center);
    
    return window(
        state.elem_txt(" PANOU CITITOR ", " READER PANEL "), 
        vbox(u_col)
    ) | flex;
}