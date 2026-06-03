#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "AppState.h"

class BiblioAI {
public:
    BiblioAI(AppState& state);
    ftxui::Component GetInputComponent() { return ai_switcher; } // Returnează switcher-ul, nu containerul direct
    ftxui::Element Render();
    
    void Toggle() { 
        state.mod_ai_activ = !state.mod_ai_activ; 
        ai_tab_index = state.mod_ai_activ ? 1 : 0; // Schimbă tab-ul intern
    }

private:
    AppState& state;
    ftxui::Component inp_ai, btn_trimite, btn_toggle, container;
    ftxui::Component ai_switcher; // Noul switcher
    int ai_tab_index = 0;
    
    void HandleSend();
    void UpdateTyping();
};