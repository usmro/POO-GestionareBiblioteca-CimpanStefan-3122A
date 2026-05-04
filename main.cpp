#include <iostream>
#include "Biblioteca.h"

using namespace std;

int main() {
    Biblioteca b;
    int optiune;

    do {
        cout << "\n=== MENIU ===\n";
        cout << "1. Adauga carte\n";
        cout << "2. Afiseaza carti\n";
        cout << "3. Salveaza in fisier\n";
        cout << "4. Incarca din fisier\n";
        cout << "5. Cauta carte\n";
        cout << "6. Editeaza carte\n";
        cout << "7. Sterge ultima carte\n";
        cout << "0. Iesire\n";
        cout << "Optiune: ";
        cin >> optiune;

        switch (optiune) {
            case 1:
                b.adaugaCarte();
                break;
            case 2:
                b.afiseazaCarti();
                break;
            case 3:
                b.salveazaInFisier();
                cout << "Salvat!\n";
                break;
            case 4:
                b.incarcaDinFisier();
                cout << "Incarcat!\n";
                break;
            case 5:
                b.cautaCarte();
                break;
            case 6:
                b.editeazaCarte();
                break;
            case 7:
                b.stergeCarte();
                break;
        }

    } while (optiune != 0);

    return 0;
}
