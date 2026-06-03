#include "screens/UserScreen.h"
#include "Theme.h"
#include <ftxui/dom/elements.hpp>
#include <vector>
#include <iostream>
using namespace ftxui;

UserScreen::UserScreen(AppState& s) : state(s) {
    // 1. Componente Căutare (fără btn_tendinte aici)
    inp_cautare = Input(&state.query_cautare, "Cauta dupa titlu/autor...");
    
    btn_cauta = MakeSmallBtn(state.limba_selectata, "Cauta", "Search", [&]{
        state.lista_cautare = state.biblio.cautaCarti(state.query_cautare);
        state.afiseaza_rezultate_cautare = true;
        state.arata_tendinte = false;
        scroll_index = 0;
    });

    // 2. Filtre Vizualizare (adăugăm al 4-lea buton - Tendințe)
    btn_filtru_toate = Button(" Toate ", [&]{ 
        state.mod_vizualizare_user = 0; 
        state.afiseaza_rezultate_cautare = false; 
        state.arata_tendinte = false; 
        scroll_index = 0;
    }, ButtonOption::Animated());
    
    btn_filtru_libere = Button(" Libere ", [&]{ 
        state.mod_vizualizare_user = 1; 
        state.afiseaza_rezultate_cautare = false; 
        state.arata_tendinte = false; 
        scroll_index = 0;
    }, ButtonOption::Animated());
    
    btn_filtru_mele = Button(" Ale Mele ", [&]{ 
        state.mod_vizualizare_user = 2; 
        state.afiseaza_rezultate_cautare = false; 
        state.arata_tendinte = false; 
        scroll_index = 0;
    }, ButtonOption::Animated());

    // BUTONUL NOU: Tendințe (al 4-lea filtru)
    btn_filtru_tendinte = Button(" Tendinte ", [&]{ 
        state.mod_vizualizare_user = 3;  // Mod nou pentru tendințe
        state.afiseaza_rezultate_cautare = false; 
        state.arata_tendinte = true;
        state.lista_tendinte = state.biblio.iaTendinteOnline();
        scroll_index = 0;
    }, ButtonOption::Animated());

    // 3. Acțiuni cu Cărți
    inp_id = Input(&state.id_carte_str, "ID Carte (ex: 1)");
    
    btn_imprumuta = MakeSmallBtn(state.limba_selectata, "Imprumuta", "Borrow", [&]{
        try {
            if(!state.id_carte_str.empty()) {
                int id = std::stoi(state.id_carte_str);
                if(state.biblio.imprumutaCarte(id, state.user_curent)) {
                    state.mesaj_actiune = "SUCCESS: Carte imprumutata!";
                    state.mod_vizualizare_user = 2; 
                } else { state.mesaj_actiune = "EROARE: Carte indisponibila sau ID gresit."; }
                state.id_carte_str = "";
            } else { state.mesaj_actiune = "Scrie un ID in casuta de sus!"; }
        } catch(...) { state.mesaj_actiune = "ID invalid! Scrie doar numere."; }
    });

    btn_returneaza = MakeSmallBtn(state.limba_selectata, "Returneaza", "Return", [&]{
        try {
            if(!state.id_carte_str.empty()) {
                int id = std::stoi(state.id_carte_str);
                if(state.biblio.returneazaCarte(id, state.user_curent)) {
                    state.mesaj_actiune = "SUCCESS: Carte returnata!";
                } else { state.mesaj_actiune = "EROARE: Nu ai aceasta carte."; }
                state.id_carte_str = "";
                state.mod_vizualizare_user = 2;
            }
        } catch(...) { state.mesaj_actiune = "ID invalid!"; }
    });

    btn_rezerva = MakeSmallBtn(state.limba_selectata, "Rezerva", "Reserve", [&]{
        try {
            if(!state.id_carte_str.empty()) {
                int id = std::stoi(state.id_carte_str);
                if(state.biblio.rezervaCarte(id, state.user_curent)) {
                    state.mesaj_actiune = "SUCCESS: Carte rezervata!";
                } else { state.mesaj_actiune = "EROARE la rezervare."; }
                state.id_carte_str = "";
            }
        } catch(...) { state.mesaj_actiune = "ID invalid!"; }
    });

    btn_prelungeste = MakeSmallBtn(state.limba_selectata, "Prelungeste", "Extend", [&]{
        try {
            if(!state.id_carte_str.empty()) {
                int id = std::stoi(state.id_carte_str);
                if(state.biblio.prelungesteImprumut(id)) {
                    state.mesaj_actiune = "SUCCESS: Termen prelungit!";
                } else { state.mesaj_actiune = "EROARE."; }
                state.id_carte_str = "";
            }
        } catch(...) { state.mesaj_actiune = "ID invalid!"; }
    });

    btn_vezi_mele = MakeSmallBtn(state.limba_selectata, "Cartile Mele", "My Books", [&]{
        state.mod_vizualizare_user = 2;
        state.mesaj_actiune = "Se afiseaza doar cartile tale imprumutate.";
        state.afiseaza_rezultate_cautare = false;
        scroll_index = 0;
    });

    btn_back = MakeSmallBtn(state.limba_selectata, "Logout", "Logout", [&]{ 
        state.ecran_activ = 0; state.user_curent = ""; state.mesaj_actiune = ""; scroll_index = 0;
    });

    // 4. Container (actualizat)
    std::vector<Component> comps;
    comps.push_back(inp_cautare); comps.push_back(btn_cauta);
    // NU mai adăuga btn_tendinte aici
    comps.push_back(btn_filtru_toate); comps.push_back(btn_filtru_libere); 
    comps.push_back(btn_filtru_mele); comps.push_back(btn_filtru_tendinte); // <--- Aici e acum
    comps.push_back(inp_id);
    comps.push_back(btn_imprumuta); comps.push_back(btn_returneaza);
    comps.push_back(btn_rezerva); comps.push_back(btn_prelungeste);
    comps.push_back(btn_vezi_mele);
    comps.push_back(btn_back);
    
    container = Container::Vertical(comps);

    // ADAUGĂ ASTA IMEDIAT DUPĂ LINIA DE MAI SUS
    container = CatchEvent(container, [this](Event event) {
        int lista_size = 0;
        if (state.afiseaza_rezultate_cautare) {
            lista_size = state.lista_cautare.size();
        } else {
            if (state.mod_vizualizare_user == 0) lista_size = state.biblio.getCartiPentruUser().size();
            else if (state.mod_vizualizare_user == 1) lista_size = state.biblio.getCartiDisponibilePentruUser().size();
            else if (state.mod_vizualizare_user == 2) lista_size = state.biblio.getCartiImprumutateDeUser(state.user_curent).size();
            else if (state.mod_vizualizare_user == 3) lista_size = state.lista_tendinte.size();
        }
        
        if (scroll_index > lista_size) scroll_index = lista_size;
        if (event == Event::ArrowUp && scroll_index > 0) { scroll_index--; return true; }
        if (event == Event::ArrowDown && scroll_index < lista_size - 1) { scroll_index++; return true; }
        return false;
    });
}

