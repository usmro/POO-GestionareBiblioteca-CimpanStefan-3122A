#include <iostream>
#include <vector>
#include "Biblioteca.h"
#include "AppState.h"
#include "Theme.h"
#include "BiblioAI.h"
#include "screens/StartScreen.h"
#include "screens/LoginScreen.h"
#include "screens/UserScreen.h"
#include "screens/BiblioScreen.h"
#include "screens/AdminScreen.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

int main() {
    Biblioteca biblio;
    auto screen = ScreenInteractive::Fullscreen();
    AppState state(biblio);
    
    auto toggle_lang = Toggle(&state.optiuni_limba, &state.limba_selectata);
    
    StartScreen startScreen(state, screen);
    LoginScreen loginScreen(state);
    UserScreen userScreen(state);
    BiblioScreen biblioScreen(state);
    AdminScreen adminScreen(state);
    BiblioAI biblioAI(state);
    
    std::vector<Component> tab_comps;
    tab_comps.push_back(startScreen.Get());
    tab_comps.push_back(loginScreen.Get());
    tab_comps.push_back(userScreen.Get());
    tab_comps.push_back(biblioScreen.Get());
    tab_comps.push_back(adminScreen.Get());
    
    auto sistem_rutare = Container::Tab(tab_comps, &state.ecran_activ);
    
    std::vector<Component> glob_comps;
    glob_comps.push_back(toggle_lang);
    glob_comps.push_back(sistem_rutare);
    glob_comps.push_back(biblioAI.GetInputComponent());
    
    auto aplicatie_completa = Container::Vertical(glob_comps);

    auto renderer = Renderer(aplicatie_completa, [&] {
        auto ui_header = hbox({
            text(" Library Manager PRO ") | bold | color(Color::Black) | bgcolor(Color::CyanLight),
            filler(),
            toggle_lang->Render()
        });

        Element ui_content;
        if (state.ecran_activ == 0) ui_content = startScreen.Render();
        else if (state.ecran_activ == 1) ui_content = loginScreen.Render();
        else if (state.ecran_activ == 2) ui_content = userScreen.Render();    // <--- 2, nu 3
        else if (state.ecran_activ == 3) ui_content = biblioScreen.Render();  // <--- 3, nu 4
        else if (state.ecran_activ == 4) ui_content = adminScreen.Render();   // <--- 4, nu 5
        else ui_content = text("Ecran in dezvoltare...");

        Element ui_ai = biblioAI.Render();
        
        Elements final_layout;
        final_layout.push_back(ui_header);
        final_layout.push_back(separatorEmpty()); 
        final_layout.push_back(ui_content);
        final_layout.push_back(ui_ai);
        
        return vbox(final_layout) | flex;
    });

    screen.Loop(renderer);
    return 0;
}