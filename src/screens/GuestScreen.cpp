#include "screens/GuestScreen.h"
#include "Theme.h"
using namespace ftxui;

GuestScreen::GuestScreen(AppState& s) : state(s) {
    btn_back = MakeSmallBtn(state.limba_selectata, "Inapoi", "Back", [&]{ state.ecran_activ = 0; });
    container = Container::Vertical({btn_back});
}

Element GuestScreen::Render() {
    Elements lista;
    for(auto& c : state.biblio.getCartiFormatate()) lista.push_back(text("  " + c));
    
    Elements col;
    col.push_back(text("⚠️ MOD VIZITATOR - Doar vizualizare") | bold | color(Color::Yellow) | center);
    col.push_back(separator());
    col.push_back(vbox(lista) | border | flex);
    col.push_back(btn_back->Render() | center);
    
    return window(state.elem_txt(" CATALOG PUBLIC ", " PUBLIC CATALOG "), vbox(col)) | flex;
}