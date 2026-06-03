#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "AppState.h"

class GuestScreen {
public:
    GuestScreen(AppState& state);
    ftxui::Component Get() { return container; }
    ftxui::Element Render();

private:
    AppState& state;
    // Căutare
    ftxui::Component inp_cautare, btn_cauta;
    // Tendințe
    ftxui::Component btn_tendinte;
    // Navigare
    ftxui::Component btn_back, container;
};