Element UserScreen::Render() {
    std::vector<std::string> lista_afisata;
    
    if (state.afiseaza_rezultate_cautare) {
        lista_afisata = state.lista_cautare;
    } else if (state.mod_vizualizare_user == 0) {
        lista_afisata = state.biblio.getCartiPentruUser();
    } else if (state.mod_vizualizare_user == 1) {
        lista_afisata = state.biblio.getCartiDisponibilePentruUser();
    } else if (state.mod_vizualizare_user == 2) {
        lista_afisata = state.biblio.getCartiImprumutateDeUser(state.user_curent);
    } else if (state.mod_vizualizare_user == 3) {
        lista_afisata = state.lista_tendinte;
    }
    
    Elements u_col, lista_c;
    for(auto& c : lista_afisata) lista_c.push_back(text("  📖 " + c));
    if(lista_c.empty()) lista_c.push_back(text("  Nicio carte in aceasta categorie.") | dim);
    
    // Header
    u_col.push_back(hbox({text("Utilizator: ") | bold | color(Color::Cyan), text(state.user_curent)}));
    u_col.push_back(separator());
    
    // Zona Căutare (fără btn_tendinte)
    u_col.push_back(text("Cauta:") | bold);
    Elements search_row;
    search_row.push_back(inp_cautare->Render() | flex);
    search_row.push_back(btn_cauta->Render());
    // NU mai adăuga btn_tendinte aici
    u_col.push_back(hbox(search_row));
    
    u_col.push_back(separator());
    
    // Zona Filtre (cu 4 butoane acum)
    u_col.push_back(text("Filtru vizualizare:") | bold);
    Elements filtre_row;
    filtre_row.push_back(btn_filtru_toate->Render() | (state.mod_vizualizare_user == 0 ? color(Color::Green) : dim));
    filtre_row.push_back(btn_filtru_libere->Render() | (state.mod_vizualizare_user == 1 ? color(Color::Green) : dim));
    filtre_row.push_back(btn_filtru_mele->Render() | (state.mod_vizualizare_user == 2 ? color(Color::Green) : dim));
    filtre_row.push_back(btn_filtru_tendinte->Render() | (state.mod_vizualizare_user == 3 ? color(Color::Magenta) : dim)); // <--- Butonul nou
    u_col.push_back(hbox(filtre_row));
    
    // Zona Lista cu Scroll
    auto scroll_renderer = Renderer([this, &lista_c]() {
        Elements visible_items;
        int start = scroll_index;
        int end = std::min(start + 8, (int)lista_c.size());
        
        for(int i = start; i < end; i++) {
            visible_items.push_back(lista_c[i]);
        }
        
        if (start > 0) visible_items.insert(visible_items.begin(), text("  ... (sus) ...") | dim);
        if (end < (int)lista_c.size()) visible_items.push_back(text("  ... (jos) ...") | dim);
        
        return vbox(visible_items) | border;
    });
    
    u_col.push_back(scroll_renderer->Render());
    u_col.push_back(text("Foloseste sagetile SUS/JOS pentru a derula lista") | dim | center);
    
    // Zona Acțiuni
    u_col.push_back(text("ID Carte pentru actiuni:") | bold);
    u_col.push_back(inp_id->Render() | size(WIDTH, EQUAL, 15));
    
    Elements actiuni;
    actiuni.push_back(btn_imprumuta->Render()); actiuni.push_back(btn_returneaza->Render());
    actiuni.push_back(btn_rezerva->Render()); actiuni.push_back(btn_prelungeste->Render());
    u_col.push_back(hbox(actiuni));
    
    u_col.push_back(btn_vezi_mele->Render() | center);
    u_col.push_back(text(state.mesaj_actiune) | color(Color::Yellow) | bold | center);
    u_col.push_back(btn_back->Render() | center);
    
    return window(text(" PANOU CITITOR "), vbox(u_col)) | flex;
}