#include "BiblioAI.h"
#include "Theme.h"
#include <chrono>

using namespace ftxui;

BiblioAI::BiblioAI(AppState& s) : state(s) {
    inp_ai = Input(&state.ai_prompt, "Scrie mesajul tău...");
    
    btn_trimite = MakeSmallBtn(state.limba_selectata, "Trimite", "Send", [&]{
        HandleSend();
    });
    
    btn_toggle = MakeSmallBtn(state.limba_selectata, "🤖 BiblioAI", "🤖 BiblioAI", [&]{
        Toggle();
        if(state.mod_ai_activ && state.ai_raspuns.empty()) {
            state.ai_raspuns = "Salut! Sunt BiblioAI. Cu ce te pot ajuta? 📚";
            state.ai_raspuns_afisat = "";
            state.ai_index_curent = 0;
            state.ai_scrie = true;
            state.ai_timp_ultimul_cadru = std::chrono::steady_clock::now();
        }
    });
    
    container = Container::Horizontal({inp_ai, btn_trimite});
    
    // FIX CRITIC: Folosim un Tab intern pentru a activa/dezactiva AI-ul în arborele de componente
    // Când ai_tab_index = 0, containerul activ este gol (nu fură focusul).
    // Când ai_tab_index = 1, containerul activ este cel cu Input (poate primi focus).
    std::vector<Component> ai_tabs;
    ai_tabs.push_back(Container::Vertical({})); 
    ai_tabs.push_back(container);               
    ai_switcher = Container::Tab(ai_tabs, &ai_tab_index);
}

void BiblioAI::HandleSend() {
    if(!state.ai_prompt.empty() && !state.ai_scrie) {
        std::string intrebare = state.ai_prompt;
        state.ai_prompt = "";
        state.ai_raspuns_afisat = "BiblioAI scrie...";
        state.ai_scrie = false;
        
        state.ai_raspuns = state.biblio.intreabaBiblioAI(intrebare);
        
        state.ai_raspuns_afisat = "";
        state.ai_index_curent = 0;
        state.ai_scrie = true;
        state.ai_timp_ultimul_cadru = std::chrono::steady_clock::now();
    }
}

void BiblioAI::UpdateTyping() {
    if (state.ai_scrie && !state.ai_raspuns.empty()) {
        auto acum = std::chrono::steady_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
            acum - state.ai_timp_ultimul_cadru
        ).count();
        
        if (diff > 30 && state.ai_index_curent < state.ai_raspuns.length()) {
            state.ai_index_curent++;
            state.ai_raspuns_afisat = state.ai_raspuns.substr(0, state.ai_index_curent);
            state.ai_timp_ultimul_cadru = acum;
        }
        
        if (state.ai_index_curent >= state.ai_raspuns.length()) {
            state.ai_scrie = false;
            state.ai_raspuns_afisat = state.ai_raspuns;
        }
    }
}

Element BiblioAI::Render() {

    if (state.ecran_activ == 0) {
        return text("");
    }

    UpdateTyping();
    
    if (!state.mod_ai_activ) {
        return hbox({filler(), btn_toggle->Render() | bold | color(Color::White) | bgcolor(Color::Blue)});
    }
    
    Elements ai_chat;
    
    if (!state.ai_raspuns_afisat.empty()) {
        Elements avatar_row;
        avatar_row.push_back(text("🤖") | bgcolor(Color::Blue) | color(Color::White) | bold);
        avatar_row.push_back(text(" BiblioAI ") | bold | color(Color::Blue));
        ai_chat.push_back(hbox(avatar_row));
        
        Element text_content = text(state.ai_raspuns_afisat) | color(Color::White);
        if (state.ai_scrie) {
            text_content = hbox({text_content, text(" ▌") | blink | color(Color::Cyan)});
        }
        ai_chat.push_back(text_content | bgcolor(Color::Blue) | border);
        ai_chat.push_back(separatorEmpty());
    }
    
    Elements input_row;
    input_row.push_back(text("➤ ") | bold | color(Color::Cyan));
    input_row.push_back(inp_ai->Render() | flex | border);
    input_row.push_back(btn_trimite->Render());
    
    if (state.ai_scrie) {
        input_row.push_back(text(" (scrie...)") | dim | color(Color::Yellow));
    }
    
    ai_chat.push_back(hbox(input_row));
    
    return window(
        text(" 💬 BIBLIOAI CHAT ") | bold | color(Color::Magenta), 
        vbox(ai_chat) | flex
    ) | size(HEIGHT, EQUAL, 12) | size(WIDTH, EQUAL, 60);
}