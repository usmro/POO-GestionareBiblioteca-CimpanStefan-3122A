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
        fout << c.getTitlu() << "-" << c.getAutor() << endl;
    }

    fout.close();
}

// 🔽 NOI

void Biblioteca::incarcaDinFisier() {
    ifstream fin("carti.txt");
    string titlu, autor;
    int id = 1;

    while (getline(fin, titlu, '-') && getline(fin, autor)) {
        carti.push_back(Carte(id++, titlu, autor));
    }

    fin.close();
}

void Biblioteca::cautaCarte() {
    string cautare;
    cin.ignore();

    cout << "Introdu titlu cautat: ";
    getline(cin, cautare);

    for (auto &c : carti) {
        if (c.getTitlu().find(cautare) != string::npos) {
            c.afisare();
        }
    }
}

void Biblioteca::editeazaCarte() {
    int id;
    cout << "ID carte de editat: ";
    cin >> id;
    cin.ignore();

    string titluNou, autorNou;

    cout << "Titlu nou: ";
    getline(cin, titluNou);

    cout << "Autor nou: ";
    getline(cin, autorNou);

    for (auto &c : carti) {
        c = Carte(id, titluNou, autorNou);
        break;
    }
}

void Biblioteca::stergeCarte() {
    if (!carti.empty()) {
        carti.pop_back();
    }
}
