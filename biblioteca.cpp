#include "Biblioteca.h"
#include <iostream>
#include <fstream>
using namespace std;

void Biblioteca::adaugaCarte() {
    int id;
    string titlu, autor;

    cout << "ID: ";
    cin >> id;
    cin.ignore();

    cout << "Titlu: ";
    getline(cin, titlu);

    cout << "Autor: ";
    getline(cin, autor);

    carti.push_back(Carte(id, titlu, autor));
}

void Biblioteca::afiseazaCarti() {
    for (auto &c : carti) {
        c.afisare();
    }
}

void Biblioteca::salveazaInFisier() {
    ofstream fout("carti.txt");

    for (auto &c : carti) {
        fout << c.getTitlu() << " - " << c.getAutor() << endl;
    }

    fout.close();
}
