#include <iostream>
#include "Biblioteca.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

int main() {
    Biblioteca biblio;
    auto screen = ScreenInteractive::TerminalOutput();

    // ====== STATE (Starea aplicației) ======
    int meniu_selectat = 0;
    std::vector<std::string> optiuni_meniu = {
        "1. Gestiune Carti",
        "2. Gestiune Cititori",
        "3. Imprumuta o Carte",
        "4. Returneaza o Carte",
        "5. Penalizari / Intarzieri",
        "6. Iesire"
    };

    // Variabile pentru formular adaugare carte
    std::string input_titlu;
    std::string input_autor;
    std::string input_locatie;
    int sursa_selectata = 0;
    std::vector<std::string> optiuni_sursa = {"Achizitie Noua", "Donatie", "Anticariat"};
    std::string status_msg = "";

    // ====== COMPONENTE UI ======
    auto meniu_stanga = Menu(&optiuni_meniu, &meniu_selectat);

    auto inp_titlu = Input(&input_titlu, "Introdu titlul...");
    auto inp_autor = Input(&input_autor, "Introdu autorul...");
    auto inp_locatie = Input(&input_locatie, "Ex: Raionul A1");
    auto radio_sursa = Radiobox(&optiuni_sursa, &sursa_selectata);

    auto btn_salveaza = Button("Salveaza in DB", [&]() {
        if(!input_titlu.empty() && !input_autor.empty()) {
            biblio.adaugaCarteFizica(input_titlu, input_autor, optiuni_sursa[sursa_selectata], input_locatie);
            status_msg = "Salvat cu succes!";
            input_titlu = ""; input_autor = ""; input_locatie = ""; // Reset
        } else {
            status_msg = "Eroare: Completati titlul si autorul!";
        }
    });

    auto formular_adaugare = Container::Vertical({
        inp_titlu, inp_autor, inp_locatie, radio_sursa, btn_salveaza
    });

    // Punem totul într-un container principal
    auto container_principal = Container::Horizontal({
        meniu_stanga,
        formular_adaugare
    });

    // ====== RANDAREA VIZUALA ======
    auto renderer = Renderer(container_principal, [&] {
        // Logică pentru a schimba ce apare în dreapta pe baza meniului selectat
        Element continut_dreapta;

        if (meniu_selectat == 0) { // 1. Gestiune Cărți
            auto carti = biblio.getToateCartile();
            Elements lista_ui;
            for(const auto& c : carti) {
                lista_ui.push_back(text(" - " + c->getDetaliiComplete()));
            }
            if(lista_ui.empty()) lista_ui.push_back(text("Baza de date este goala.") | dim);

            continut_dreapta = vbox({
                text(" ADAUGARE CARTE NOUA ") | bold | color(Color::Green),
                separator(),
                hbox(text(" Titlu:   "), inp_titlu->Render() | border),
                hbox(text(" Autor:   "), inp_autor->Render() | border),
                hbox(text(" Locatie: "), inp_locatie->Render() | border),
                text(" Provenienta:") | bold, radio_sursa->Render(),
                btn_salveaza->Render() | center,
                text(status_msg) | center | color(Color::Yellow),
                separator(),
                text(" STOC CURENT IN BAZA DE DATE ") | bold | color(Color::Cyan),
                vbox(move(lista_ui)) | border
            });
        } 
        else if (meniu_selectat == 5) { // 6. Iesire
            screen.ExitLoopClosure()();
            continut_dreapta = text("Se inchide aplicatia...");
        } 
        else {
            // Placeholder pentru restul meniurilor
            continut_dreapta = vbox({
                text(" " + optiuni_meniu[meniu_selectat] + " ") | bold | color(Color::Blue),
                separator(),
                text("Acest modul este in curs de dezvoltare pentru urmatorul pas.") | dim
            });
        }

        // Asamblarea finală a ferestrei
        return window(text(" MANAGEMENT BIBLIOTECA PRO "),
            hbox({
                vbox({ meniu_stanga->Render() }) | border | size(WIDTH, GREATER_THAN, 25),
                continut_dreapta | border | flex
            })
        );
    });

    // Pornim bucla aplicației
    screen.Loop(renderer);

    return 0;
}