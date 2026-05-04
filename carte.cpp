#include "Carte.h"
#include <iostream>
using namespace std;

// Item
Item::Item() {}

Item::Item(int id, string titlu) {
    this->id = id;
    this->titlu = titlu;
}

void Item::afisare() {}

// Carte
Carte::Carte() {}

Carte::Carte(int id, string titlu, string autor)
    : Item(id, titlu) {
    this->autor = autor;
}

void Carte::afisare() {
    cout << "ID: " << id << " | Titlu: " << titlu << " | Autor: " << autor << endl;
}

string Carte::getTitlu() {
    return titlu;
}

string Carte::getAutor() {
    return autor;
}
