#include "screens/BiblioScreen.h"
#include "Theme.h"
#include <vector>
using namespace ftxui;

BiblioScreen::BiblioScreen(AppState& s) : state(s) {
    inp_titlu = Input(&state.titlu_nou, "Titlu Carte");
    inp_autor = Input(&state.autor_nou, "Autor");
    inp_locatie = Input(&state.locatie_nou, "Locatie");
    inp_id_sterge = Input(&state.id_carte_str, "ID Carte de sters");
    
    btn_add = MakeSmallBtn(state.limba_selectata, "Adauga Carte", "Add Book", [&]{
        if(!state.titlu_nou.empty()) {
            state.biblio.adaugaCarteFizica(state.titlu_nou, state.autor_nou, "Noua", state.locatie_nou);
            state.mesaj_actiune = "Carte adaugata in inventar!";
            state.titlu_nou = ""; state.autor_nou = ""; state.locatie_nou = "";
        }
    });
    
    btn_sterge = MakeSmallBtn(state.limba_selectata, "Sterge Carte", "Delete Book", [&]{
        if(!state.id_carte_str.empty()) {
            state.biblio.stergeCarte(std::stoi(state.id_carte_str));
            state.mesaj_actiune = "Carte stersa din inventar!";
            state.id_carte_str = "";
        }
    });
    
    btn_back = MakeSmallBtn(state.limba_selectata, "Logout", "Logout", [&]{ 
        state.ecran_activ = 0; state.user_curent = ""; state.mesaj_actiune = "";
    });

    // Folosim std::vector explicit pentru Container::Vertical
    std::vector<Component> comps;
    comps.push_back(inp_titlu);
    comps.push_back(inp_autor);
    comps.push_back(inp_locatie);
    comps.push_back(btn_add);
    comps.push_back(inp_id_sterge);
    comps.push_back(btn_sterge);
    comps.push_back(btn_back);
    
    container = Container::Vertical(comps);
}

Element BiblioScreen::Render() {
    Elements lista_inv;
    for(auto& c : state.biblio.getCartiFormatate()) lista_inv.push_back(text(" 📚 " + c));

    Elements b_col;
    b_col.push_back(state.elem_txt("Panou Bibliotecar - Inventar", "Librarian Panel - Inventory") | bold | color(Color::Green));
    b_col.push_back(separator());
    
    b_col.push_back(text("Adaugare carte noua:") | bold);
    b_col.push_back(inp_titlu->Render() | border);
    b_col.push_back(inp_autor->Render() | border);
    b_col.push_back(inp_locatie->Render() | border);
    b_col.push_back(btn_add->Render() | center);
    
    b_col.push_back(separatorEmpty());
    b_col.push_back(text("Inventar existent:") | bold);
    b_col.push_back(vbox(lista_inv) | border | size(HEIGHT, LESS_THAN, 6) | flex);
    
    b_col.push_back(text("Stergere carte (dupa ID):") | bold);
    
    // Folosim Elements explicit pentru hbox
    Elements sterge_row;
    sterge_row.push_back(inp_id_sterge->Render() | size(WIDTH, EQUAL, 15));
    sterge_row.push_back(btn_sterge->Render());
    b_col.push_back(hbox(sterge_row));
    
    b_col.push_back(text(state.mesaj_actiune) | color(Color::Yellow) | center);
    b_col.push_back(btn_back->Render() | center);
    
    return window(state.elem_txt(" MANAGEMENT CARTI ", " BOOK MANAGEMENT "), vbox(b_col)) | flex;
}