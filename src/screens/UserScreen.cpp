#include "screens/UserScreen.h"
#include "Theme.h"
#include <vector>
using namespace ftxui;

UserScreen::UserScreen(AppState& s) : state(s) {
    inp_id = Input(&state.id_carte_str, "ID Carte");
    inp_cautare = Input(&state.query_cautare, "Cauta dupa titlu/autor...");
    
    btn_cauta = MakeSmallBtn(state.limba_selectata, "Cauta", "Search", [&]{
        state.lista_cautare = state.biblio.cautaCarti(state.query_cautare);
        state.afiseaza_rezultate_cautare = true;
    });

    btn_imprumuta = MakeSmallBtn(state.limba_selectata, "Imprumuta", "Borrow", [&]{
        if(!state.id_carte_str.empty()) {
            if(state.biblio.imprumutaCarte(std::stoi(state.id_carte_str), state.user_curent)) 
                state.mesaj_actiune = "Imprumutat!";
            else 
                state.mesaj_actiune = "Eroare/Indisponibil!";
            state.id_carte_str = "";
        }
    });

    btn_returneaza = MakeSmallBtn(state.limba_selectata, "Returneaza", "Return", [&]{
        if(!state.id_carte_str.empty()) {
            if(state.biblio.returneazaCarte(std::stoi(state.id_carte_str))) 
                state.mesaj_actiune = "Returnat!";
            else 
                state.mesaj_actiune = "Eroare!";
            state.id_carte_str = "";
        }
    });

    btn_rezerva = MakeSmallBtn(state.limba_selectata, "Rezerva", "Reserve", [&]{
        if(!state.id_carte_str.empty()) {
            if(state.biblio.rezervaCarte(std::stoi(state.id_carte_str), state.user_curent)) 
                state.mesaj_actiune = "Rezervat!";
            else 
                state.mesaj_actiune = "Eroare la rezervare!";
            state.id_carte_str = "";
        }
    });

    btn_prelungeste = MakeSmallBtn(state.limba_selectata, "Prelungeste", "Extend", [&]{
        if(!state.id_carte_str.empty()) {
            if(state.biblio.prelungesteImprumut(std::stoi(state.id_carte_str))) 
                state.mesaj_actiune = "Termen prelungit!";
            else 
                state.mesaj_actiune = "Eroare!";
            state.id_carte_str = "";
        }
    });
    
    btn_back = MakeSmallBtn(state.limba_selectata, "Logout", "Logout", [&]{ 
        state.ecran_activ = 0; 
        state.user_curent = ""; 
        state.mesaj_actiune = ""; 
    });

    // Folosim std::vector explicit
    std::vector<Component> comps;
    comps.push_back(inp_cautare);
    comps.push_back(btn_cauta);
    comps.push_back(inp_id);
    comps.push_back(btn_imprumuta);
    comps.push_back(btn_returneaza);
    comps.push_back(btn_rezerva);
    comps.push_back(btn_prelungeste);
    comps.push_back(btn_back);
    
    container = Container::Vertical(comps);
}

Element UserScreen::Render() {
    std::vector<std::string> lista_afisata = state.afiseaza_rezultate_cautare ? 
        state.lista_cautare : state.biblio.getCartiFormatate();
    
    Elements u_col, lista_c;
    for(auto& c : lista_afisata) lista_c.push_back(text(" 📖 " + c));
    
    u_col.push_back(hbox({
        state.elem_txt("Bine ai venit, ", "Welcome, ") | bold | color(Color::Cyan), 
        text(state.user_curent) | bold
    }));
    u_col.push_back(separator());
    
    u_col.push_back(text("Cautare carti:") | bold);
    
    Elements cautare_row;
    cautare_row.push_back(inp_cautare->Render() | flex);
    cautare_row.push_back(btn_cauta->Render());
    u_col.push_back(hbox(cautare_row));
    
    u_col.push_back(vbox(lista_c) | border | size(HEIGHT, LESS_THAN, 8) | flex);
    
    u_col.push_back(text("Actiuni (introdu ID):") | bold);
    u_col.push_back(inp_id->Render() | size(WIDTH, EQUAL, 15));
    
    Elements butoane_actiune;
    butoane_actiune.push_back(btn_imprumuta->Render());
    butoane_actiune.push_back(btn_returneaza->Render());
    butoane_actiune.push_back(btn_rezerva->Render());
    butoane_actiune.push_back(btn_prelungeste->Render());
    u_col.push_back(hbox(butoane_actiune));
    
    u_col.push_back(text(state.mesaj_actiune) | color(Color::Yellow) | center);
    u_col.push_back(btn_back->Render() | center);
    
    return window(state.elem_txt(" PANOU CITITOR ", " READER PANEL "), vbox(u_col)) | flex;
}