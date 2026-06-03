#include "screens/BiblioScreen.h"
#include "Theme.h"
#include <vector>
using namespace ftxui;

BiblioScreen::BiblioScreen(AppState& s) : state(s) {
    // 1. Inițializare butoane Tab (TOATE trebuie inițializate!)
    btn_tab_1 = Button(" 1. Adaugare ", [&]{ tab_index = 0; }, ButtonOption::Animated());
    btn_tab_2 = Button(" 2. Inventar ", [&]{ tab_index = 1; }, ButtonOption::Animated());
    btn_tab_3 = Button(" 3. Imprumuturi ", [&]{ tab_index = 2; }, ButtonOption::Animated());
    btn_tab_4 = Button(" 4. Receptie ", [&]{ tab_index = 3; }, ButtonOption::Animated());
    btn_tab_5 = Button(" 5. Utilizatori ", [&]{ tab_index = 4; }, ButtonOption::Animated());

    // 2. Tab 1: Adaugare
    inp_t1_titlu = Input(&state.titlu_nou, "Titlu");
    inp_t1_autor = Input(&state.autor_nou, "Autor");
    inp_t1_loc = Input(&state.locatie_nou, "Locatie (ex: A-LitRom)");
    inp_t1_stoc = Input(&state.carte_an, "Stoc initial (ex: 5)");
    btn_t1_add = MakeSmallBtn(state.limba_selectata, "Adauga Carte", "Add", [&]{
        if(!state.titlu_nou.empty()) {
            int stoc = 1;
            try { if(!state.carte_an.empty()) stoc = std::stoi(state.carte_an); } catch(...) {}
            state.biblio.adaugaCarte(state.titlu_nou, state.autor_nou, "N/A", "N/A", "N/A", state.locatie_nou, stoc);
            state.mesaj_actiune = "Adaugat cu stoc " + std::to_string(stoc) + "!";
            state.titlu_nou = ""; state.autor_nou = ""; state.locatie_nou = ""; state.carte_an = "";
        }
    });
    tab1_container = Container::Vertical({inp_t1_titlu, inp_t1_autor, inp_t1_loc, inp_t1_stoc, btn_t1_add});

    // 3. Tab 2: Inventar
    tab2_container = Container::Vertical({});

    // 4. Tab 3: Imprumuturi
    inp_t3_id = Input(&state.id_carte_str, "ID Carte");
    btn_t3_return = MakeSmallBtn(state.limba_selectata, "Fortare Returnare", "Return", [&]{
        if(!state.id_carte_str.empty()) { 
            if(state.biblio.returneazaFortat(std::stoi(state.id_carte_str))) state.mesaj_actiune = "Returnata fortat!"; 
            else state.mesaj_actiune = "Eroare! ID invalid.";
            state.id_carte_str = ""; 
        }
    });
    btn_t3_extend = MakeSmallBtn(state.limba_selectata, "Prelungeste", "Extend", [&]{
        if(!state.id_carte_str.empty()) { 
            if(state.biblio.prelungesteImprumut(std::stoi(state.id_carte_str))) state.mesaj_actiune = "Prelungit!";
            state.id_carte_str = ""; 
        }
    });
    tab3_container = Container::Vertical({inp_t3_id, btn_t3_return, btn_t3_extend});

    // 5. Tab 4: Receptie
    inp_t4_titlu = Input(&state.titlu_nou, "Titlu Marfa");
    inp_t4_autor = Input(&state.autor_nou, "Autor");
    inp_t4_cantitate = Input(&state.carte_an, "Nr. Exemplare");
    btn_t4_receptie = MakeSmallBtn(state.limba_selectata, "Receptioneaza Marfa", "Receive", [&]{
        if(!state.titlu_nou.empty() && !state.carte_an.empty()) {
            int cantitate = std::stoi(state.carte_an);
            state.biblio.adaugaCarte(state.titlu_nou, state.autor_nou, "N/A", "N/A", "N/A", "Depozit", cantitate);
            state.mesaj_actiune = "Au fost receptionate " + std::to_string(cantitate) + " exemplare!";
            state.titlu_nou = ""; state.autor_nou = ""; state.carte_an = "";
        }
    });
    tab4_container = Container::Vertical({inp_t4_titlu, inp_t4_autor, inp_t4_cantitate, btn_t4_receptie});

    // 6. Tab 5: Utilizatori & Statistici (INIȚIALIZARE SIGURĂ)
    tab5_container = Container::Vertical({});

    // 7. Logout
    btn_back = MakeSmallBtn(state.limba_selectata, "Logout", "Logout", [&]{ 
        state.ecran_activ = 0; state.user_curent = ""; state.mesaj_actiune = ""; 
    });

    // 8. Asamblare Tab-uri Selector
    std::vector<Component> tabs_content = {tab1_container, tab2_container, tab3_container, tab4_container, tab5_container};
    tab_selector = Container::Tab(tabs_content, &tab_index);

    // 9. Asamblare Container Principal (ORDINEA ESTE CRUCIALĂ)
    std::vector<Component> all_comps;
    all_comps.push_back(btn_tab_1); 
    all_comps.push_back(btn_tab_2);
    all_comps.push_back(btn_tab_3); 
    all_comps.push_back(btn_tab_4);
    all_comps.push_back(btn_tab_5); // Butonul este inițializat mai sus, deci e sigur
    all_comps.push_back(tab_selector);
    all_comps.push_back(btn_back);
    
    container = Container::Vertical(all_comps);
}

