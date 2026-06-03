#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "AppState.h"

class RegisterScreen {
public:
    RegisterScreen(AppState& state);
    ftxui::Component Get() { return container; }
    ftxui::Element Render();
private:
    AppState& state;
    ftxui::Component inp_user, inp_email, inp_pass, inp_confirm, inp_cod, btn_exec, btn_back, container;
};