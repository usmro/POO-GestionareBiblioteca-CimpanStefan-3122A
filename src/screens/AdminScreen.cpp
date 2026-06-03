#include "screens/AdminScreen.h"
#include "Theme.h"
using namespace ftxui;

AdminScreen::AdminScreen(AppState& s) : state(s) {
    btn_back = MakeSmallBtn(state.limba_selectata, "Logout", "Logout", [&]{ 
    state.ecran_activ = 0; 
    state.user_curent = "";
    });
    container = Container::Vertical({btn_back});
}

Element AdminScreen::Render() {
    Elements a_col, lista;
    for(auto& c : state.biblio.getCititori()) lista.push_back(text(" " + c));
    a_col.push_back(text("ADMIN") | bold | color(Color::Red));
    a_col.push_back(vbox(lista) | border);
    a_col.push_back(btn_back->Render());
    return window(text("ADMIN"), vbox(a_col));
}