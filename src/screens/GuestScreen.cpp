#include "screens/GuestScreen.h"
#include "Theme.h"
#include <vector>
using namespace ftxui;

GuestScreen::GuestScreen(AppState& s) : state(s) {
    // 1. Componente pentru CĂUTARE LOCALĂ
    inp_cautare = Input(&state.query_cautare, "Cauta dupa titlu sau autor...");
    btn_cauta = MakeSmallBtn(state.limba_selectata, "Cauta", "Search", [&]{
        state.lista_cautare = state.biblio.cautaCarti(state.query_cautare);
        state.afiseaza_rezultate_cautare = true;
    });

    // 2. Componente pentru TENDINȚE GLOBALE
    btn_tendinte = MakeSmallBtn(state.limba_selectata, "Vezi Tendinte", "View Trends", [&]{
        state.arata_tendinte = true;
        state.lista_tendinte.clear();
        state.lista_tendinte.push_back("Se incarca datele... (asteapta 1-2 secunde)");
        // Aici se face cererea API (cu timeout de 3 secunde din Biblioteca.cpp)
        state.lista_tendinte = state.biblio.iaTendinteOnline();
    });

    // 3. Buton de ieșire
    btn_back = MakeSmallBtn(state.limba_selectata, "Inapoi", "Back", [&]{ 
        state.ecran_activ = 0; 
        state.arata_tendinte = false;
        state.afiseaza_rezultate_cautare = false;
        state.query_cautare = "";
    });

    // 4. Container (Toate butoanele aici pentru a primi click)
    std::vector<Component> comps;
    comps.push_back(inp_cautare);
    comps.push_back(btn_cauta);
    comps.push_back(btn_tendinte);
    comps.push_back(btn_back);
    container = Container::Vertical(comps);
}

Element GuestScreen::Render() {
    // Alegem ce listă să afișăm (rezultatele căutării sau tot inventarul)
    std::vector<std::string> lista_locala = state.afiseaza_rezultate_cautare ? 
        state.lista_cautare : state.biblio.getToateCartile();
    
    Elements col;
    
    // Header
    col.push_back(text("⚠️ MOD VIZITATOR") | bold | color(Color::Yellow) | center);
    col.push_back(separator());
    
    // ZONA 1: CĂUTARE LOCALĂ
    col.push_back(text("🔍 Cauta in biblioteca locala:") | bold);
    Elements search_row;
    search_row.push_back(inp_cautare->Render() | flex | border);
    search_row.push_back(btn_cauta->Render());
    col.push_back(hbox(search_row));
    
    Elements lista_elem;
    for(auto& c : lista_locala) lista_elem.push_back(text("  📖 " + c));
    if(lista_elem.empty()) lista_elem.push_back(text("  Nicio carte gasita.") | dim);
    col.push_back(vbox(lista_elem) | border | size(HEIGHT, LESS_THAN, 10) | flex);
    
    // ZONA 2: TENDINȚE GLOBALE
    col.push_back(separator());
    col.push_back(btn_tendinte->Render() | center);
    
    if (state.arata_tendinte) {
        col.push_back(text("🔥 TENDINTE GLOBALE (Open Library):") | bold | color(Color::Magenta) | center);
        Elements lista_ext;
        for(auto& c : state.lista_tendinte) lista_ext.push_back(text("  🌟 " + c));
        if(lista_ext.empty()) lista_ext.push_back(text("  Se incarca...") | dim);
        col.push_back(vbox(lista_ext) | border | flex);
    }
    
    // Footer
    col.push_back(separator());
    col.push_back(btn_back->Render() | center);
    
    return window(state.elem_txt(" CATALOG PUBLIC ", " PUBLIC CATALOG "), vbox(col)) | flex;
}