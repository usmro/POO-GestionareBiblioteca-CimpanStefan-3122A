#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp> 
#include <ftxui/dom/elements.hpp>
#include "AppState.h"

class StartScreen {
public:
    StartScreen(AppState& state, ftxui::ScreenInteractive& screen);
    ftxui::Component Get() { return container; }
    ftxui::Element Render();

private:
    AppState& state;
    ftxui::ScreenInteractive& screen_ref;
    ftxui::Component btn_login, btn_register, btn_guest, btn_exit, container;
};