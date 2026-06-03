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
    ftxui::Component inp_titlu, inp_autor, inp_locatie, btn_add, btn_back, container;
};