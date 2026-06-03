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
    
    // Butoanele de sus (Tab-uri)
    ftxui::Component btn_tab_1, btn_tab_2, btn_tab_3, btn_tab_4;
    int tab_index = 0;
    ftxui::Component tab_selector;

    // Tab 1: Adaugare Simplă
    ftxui::Component inp_t1_titlu, inp_t1_autor, inp_t1_loc, inp_t1_stoc, btn_t1_add, tab1_container;
    
    // Tab 2: Inventar (doar vizualizare)
    ftxui::Component tab2_container;

    // Tab 3: Gestiune Imprumuturi
    ftxui::Component inp_t3_id, btn_t3_return, btn_t3_extend, tab3_container;

    // Tab 4: Receptie Marfă (Adaugare multiplă)
    ftxui::Component inp_t4_titlu, inp_t4_autor, inp_t4_cantitate, btn_t4_receptie, tab4_container;
    
    // General
    ftxui::Component btn_back, container;

    // Ascundere stoc carti user
    std::vector<std::string> getCartiPentruUser();
    std::vector<std::string> getCartiDisponibilePentruUser();
};