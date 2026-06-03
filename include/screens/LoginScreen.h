#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "AppState.h"

class LoginScreen {
public:
    LoginScreen(AppState& state);
    ftxui::Component Get() { return container; }
    ftxui::Element Render();

private:
    AppState& state;
    ftxui::Component inp_user, inp_pass, btn_login_exec, btn_login_back, container;
};