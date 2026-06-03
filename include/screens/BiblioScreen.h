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
    
    // Butoanele de Tab
    ftxui::Component btn_tab_1, btn_tab_2, btn_tab_3, btn_tab_4, btn_tab_5;
    int tab_index = 0;
    ftxui::Component tab_selector;

    // Tab 1: Adaugare
    ftxui::Component inp_t1_titlu, inp_t1_autor, inp_t1_loc, inp_t1_stoc, btn_t1_add, tab1_container;
    // Tab 2: Inventar
    ftxui::Component tab2_container;
    // Tab 3: Imprumuturi
    ftxui::Component inp_t3_id, btn_t3_return, btn_t3_extend, tab3_container;
    // Tab 4: Receptie
    ftxui::Component inp_t4_titlu, inp_t4_autor, inp_t4_cantitate, btn_t4_receptie, tab4_container;
    // Tab 5: Utilizatori & Statistici
    ftxui::Component tab5_container;
    
    // General
    ftxui::Component btn_back, container;
};