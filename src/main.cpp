#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <functional>
#include "Biblioteca.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

// Funcție pentru efectul de typing - literele apar una câte una
std::string getTypingText(const std::string& fullText, size_t currentIndex) {
    if (currentIndex >= fullText.length()) return fullText;
    return fullText.substr(0, currentIndex);
}

int main() {
    Biblioteca biblio;
    auto screen = ScreenInteractive::Fullscreen();

    int ecran_activ = 0; 
    std::string user_curent = "";      // Ține minte cine e logat
    std::string mesaj_actiune = "";    // Mesaje de succes/eroare pentru User/Biblio

    // Sistem de Limbă
    int limba_selectata = 0;
    std::vector<std::string> optiuni_limba;
    optiuni_limba.push_back("RO");
    optiuni_limba.push_back("EN");
    auto toggle_lang = Toggle(&optiuni_limba, &limba_selectata);

    auto L = [&](const std::string& ro, const std::string& en) -> std::string {
        return (limba_selectata == 1) ? en : ro;
    };
    auto elem_txt = [&](const std::string& ro, const std::string& en) {
        return text((limba_selectata == 1) ? en : ro);
    };

    // ----------------------------------------------------
    // BUTOANE (Acum au marimi egale - WIDTH EQUAL)
    // ----------------------------------------------------
    auto MakeBigBtn = [&limba_selectata](std::string ro, std::string en, std::function<void()> on_click) {
        ButtonOption option = ButtonOption::Animated();
        option.transform = [&limba_selectata, ro, en](const EntryState& s) {
            bool is_en = (limba_selectata == 1);
            Element t = text(is_en ? en : ro) | center; // Textul in centru
            if (s.focused) t = t | inverted | bold;
            
            Elements block;
            block.push_back(text(""));
            block.push_back(t);
            block.push_back(text(""));
            // Fortam o latime de 65 caractere pentru TOATE butoanele principale
            return vbox(block) | border | size(WIDTH, EQUAL, 65) | center; 
        };
        return Button(ro, std::move(on_click), option);
    };

    auto MakeSmallBtn = [&limba_selectata](std::string ro, std::string en, std::function<void()> on_click) {
        ButtonOption option = ButtonOption::Animated();
        option.transform = [&limba_selectata, ro, en](const EntryState& s) {
            bool is_en = (limba_selectata == 1);
            Element t = text(is_en ? en : ro) | center;
            if (s.focused) t = t | inverted | bold;
            // Fortam o latime de 25 caractere pentru butoanele de login/register
            return t | border | size(WIDTH, EQUAL, 25) | center;
        };
        return Button(ro, std::move(on_click), option);
    };

    // ----------------------------------------------------
    // COMPONENTE UI - BIBLIOAI
    // ----------------------------------------------------
    
    // AI 
    bool mod_ai_activ = false;
    std::string ai_prompt = "";
    std::string ai_raspuns = "";
    std::string ai_raspuns_afisat = "";
    bool ai_scrie = false;
    size_t ai_index_curent = 0;
    std::chrono::steady_clock::time_point ai_timp_ultimul_cadru;


    auto btn_ai_deschide = MakeSmallBtn("🤖 BiblioAI", "🤖 BiblioAI", [&] { 
        mod_ai_activ = !mod_ai_activ; 
        if(mod_ai_activ && ai_raspuns.empty()) {
            // Primul mesaj de bun venit
            ai_raspuns = "Salut! Sunt BiblioAI, asistentul tău virtual. Cu ce te pot ajuta astăzi? 📚";
            ai_raspuns_afisat = "";
            ai_index_curent = 0;
            ai_scrie = true;
            ai_timp_ultimul_cadru = std::chrono::steady_clock::now();
        }
    });
    auto inp_ai = Input(&ai_prompt, "Scrie mesajul tău...");
    auto btn_ai_trimite = MakeSmallBtn("Trimite", "Send", [&] {
        if(!ai_prompt.empty() && !ai_scrie) {
            // Adaugă întrebarea userului în istoric (opțional, poți extinde)
            std::string intrebare = ai_prompt;
            ai_prompt = "";
            
            // Afișează "BiblioAI scrie..."
            ai_raspuns_afisat = "BiblioAI scrie";
            ai_scrie = false;
            
            // Call către API Gemini (async în background)
            ai_raspuns = biblio.intreabaBiblioAI(intrebare);
            
            // Pornește animația de typing pentru răspuns
            ai_raspuns_afisat = "";
            ai_index_curent = 0;
            ai_scrie = true;
            ai_timp_ultimul_cadru = std::chrono::steady_clock::now();
        }
    });

    std::vector<Component> ai_comps;
    ai_comps.push_back(inp_ai); 
    ai_comps.push_back(btn_ai_trimite);
    auto container_ai = Container::Horizontal(ai_comps);

    // ECRAN 0: MENIU PRINCIPAL
    auto btn_start_login    = MakeBigBtn("1. Intra in cont", "1. Login", [&]{ ecran_activ = 1; });
    auto btn_start_register = MakeBigBtn("2. Creare cont", "2. Create Account", [&]{ ecran_activ = 6; });
    auto btn_start_guest    = MakeBigBtn("3. Intra ca vizitator (cu acces limitat)", "3. Continue as Guest (limited access)", [&]{ ecran_activ = 2; });
    auto btn_start_exit     = MakeBigBtn("4. Iesire", "4. Exit", [&]{ screen.ExitLoopClosure()(); });
    
    std::vector<Component> s_comps;
    s_comps.push_back(btn_start_login); 
    s_comps.push_back(btn_start_register); 
    s_comps.push_back(btn_start_guest); 
    s_comps.push_back(btn_start_exit);
    auto ecran_start = Container::Vertical(s_comps);

    // ECRAN 1: LOGIN
    std::string inp_user_str = "", inp_pass_str = "", mesaj_eroare = "";
    auto inp_user = Input(&inp_user_str, "Username");
    InputOption opt_pass; opt_pass.password = true;
    auto inp_pass = Input(&inp_pass_str, "Password", opt_pass);
    
    auto btn_login_exec = MakeSmallBtn("Intra", "Enter", [&]{
        if(inp_user_str == "admin" && inp_pass_str == "admin123") { ecran_activ = 5; user_curent = "admin"; }
        else if(inp_user_str == "biblio" && inp_pass_str == "biblio123") { ecran_activ = 4; user_curent = "biblio"; }
        else if(!inp_user_str.empty()) { 
            ecran_activ = 3; 
            user_curent = inp_user_str; 
            biblio.adaugaCititor(user_curent); // Îl salvăm în DB dacă e user nou
        }
        else mesaj_eroare = (limba_selectata == 1) ? "Invalid Credentials!" : "Date Invalide!";
        inp_user_str = ""; inp_pass_str = "";
    });
    auto btn_login_back = MakeSmallBtn("Inapoi", "Back", [&]{ ecran_activ = 0; mesaj_eroare = ""; });

    std::vector<Component> log_comps;
    log_comps.push_back(inp_user); log_comps.push_back(inp_pass);
    log_comps.push_back(btn_login_exec); log_comps.push_back(btn_login_back);
    auto ecran_login = Container::Vertical(log_comps);

    // ECRAN 6: CREARE CONT
    std::string reg_user = "", reg_pass = "", mesaj_reg = "";
    auto inp_reg_user = Input(&reg_user, "Username nou");
    auto inp_reg_pass = Input(&reg_pass, "Parola noua", opt_pass);
    auto btn_reg_exec = MakeSmallBtn("Inregistrare", "Sign Up", [&]{
        if(!reg_user.empty() && !reg_pass.empty()) {
            biblio.adaugaCititor(reg_user); // <--- SALVĂM USERUL ÎN BAZA DE DATE
            mesaj_reg = (limba_selectata == 1) ? "Account created successfully!" : "Cont creat cu succes!";
            reg_user = ""; reg_pass = "";
        } else {
            mesaj_reg = (limba_selectata == 1) ? "Please fill all fields." : "Completati toate campurile.";
        }
    });
    auto btn_reg_back = MakeSmallBtn("Inapoi", "Back", [&]{ ecran_activ = 0; mesaj_reg = ""; });
    
    std::vector<Component> reg_comps;
    reg_comps.push_back(inp_reg_user); reg_comps.push_back(inp_reg_pass);
    reg_comps.push_back(btn_reg_exec); reg_comps.push_back(btn_reg_back);
    auto ecran_register = Container::Vertical(reg_comps);

    // ECRANE INTERNE 
    auto btn_back_g = MakeSmallBtn("Exit", "Exit", [&]{ ecran_activ = 0; });
    std::vector<Component> cg; cg.push_back(btn_back_g); auto ecran_guest = Container::Vertical(cg);

    auto btn_back_u = MakeSmallBtn("Logout", "Logout", [&]{ ecran_activ = 0; });
    std::vector<Component> cu; cu.push_back(btn_back_u); auto ecran_user = Container::Vertical(cu);

    auto btn_back_b = MakeSmallBtn("Logout", "Logout", [&]{ ecran_activ = 0; });
    std::vector<Component> cb; cb.push_back(btn_back_b); auto ecran_biblio = Container::Vertical(cb);

    auto btn_back_a = MakeSmallBtn("Logout", "Logout", [&]{ ecran_activ = 0; });
    std::vector<Component> ca; ca.push_back(btn_back_a); auto ecran_admin = Container::Vertical(ca);

    // RUTARE GLOBALĂ
    std::vector<Component> tab_comps;
    tab_comps.push_back(ecran_start); tab_comps.push_back(ecran_login); tab_comps.push_back(ecran_guest);
    tab_comps.push_back(ecran_user); tab_comps.push_back(ecran_biblio); tab_comps.push_back(ecran_admin);
    tab_comps.push_back(ecran_register);
    auto sistem_rutare = Container::Tab(tab_comps, &ecran_activ);

    std::vector<Component> glob_comps;
    glob_comps.push_back(toggle_lang); glob_comps.push_back(sistem_rutare); 
    glob_comps.push_back(btn_ai_deschide); glob_comps.push_back(container_ai);
    auto aplicatie_completa = Container::Vertical(glob_comps);

    // ----------------------------------------------------
    // DESIGN VIZUAL 
    // ----------------------------------------------------
    auto renderer = Renderer(aplicatie_completa, [&] {
        
        Elements h_elem;
        h_elem.push_back(text(" NEXUS | Library OS ") | bold | color(Color::Black) | bgcolor(Color::CyanLight));
        h_elem.push_back(filler());
        h_elem.push_back(toggle_lang->Render());
        auto ui_header = hbox(h_elem);

        Element ui_content;
        
                if (ecran_activ == 0) { // START
            Elements col;
            col.push_back(filler());
            col.push_back(btn_start_login->Render());
            col.push_back(btn_start_register->Render());
            col.push_back(btn_start_guest->Render());
            col.push_back(btn_start_exit->Render());
            col.push_back(filler());
            ui_content = vbox(col) | center | flex;
        } 
        else if (ecran_activ == 1) { // LOGIN
            Elements row1, row2, form_col;
            row1.push_back(elem_txt(" Utilizator: ", " Username:   ") | bold); 
            row1.push_back(inp_user->Render() | border | size(WIDTH, EQUAL, 30)); 
            
            row2.push_back(elem_txt(" Parola:     ", " Password:   ") | bold); 
            row2.push_back(inp_pass->Render() | border | size(WIDTH, EQUAL, 30));
            
            form_col.push_back(hbox(row1) | center);
            form_col.push_back(hbox(row2) | center);
            form_col.push_back(separatorEmpty());
            form_col.push_back(btn_login_exec->Render() | color(Color::Green));
            form_col.push_back(btn_login_back->Render() | dim);
            form_col.push_back(text(mesaj_eroare) | color(Color::Red) | bold | center);
            
            auto login_card = window(elem_txt(" INTRA IN CONT ", " LOG IN ") | bold | center, vbox(form_col));
            
            Elements main_col;
            main_col.push_back(filler());
            main_col.push_back(login_card | center);
            main_col.push_back(filler());
            ui_content = vbox(main_col) | flex;
        }
        else if (ecran_activ == 6) { // REGISTER
            Elements row1, row2, form_col;
            row1.push_back(elem_txt(" Utilizator nou: ", " New Username: ") | bold); 
            row1.push_back(inp_reg_user->Render() | border | size(WIDTH, EQUAL, 30));
            
            row2.push_back(elem_txt(" Parola noua:    ", " New Password: ") | bold); 
            row2.push_back(inp_reg_pass->Render() | border | size(WIDTH, EQUAL, 30));
            
            form_col.push_back(hbox(row1) | center);
            form_col.push_back(hbox(row2) | center);
            form_col.push_back(separatorEmpty());
            form_col.push_back(btn_reg_exec->Render() | color(Color::Cyan));
            form_col.push_back(btn_reg_back->Render() | dim);
            form_col.push_back(text(mesaj_reg) | color(Color::Yellow) | bold | center);
            
            auto reg_card = window(elem_txt(" CREARE CONT NOU ", " SIGN UP ") | bold | center, vbox(form_col));
            
            Elements main_col;
            main_col.push_back(filler());
            main_col.push_back(reg_card | center);
            main_col.push_back(filler());
            ui_content = vbox(main_col) | flex;
        }
        else if (ecran_activ == 2) { // GUEST
            Elements viz_col, lista_c; 
            for(auto& c : biblio.getCartiFormatate()) lista_c.push_back(text("  " + c) | color(Color::White));
            if(lista_c.empty()) lista_c.push_back(text(L("Nicio carte gasita.", "No books found.")) | dim);

            viz_col.push_back(elem_txt("⚠️ Mod Invitat - Imprumutul dezactivat", "⚠️ Guest Mode - Borrowing disabled") | bold | color(Color::Black) | bgcolor(Color::Yellow) | center);
            viz_col.push_back(separator());
            viz_col.push_back(vbox(lista_c) | border | flex);
            viz_col.push_back(btn_back_g->Render() | center);
            ui_content = window(elem_txt(" CATALOG ", " CATALOG "), vbox(viz_col)) | flex;
        }
                else if (ecran_activ == 3) { // USER / CITITOR
            std::string id_carte_str = "";
            auto inp_id = Input(&id_carte_str, "ID Carte");
            
            auto btn_imprumuta = MakeSmallBtn("Imprumuta", "Borrow", [&]{
                try {
                    if(!id_carte_str.empty()) {
                        if(biblio.imprumutaCarte(std::stoi(id_carte_str), user_curent)) {
                            mesaj_actiune = L("Carte imprumutata cu succes!", "Book borrowed successfully!");
                        } else {
                            mesaj_actiune = L("Eroare: Carte indisponibila sau ID invalid.", "Error: Book unavailable or invalid ID.");
                        }
                        id_carte_str = "";
                    }
                } catch(...) { mesaj_actiune = L("ID invalid! Introdu doar numere.", "Invalid ID! Enter numbers only."); }
            });

            auto btn_returneaza = MakeSmallBtn("Returneaza", "Return", [&]{
                try {
                    if(!id_carte_str.empty()) {
                        if(biblio.returneazaCarte(std::stoi(id_carte_str))) {
                            mesaj_actiune = L("Carte returnata cu succes!", "Book returned successfully!");
                        } else {
                            mesaj_actiune = L("Eroare: Nu ai aceasta carte imprumutata.", "Error: You don't have this book borrowed.");
                        }
                        id_carte_str = "";
                    }
                } catch(...) { mesaj_actiune = L("ID invalid! Introdu doar numere.", "Invalid ID! Enter numbers only."); }
            });

            Elements u_col, lista_c;
            for(auto& c : biblio.getCartiFormatate()) lista_c.push_back(text(" 📖 " + c));
            
            // FIX: Am inlocuit | text(user_curent) cu hbox pentru a alipi cele doua texte
            u_col.push_back(hbox({
                elem_txt("Bine ai venit, ", "Welcome, ") | bold | color(Color::Cyan), 
                text(user_curent) | bold
            }));
            
            u_col.push_back(separator());
            u_col.push_back(vbox(lista_c) | border | size(HEIGHT, LESS_THAN, 8) | flex);
            u_col.push_back(hbox({inp_id->Render() | size(WIDTH, EQUAL, 15), btn_imprumuta->Render(), btn_returneaza->Render()}));
            u_col.push_back(text(mesaj_actiune) | color(Color::Yellow) | center);
            u_col.push_back(btn_back_u->Render() | center);
            ui_content = window(elem_txt(" PANOU CITITOR ", " READER PANEL "), vbox(u_col)) | flex;
        }
        else if (ecran_activ == 4) { // BIBLIOTECAR
            std::string titlu_nou = "", autor_nou = "", locatie_nou = "";
            auto inp_titlu = Input(&titlu_nou, "Titlu Carte");
            auto inp_autor = Input(&autor_nou, "Autor");
            auto inp_locatie = Input(&locatie_nou, "Locatie (ex: Raion A1)");
            
            auto btn_add = MakeSmallBtn("Adauga Carte", "Add Book", [&]{
                if(!titlu_nou.empty() && !autor_nou.empty()) {
                    biblio.adaugaCarteFizica(titlu_nou, autor_nou, "Noua", locatie_nou);
                    mesaj_actiune = L("Carte adaugata cu succes!", "Book added successfully!");
                    titlu_nou = ""; autor_nou = ""; locatie_nou = "";
                } else {
                    mesaj_actiune = L("Completeaza titlul si autorul!", "Fill title and author!");
                }
            });

            Elements b_col;
            b_col.push_back(elem_txt("Panou Bibliotecar - Adaugare Carti", "Librarian Panel - Add Books") | bold | color(Color::Green));
            b_col.push_back(separator());
            b_col.push_back(inp_titlu->Render() | border);
            b_col.push_back(inp_autor->Render() | border);
            b_col.push_back(inp_locatie->Render() | border);
            b_col.push_back(btn_add->Render() | center);
            b_col.push_back(text(mesaj_actiune) | color(Color::Yellow) | center);
            b_col.push_back(filler());
            b_col.push_back(btn_back_b->Render() | center);
            ui_content = window(elem_txt(" MANAGEMENT CARTI ", " BOOK MANAGEMENT "), vbox(b_col)) | flex;
        }
        else if (ecran_activ == 5) { // ADMIN
            Elements a_col, cititori_lista;
            for(auto& c : biblio.getCititori()) cititori_lista.push_back(text(" 👤 " + c));
            
            a_col.push_back(text(" ROOT ACCESS GRANTED ") | bold | inverted | color(Color::Red) | center);
            a_col.push_back(separator());
            a_col.push_back(elem_txt("Lista Cititori Inregistrati:", "Registered Readers List:") | bold);
            a_col.push_back(vbox(cititori_lista) | border | flex);
            a_col.push_back(filler());
            a_col.push_back(btn_back_a->Render() | center);
            ui_content = window(text(" ADMIN CONSOLE "), vbox(a_col)) | flex;
        }

                // PANOU BIBLIOAI - INTERFAȚĂ CHAT
        Element ui_ai = text("");
        if (ecran_activ >= 2 && ecran_activ != 6) { 
            if (mod_ai_activ) {
                // Animație de typing - actualizează indexul
                if (ai_scrie && !ai_raspuns.empty()) {
                    auto acum = std::chrono::steady_clock::now();
                    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(acum - ai_timp_ultimul_cadru).count();
                    
                    // Afișează o literă la fiecare 30ms (viteza de typing)
                    if (diff > 30 && ai_index_curent < ai_raspuns.length()) {
                        ai_index_curent++;
                        ai_raspuns_afisat = ai_raspuns.substr(0, ai_index_curent);
                        ai_timp_ultimul_cadru = acum;
                    }
                    
                    // S-a terminat de scris
                    if (ai_index_curent >= ai_raspuns.length()) {
                        ai_scrie = false;
                        ai_raspuns_afisat = ai_raspuns;
                    }
                }
                
                Elements ai_chat;
                
                // Mesajul de bun venit sau răspunsul AI cu avatar
                if (!ai_raspuns_afisat.empty()) {
                    Elements avatar_row;
                    avatar_row.push_back(text("🤖") | bgcolor(Color::Blue) | color(Color::White) | bold);
                    avatar_row.push_back(text(" BiblioAI ") | bold | color(Color::Blue));
                    ai_chat.push_back(hbox(avatar_row));
                    
                    // Balon de mesaj pentru AI
                    Element text_content = text(ai_raspuns_afisat) | color(Color::White);
                    if (ai_scrie) {
                        // Aici folosim hbox pentru a adăuga cursorul, nu operatorul |
                        text_content = hbox({text_content, text(" ▌") | blink | color(Color::Cyan)});
                    }
                    ai_chat.push_back(text_content | bgcolor(Color::Blue) | border);
                    ai_chat.push_back(separatorEmpty());
                }
                
                // Input row cu stil de chat
                Elements input_row;
                input_row.push_back(text("➤ ") | bold | color(Color::Cyan));
                input_row.push_back(inp_ai->Render() | flex | border);
                input_row.push_back(btn_ai_trimite->Render());
                
                if (ai_scrie) {
                    input_row.push_back(text(" (BiblioAI scrie...)") | dim | color(Color::Yellow));
                }
                
                ai_chat.push_back(hbox(input_row));
                
                // Panoul principal AI cu aspect de chat
                ui_ai = window(
                    elem_txt(" 💬 BIBLIOAI CHAT ", " 💬 BIBLIOAI CHAT ") | bold | color(Color::Magenta), 
                    vbox(ai_chat) | flex
                ) | size(HEIGHT, EQUAL, 12) | size(WIDTH, EQUAL, 60);
            } else {
                // Buton minimizat (am corectat culoarea din BlueDark in Blue)
                Elements ai_min;
                ai_min.push_back(filler());
                ai_min.push_back(btn_ai_deschide->Render() | bold | color(Color::White) | bgcolor(Color::Blue));
                ui_ai = hbox(ai_min);
            }
        }

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