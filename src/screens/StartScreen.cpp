#include "screens/StartScreen.h"
#include "Theme.h"

using namespace ftxui;

StartScreen::StartScreen(AppState& s, ScreenInteractive& screen) 
    : state(s), screen_ref(screen) {
    
    btn_login = MakeBigBtn(state.limba_selectata, "1. Intra in cont", "1. Login", [&]{ 
        state.ecran_activ = 1; 
    });
    
    btn_register = MakeBigBtn(state.limba_selectata, "2. Creare cont", "2. Create Account", [&]{ 
        state.ecran_activ = 6; 
    });
    
    btn_guest = MakeBigBtn(state.limba_selectata, "3. Intra ca vizitator", "3. Guest Mode", [&]{ 
        state.ecran_activ = 2; 
    });
    
    btn_exit = MakeBigBtn(state.limba_selectata, "4. Iesire", "4. Exit", [&]{ 
        screen_ref.ExitLoopClosure()(); 
    });
    
    container = Container::Vertical({btn_login, btn_register, btn_guest, btn_exit});
}

Element StartScreen::Render() {
    return vbox({
        filler(),
        btn_login->Render(),
        btn_register->Render(),
        btn_guest->Render(),
        btn_exit->Render(),
        filler()
    }) | center | flex;
}