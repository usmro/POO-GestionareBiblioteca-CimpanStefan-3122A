#include "screens/StartScreen.h"
#include "Theme.h"
#include <vector>

using namespace ftxui;

StartScreen::StartScreen(AppState& s, ScreenInteractive& screen) 
    : state(s), screen_ref(screen) {
    
    btn_login = MakeBigBtn(state.limba_selectata, "1. Intra in cont", "1. Login", [&]{ 
        state.ecran_activ = 1; // Mergem la Login
    });
    
    btn_register = MakeBigBtn(state.limba_selectata, "2. Creare cont", "2. Create Account", [&]{ 
        state.ecran_activ = 2; // <--- MERGEM LA ECRANUL DE REGISTER
    });
    
    btn_guest = MakeBigBtn(state.limba_selectata, "3. Vizitator", "3. Guest Mode", [&]{ 
        state.ecran_activ = 3; // Mergem la Guest (dacă îl ai)
    });
    
    btn_exit = MakeBigBtn(state.limba_selectata, "4. Iesire", "4. Exit", [&]{ 
        screen_ref.ExitLoopClosure()(); 
    });
    
    std::vector<Component> comps;
    comps.push_back(btn_login); 
    comps.push_back(btn_register); 
    comps.push_back(btn_guest); 
    comps.push_back(btn_exit);
    container = Container::Vertical(comps);
}

Element StartScreen::Render() {
    Elements col;
    col.push_back(filler());
    col.push_back(btn_login->Render());
    col.push_back(btn_register->Render());
    col.push_back(btn_guest->Render());
    col.push_back(btn_exit->Render());
    col.push_back(filler());
    return vbox(col) | center | flex;
}