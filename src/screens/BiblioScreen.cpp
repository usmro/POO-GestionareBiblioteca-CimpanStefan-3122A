#include "screens/BiblioScreen.h"
#include "Theme.h"
#include <vector>
using namespace ftxui;

BiblioScreen::BiblioScreen(AppState& s) : state(s) {
    // --- TAB 1: ADĂUGARE CARTE ---
    inp_titlu = Input(&state.titlu_nou, "Titlu");
    inp_autor = Input(&state.autor_nou, "Autor");
    inp_isbn = Input(&state.carte_isbn, "ISBN / ISSN");
    inp_editura = Input(&state.carte_editura, "Editura");
    inp_an = Input(&state.carte_an, "An Aparitie");
    inp_locatie = Input(&state.locatie_nou, "Locatie (ex: Raion A1)");
    
    btn_add = MakeSmallBtn(state.limba_selectata, "Adauga in Inventar", "Add to Inventory", [&]{
        if(!state.titlu_nou.empty()) {
            state.biblio.adaugaCarteFizica(state.titlu_nou, state.autor_nou, state.carte_isbn, state.carte_editura, state.carte_an, state.locatie_nou);
            state.mesaj_actiune = "Carte adaugata cu succes!";
            state.titlu_nou = ""; state.autor_nou = ""; state.carte_isbn = ""; state.carte_editura = ""; state.carte_an = ""; state.locatie_nou = "";
        } else { state.mesaj_actiune = "Titlul este obligatoriu!"; }
    });

    std::vector<Component> t1;
    t1.push_back(inp_titlu); t1.push_back(inp_autor); t1.push_back(inp_isbn);
    t1.push_back(inp_editura); t1.push_back(inp_an); t1.push_back(inp_locatie);
    t1.push_back(btn_add);
    tab1_container = Container::Vertical(t1);

    // --- TAB 2: INVENTAR ---
    btn_back_inv = MakeSmallBtn(state.limba_selectata, "Refresh Lista", "Refresh List", [&]{ state.mesaj_actiune = "Lista actualizata!"; });
    std::vector<Component> t2;
    t2.push_back(btn_back_inv);
    tab2_container = Container::Vertical(t2);

    // --- TAB 3: GESTIUNE ÎMPRUMUTURI ---
    inp_id_return = Input(&state.id_carte_str, "ID Carte");
    btn_force_return = MakeSmallBtn(state.limba_selectata, "Fortare Returnare", "Force Return", [&]{
        if(!state.id_carte_str.empty()) {
            state.biblio.returneazaCarte(std::stoi(state.id_carte_str));
            state.mesaj_actiune = "Carte returnata fortat!"; state.id_carte_str = "";
        }
    });
    btn_extend = MakeSmallBtn(state.limba_selectata, "Prelungeste Termen", "Extend Term", [&]{
        if(!state.id_carte_str.empty()) {
            state.biblio.prelungesteImprumut(std::stoi(state.id_carte_str));
            state.mesaj_actiune = "Termen prelungit!"; state.id_carte_str = "";
        }
    });

    std::vector<Component> t3;
    t3.push_back(inp_id_return); t3.push_back(btn_force_return); t3.push_back(btn_extend);
    tab3_container = Container::Vertical(t3);

    // --- TAB-URI INTERNE ȘI BUTON DE LOGOUT ---
    std::vector<Component> tabs;
    tabs.push_back(tab1_container);
    tabs.push_back(tab2_container);
    tabs.push_back(tab3_container);
    tab_selector = Container::Tab(tabs, &tab_biblio_index);

    btn_back = MakeSmallBtn(state.limba_selectata, "Logout", "Logout", [&]{ 
        state.ecran_activ = 0; state.user_curent = ""; state.mesaj_actiune = "";
    });

    std::vector<Component> main_comps;
    main_comps.push_back(tab_selector);
    main_comps.push_back(btn_back);
    container = Container::Vertical(main_comps);
}

Element BiblioScreen::Render() {
    // Titluri pentru Tab-uri
    Elements tab_titles;
    tab_titles.push_back(text(" 1. Adaugare ") | (tab_biblio_index == 0 ? bold | color(Color::Green) : dim));
    tab_titles.push_back(text(" 2. Inventar ") | (tab_biblio_index == 1 ? bold | color(Color::Blue) : dim));
    tab_titles.push_back(text(" 3. Imprumuturi ") | (tab_biblio_index == 2 ? bold | color(Color::Red) : dim));
    
    Element header_tabs = hbox(tab_titles) | border;

    Element content;
    
    if (tab_biblio_index == 0) {
        Elements form;
        form.push_back(text("Formular Adaugare Carte") | bold | underlined);
        form.push_back(inp_titlu->Render() | border);
        form.push_back(inp_autor->Render() | border);
        
        Elements row2; row2.push_back(inp_isbn->Render() | flex | border); row2.push_back(inp_an->Render() | size(WIDTH, EQUAL, 12) | border);
        form.push_back(hbox(row2));
        
        form.push_back(inp_editura->Render() | border);
        form.push_back(inp_locatie->Render() | border);
        form.push_back(btn_add->Render() | center);
        form.push_back(text(state.mesaj_actiune) | color(Color::Yellow) | center);
        content = vbox(form);
    } 
    else if (tab_biblio_index == 1) {
        Elements inv;
        inv.push_back(text("Inventar Complet (toate cartile)") | bold | underlined);
        for(auto& c : state.biblio.getCartiFormatate()) inv.push_back(text(" " + c));
        if(inv.size() == 1) inv.push_back(text(" Nicio carte in inventar.") | dim);
        inv.push_back(btn_back_inv->Render() | center);
        content = vbox(inv);
    } 
    else if (tab_biblio_index == 2) {
        Elements gest;
        gest.push_back(text("Carti Imprumutate Activ") | bold | underlined);
        for(auto& c : state.biblio.getCartiImprumutate()) gest.push_back(text(" ⚠️ " + c) | color(Color::Yellow));
        if(gest.size() == 1) gest.push_back(text(" Nu sunt carti imprumutate momentan.") | dim);
        
        gest.push_back(separator());
        gest.push_back(text("Actiuni Administrative:") | bold);
        gest.push_back(hbox({inp_id_return->Render() | size(WIDTH, EQUAL, 12) | border, btn_force_return->Render(), btn_extend->Render()}));
        content = vbox(gest);
    }

    return window(text(" 📚 PANOU BIBLIOTECAR PRO "), vbox({
        header_tabs,
        content | flex,
        btn_back->Render() | center
    })) | flex;
}