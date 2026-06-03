#include "screens/BiblioScreen.h"
#include "Theme.h"
using namespace ftxui;

BiblioScreen::BiblioScreen(AppState& s) : state(s) {
    inp_titlu = Input(&state.titlu_nou, "Titlu Carte");
    inp_autor = Input(&state.autor_nou, "Autor");
    inp_locatie = Input(&state.locatie_nou, "Locatie");
    
    btn_add = MakeSmallBtn(state.limba_selectata, "Adauga", "Add", [&]{
        if(!state.titlu_nou.empty()) {
            state.biblio.adaugaCarteFizica(state.titlu_nou, state.autor_nou, "Noua", state.locatie_nou);
            state.mesaj_actiune = "Adaugat!";
            state.titlu_nou = ""; state.autor_nou = ""; state.locatie_nou = "";
        }
    });
    
    btn_back = MakeSmallBtn(state.limba_selectata, "Logout", "Logout", [&]{ 
    state.ecran_activ = 0; 
    state.user_curent = "";
    state.mesaj_actiune = "";
    });

    container = Container::Vertical({inp_titlu, inp_autor, inp_locatie, btn_add, btn_back});
}

Element BiblioScreen::Render() {
    Elements b_col;
    b_col.push_back(text("Panou Bibliotecar") | bold);
    b_col.push_back(inp_titlu->Render() | border);
    b_col.push_back(inp_autor->Render() | border);
    b_col.push_back(inp_locatie->Render() | border);
    b_col.push_back(btn_add->Render());
    b_col.push_back(text(state.mesaj_actiune) | color(Color::Yellow));
    b_col.push_back(btn_back->Render());
    return window(text("BIBLIOTECAR"), vbox(b_col));
}