Element BiblioScreen::Render() {
    // Antet cu butoanele de Tab
    Elements tabs_row;
    tabs_row.push_back(btn_tab_1->Render() | (tab_index == 0 ? color(Color::Green) : dim));
    tabs_row.push_back(btn_tab_2->Render() | (tab_index == 1 ? color(Color::Blue) : dim));
    tabs_row.push_back(btn_tab_3->Render() | (tab_index == 2 ? color(Color::Red) : dim));
    tabs_row.push_back(btn_tab_4->Render() | (tab_index == 3 ? color(Color::Magenta) : dim));
    tabs_row.push_back(btn_tab_5->Render() | (tab_index == 4 ? color(Color::Cyan) : dim));
    Element header = hbox(tabs_row) | border;

    Element content;
    
    if (tab_index == 0) {
        Elements form;
        form.push_back(text("Adaugare Carte Noua") | bold);
        form.push_back(inp_t1_titlu->Render() | border);
        form.push_back(inp_t1_autor->Render() | border);
        form.push_back(inp_t1_loc->Render() | border);
        form.push_back(inp_t1_stoc->Render() | border);
        form.push_back(btn_t1_add->Render() | center);
        content = vbox(form);
    } 
    else if (tab_index == 1) {
        Elements inv;
        auto carti = state.biblio.getToateCartile();
        inv.push_back(text("Total titluri in inventar: " + std::to_string(carti.size())) | bold | color(Color::Cyan));
        inv.push_back(separator());
        for(auto& c : carti) inv.push_back(text("  " + c));
        if(carti.empty()) inv.push_back(text("  Inventar gol.") | dim);
        content = vbox(inv) | flex;
    } 
    else if (tab_index == 2) {
        Elements loans;
        auto imprumutate = state.biblio.getToateImprumuturile();
        loans.push_back(text("Carti imprumutate activ: " + std::to_string(imprumutate.size())) | bold | color(Color::Red));
        for(auto& c : imprumutate) loans.push_back(text("  ⚠️ " + c) | color(Color::Yellow));
        if(imprumutate.empty()) loans.push_back(text("  Nicio carte imprumutata momentan.") | dim);
        loans.push_back(separator());
        loans.push_back(text("Actiuni Administrative (dupa ID):") | bold);
        loans.push_back(inp_t3_id->Render() | border);
        loans.push_back(hbox({btn_t3_return->Render(), btn_t3_extend->Render()}));
        content = vbox(loans) | flex;
    }
    else if (tab_index == 3) {
        auto total = state.biblio.getToateCartile().size();
        auto active = state.biblio.getToateImprumuturile().size();

        Elements stats;
        stats.push_back(text("📦 RECEPTIE MARFA (Adaugare Multipla)") | bold | color(Color::Magenta));
        stats.push_back(text("Foloseste pentru a adauga rapid mai multe exemplare din aceeasi carte."));
        stats.push_back(separator());
        stats.push_back(inp_t4_titlu->Render() | border);
        stats.push_back(inp_t4_autor->Render() | border);
        stats.push_back(inp_t4_cantitate->Render() | border);
        stats.push_back(btn_t4_receptie->Render() | center);
        stats.push_back(separator());
        stats.push_back(text("--- STATISTICI RAPIDE ---") | bold);
        stats.push_back(text("Total titluri in sistem: " + std::to_string(total)));
        stats.push_back(text("Imprumuturi active: " + std::to_string(active)));
        content = vbox(stats);
    }
    else if (tab_index == 4) {
        Elements stats_ui;
        stats_ui.push_back(text("📊 STATISTICI SISTEM") | bold | color(Color::Cyan));
        
        auto stats = state.biblio.getStatisticiSistem();
        for(auto& s : stats) {
            stats_ui.push_back(text("  " + s));
        }
        
        stats_ui.push_back(separator());
        stats_ui.push_back(text("👥 LISTA UTILIZATORI INREGISTRATI") | bold | color(Color::Magenta));
        
        auto utilizatori = state.biblio.getTotiUtilizatorii();
        for(auto& u : utilizatori) {
            stats_ui.push_back(text("  " + u));
        }
        if(utilizatori.empty()) stats_ui.push_back(text("  Niciun utilizator inregistrat.") | dim);

        content = vbox(stats_ui) | flex;
    }

    Elements final_content;
    final_content.push_back(header);
    final_content.push_back(content | flex);
    final_content.push_back(text(state.mesaj_actiune) | color(Color::Yellow) | center | border);
    final_content.push_back(btn_back->Render() | center);
    
    return window(text("  BIBLIOTECAR DASHBOARD "), vbox(final_content)) | flex;
}