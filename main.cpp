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
        }

    } while (optiune != 0);

    return 0;
}
