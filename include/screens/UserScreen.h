#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "AppState.h"

class UserScreen {
public:
    UserScreen(AppState& state);
    ftxui::Component Get() { return container; }
    ftxui::Element Render();

private:
    AppState& state;
    int scroll_index = 0;
    
    ftxui::Component inp_cautare, btn_cauta, btn_tendinte;
    ftxui::Component btn_filtru_toate, btn_filtru_libere, btn_filtru_mele, btn_filtru_tendinte; 
    ftxui::Component inp_id;
    ftxui::Component btn_imprumuta, btn_returneaza, btn_rezerva, btn_prelungeste, btn_vezi_mele;
    ftxui::Component btn_back, container;
};