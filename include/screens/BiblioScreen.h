#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "AppState.h"

class BiblioScreen {
public:
    BiblioScreen(AppState& state);
    ftxui::Component Get() { return container; }
    ftxui::Element Render();
private:
    AppState& state;
    ftxui::Component tab_selector; // Tab-urile interne
    int tab_biblio_index = 0;
    
    // Tab 1: Adaugare
    ftxui::Component inp_titlu, inp_autor, inp_isbn, inp_editura, inp_an, inp_locatie, btn_add, tab1_container;
    
    // Tab 2: Inventar
    ftxui::Component btn_back_inv, tab2_container;

    // Tab 3: Gestiune Imprumuturi
    ftxui::Component inp_id_return, btn_force_return, btn_extend, tab3_container;
    
    ftxui::Component btn_back, container;
};