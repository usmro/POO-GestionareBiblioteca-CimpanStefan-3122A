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
    ftxui::Component inp_id, btn_imprumuta, btn_returneaza, btn_back, container;
};