#include "screens/UserScreen.h"
#include "Theme.h"
#include <ftxui/dom/elements.hpp>
#include <vector>
#include <iostream>
#include <sstream>
using namespace ftxui;

UserScreen::UserScreen(AppState& s) : state(s) {
    // 1. Căutare
    inp_cautare = Input(&state.query_cautare, "Cauta dupa titlu/autor...");
    btn_cauta = MakeSmallBtn(state.limba_selectata, "Cauta", "Search", [&]{
        state.lista_cautare = state.biblio.cautaCarti(state.query_cautare);
        state.afiseaza_rezultate_cautare = true;
        state.mod_ai_activ = false;
        scroll_index = 0;
    });

    // 2. Filtre
    btn_filtru_toate = Button(" Toate ", [&]{ state.mod_vizualizare_user = 0; state.afiseaza_rezultate_cautare = false; state.mod_ai_activ = false; scroll_index = 0; }, ButtonOption::Animated());
    btn_filtru_libere = Button(" Libere ", [&]{ state.mod_vizualizare_user = 1; state.afiseaza_rezultate_cautare = false; state.mod_ai_activ = false; scroll_index = 0; }, ButtonOption::Animated());
    btn_filtru_mele = Button(" Ale Mele ", [&]{ state.mod_vizualizare_user = 2; state.afiseaza_rezultate_cautare = false; state.mod_ai_activ = false; scroll_index = 0; }, ButtonOption::Animated());
    btn_filtru_tendinte = Button(" Tendinte ", [&]{ state.mod_vizualizare_user = 3; state.afiseaza_rezultate_cautare = false; state.mod_ai_activ = false; state.lista_tendinte = state.biblio.iaTendinteOnline(); scroll_index = 0; }, ButtonOption::Animated());

    // 3. Acțiuni Cărți
    inp_id = Input(&state.id_carte_str, "ID Carte (ex: 1)");
    btn_imprumuta = MakeSmallBtn(state.limba_selectata, "Imprumuta", "Borrow", [&]{
        try { if(!state.id_carte_str.empty() && state.biblio.imprumutaCarte(std::stoi(state.id_carte_str), state.user_curent)) { state.mesaj_actiune = "SUCCESS!"; state.mod_vizualizare_user = 2; } else { state.mesaj_actiune = "EROARE."; } state.id_carte_str = ""; } catch(...) { state.mesaj_actiune = "ID invalid!"; }
    });
    btn_returneaza = MakeSmallBtn(state.limba_selectata, "Returneaza", "Return", [&]{
        try { if(!state.id_carte_str.empty() && state.biblio.returneazaCarte(std::stoi(state.id_carte_str), state.user_curent)) { state.mesaj_actiune = "SUCCESS!"; } else { state.mesaj_actiune = "EROARE."; } state.id_carte_str = ""; state.mod_vizualizare_user = 2; } catch(...) { state.mesaj_actiune = "ID invalid!"; }
    });
    btn_rezerva = MakeSmallBtn(state.limba_selectata, "Rezerva", "Reserve", [&]{
        try { if(!state.id_carte_str.empty() && state.biblio.rezervaCarte(std::stoi(state.id_carte_str), state.user_curent)) { state.mesaj_actiune = "SUCCESS!"; } else { state.mesaj_actiune = "EROARE."; } state.id_carte_str = ""; } catch(...) { state.mesaj_actiune = "ID invalid!"; }
    });
    btn_prelungeste = MakeSmallBtn(state.limba_selectata, "Prelungeste", "Extend", [&]{
        try { if(!state.id_carte_str.empty() && state.biblio.prelungesteImprumut(std::stoi(state.id_carte_str))) { state.mesaj_actiune = "SUCCESS!"; } else { state.mesaj_actiune = "EROARE."; } state.id_carte_str = ""; } catch(...) { state.mesaj_actiune = "ID invalid!"; }
    });
    btn_vezi_mele = MakeSmallBtn(state.limba_selectata, "Cartile Mele", "My Books", [&]{ state.mod_vizualizare_user = 2; state.afiseaza_rezultate_cautare = false; state.mod_ai_activ = false; scroll_index = 0; });

    // 4. BUTONUL CHAT AI (Setează flag-ul, nu schimbă ecranul)
    btn_chat_ai = Button(" 💬 BiblioAI ", [&]{ 
        state.mod_ai_activ = true; 
        state.afiseaza_rezultate_cautare = false;
        if(state.ai_raspuns.empty()) state.ai_raspuns = "🤖 BiblioAI: Bun venit! Cu ce te pot ajuta astăzi?\n\n";
        scroll_index = 1000;
    }, ButtonOption::Animated());

    // 5. Componente Chat
    inp_ai_prompt = Input(&state.ai_prompt, "Scrie mesajul aici...");
    btn_send_ai = Button(" Trimite ", [&]{
        if(!state.ai_prompt.empty()) {
            state.ai_raspuns += "👤 Tu: " + state.ai_prompt + "\n";
            state.ai_raspuns += "🤖 BiblioAI: " + state.biblio.intreabaBiblioAI(state.ai_prompt) + "\n\n";
            state.ai_prompt = "";
            scroll_index = 1000; // Derulează jos
        }
    });
    btn_back_ai = Button(" Inapoi la carti ", [&]{ state.mod_ai_activ = false; scroll_index = 0; });

    btn_back = MakeSmallBtn(state.limba_selectata, "Logout", "Logout", [&]{ state.ecran_activ = 0; state.user_curent = ""; state.mesaj_actiune = ""; state.mod_ai_activ = false; scroll_index = 0; });

    // 6. Asamblare Container (TOATE butoanele aici)
    std::vector<Component> comps;
    comps.push_back(inp_cautare); comps.push_back(btn_cauta);
    comps.push_back(btn_filtru_toate); comps.push_back(btn_filtru_libere); 
    comps.push_back(btn_filtru_mele); comps.push_back(btn_filtru_tendinte);
    comps.push_back(btn_chat_ai); // Butonul care activează chat-ul
    comps.push_back(inp_id);
    comps.push_back(btn_imprumuta); comps.push_back(btn_returneaza);
    comps.push_back(btn_rezerva); comps.push_back(btn_prelungeste);
    comps.push_back(btn_vezi_mele);
    comps.push_back(btn_back);
    
    container = Container::Vertical(comps);

    // 7. Scroll logic
    container = CatchEvent(container, [this](Event event) {
        int lista_size = state.mod_ai_activ ? 20 : (state.afiseaza_rezultate_cautare ? state.lista_cautare.size() : (state.mod_vizualizare_user == 0 ? state.biblio.getCartiPentruUser().size() : (state.mod_vizualizare_user == 1 ? state.biblio.getCartiDisponibilePentruUser().size() : (state.mod_vizualizare_user == 2 ? state.biblio.getCartiImprumutateDeUser(state.user_curent).size() : state.lista_tendinte.size()))));
        if (scroll_index > lista_size) scroll_index = lista_size;
        if (event == Event::ArrowUp && scroll_index > 0) { scroll_index--; return true; }
        if (event == Event::ArrowDown && scroll_index < lista_size - 1) { scroll_index++; return true; }
        return false;
    });
}

Element UserScreen::Render() {
    // === DACĂ SUNTEM ÎN MODUL CHAT, AFIȘĂM DOAR CHAT-UL ===
    if (state.mod_ai_activ) {
        Elements chat_lines;
        std::istringstream stream(state.ai_raspuns);
        std::string line;
        while (std::getline(stream, line)) {
            if (!line.empty()) {
                if (line.find("👤 Tu:") != std::string::npos) {
                    chat_lines.push_back(text(line) | color(Color::Cyan) | bold);
                } else if (line.find("🤖 BiblioAI:") != std::string::npos) {
                    chat_lines.push_back(text(line) | color(Color::Green));
                } else {
                    chat_lines.push_back(text(line));
                }
            }
        }

        auto chat_renderer = Renderer([this, &chat_lines]() {
            Elements visible;
            int start = scroll_index;
            int end = std::min(start + 15, (int)chat_lines.size());
            for(int i = start; i < end; i++) visible.push_back(chat_lines[i]);
            if (start > 0) visible.insert(visible.begin(), text("  ... (sus) ...") | dim);
            if (end < (int)chat_lines.size()) visible.push_back(text("  ... (jos) ...") | dim);
            return vbox(visible) | border;
        });

        return window(text(" 🤖 BIBLIOAI CHAT "), vbox({
            chat_renderer->Render() | flex,
            hbox(
                inp_ai_prompt->Render() | flex | border,
                btn_send_ai->Render()
            ),
            btn_back_ai->Render() | center
        })) | flex;
    }

    // === INTERFAȚA NORMALĂ DE UTILIZATOR ===
    std::vector<std::string> lista_afisata;
    if (state.afiseaza_rezultate_cautare) lista_afisata = state.lista_cautare;
    else if (state.mod_vizualizare_user == 0) lista_afisata = state.biblio.getCartiPentruUser();
    else if (state.mod_vizualizare_user == 1) lista_afisata = state.biblio.getCartiDisponibilePentruUser();
    else if (state.mod_vizualizare_user == 2) lista_afisata = state.biblio.getCartiImprumutateDeUser(state.user_curent);
    else if (state.mod_vizualizare_user == 3) lista_afisata = state.lista_tendinte;
    
    Elements u_col, lista_c;
    for(auto& c : lista_afisata) lista_c.push_back(text("  📖 " + c));
    if(lista_c.empty()) lista_c.push_back(text("  Nicio carte in aceasta categorie.") | dim);
    
    u_col.push_back(hbox({text("Utilizator: ") | bold | color(Color::Cyan), text(state.user_curent)}));
    u_col.push_back(separator());
    u_col.push_back(text("Cauta:") | bold);
    u_col.push_back(hbox({inp_cautare->Render() | flex, btn_cauta->Render()}));
    u_col.push_back(separator());
    
    u_col.push_back(text("Filtru vizualizare:") | bold);
    Elements filtre_row;
    filtre_row.push_back(btn_filtru_toate->Render() | (state.mod_vizualizare_user == 0 ? color(Color::Green) : dim));
    filtre_row.push_back(btn_filtru_libere->Render() | (state.mod_vizualizare_user == 1 ? color(Color::Green) : dim));
    filtre_row.push_back(btn_filtru_mele->Render() | (state.mod_vizualizare_user == 2 ? color(Color::Green) : dim));
    filtre_row.push_back(btn_filtru_tendinte->Render() | (state.mod_vizualizare_user == 3 ? color(Color::Magenta) : dim));
    u_col.push_back(hbox(filtre_row));
    
    auto scroll_renderer = Renderer([this, &lista_c]() {
        Elements visible_items;
        int start = scroll_index;
        int end = std::min(start + 8, (int)lista_c.size());
        for(int i = start; i < end; i++) visible_items.push_back(lista_c[i]);
        if (start > 0) visible_items.insert(visible_items.begin(), text("  ... (sus) ...") | dim);
        if (end < (int)lista_c.size()) visible_items.push_back(text("  ... (jos) ...") | dim);
        return vbox(visible_items) | border;
    });
    
    u_col.push_back(scroll_renderer->Render());
    u_col.push_back(text("Foloseste sagetile SUS/JOS pentru a derula lista") | dim | center);
    u_col.push_back(text("ID Carte pentru actiuni:") | bold);
    u_col.push_back(inp_id->Render() | size(WIDTH, EQUAL, 15));
    
    Elements actiuni;
    actiuni.push_back(btn_imprumuta->Render()); actiuni.push_back(btn_returneaza->Render());
    actiuni.push_back(btn_rezerva->Render()); actiuni.push_back(btn_prelungeste->Render());
    u_col.push_back(hbox(actiuni));
    
    u_col.push_back(btn_vezi_mele->Render() | center);
    u_col.push_back(text(state.mesaj_actiune) | color(Color::Yellow) | bold | center);
    u_col.push_back(btn_back->Render() | center);
    
    return window(text(" PANOU CITITOR "), vbox(u_col)) | flex;
}