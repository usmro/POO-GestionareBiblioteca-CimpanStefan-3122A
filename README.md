📚 BiblioManager Pro
Sistem modern de gestiune a bibliotecii cu interfață terminal, bază de date SQLite și asistent AI





📖 Despre Proiect
BiblioManager Pro este o aplicație în C++ care simulează funcționarea unei biblioteci reale. Permite gestionarea cărților, utilizatorilor și împrumuturilor printr-o interfață modernă în terminal.
Proiectul a fost dezvoltat pentru cursul de Programare Orientată pe Obiecte și demonstrează concepte precum:
Clase și ierarhii
Baze de date SQLite
Securitate (hashing SHA-256)
Integrare API-uri externe (Google Gemini, Open Library)
Interfață grafică în terminal (FTXUI)
✨ Funcționalități Principale
👤 Pentru Utilizatori
🔐 Creare cont și autentificare securizată
🔍 Căutare cărți după titlu sau autor
📥 Împrumut și returnare cărți
⏰ Prelungire termen împrumut
📋 Vizualizare cărți împrumutate personal
🌍 Descoperire tendințe globale (Open Library)
📚 Pentru Bibliotecari
➕ Adăugare cărți în inventar
📦 Recepție marfă (adaugare multiplă exemplare)
📊 Gestionare împrumuturi active
🔄 Forțare returnare cărți
📈 Statistici în timp real
🤖 Funcții Avansate
BiblioAI - Asistent virtual powered by Google Gemini
Căutare globală - Peste 20 milioane de cărți din Open Library
Securitate - Parole hash-uite cu SHA-256
Persistență - Date salvate în SQLite
Multi-rol - Guest / User / Bibliotecar / Admin
🛠️ Tehnologii
Tehnologie
Descriere
C++17
Limbaj principal
FTXUI
Interfață grafică terminal
SQLite3
Bază de date locală
CPR
Client HTTP (cURL wrapper)
OpenSSL
Criptare SHA-256
nlohmann/json
Parsare JSON
CMake
Sistem de build
📦 Instalare
1. Cerințe de sistem
bash
123
2. Clonare și compilare
bash
12345
3. Populare bază de date (opțional)
bash
1
4. Configurare AI (opțional)
Creează fișierul .env în folderul build/:
1
🚀 Utilizare
Pornire aplicație
bash
12
Conturi implicite
Username
Parolă
Rol
admin
admin123
Administrator
biblio
1234
Bibliotecar
Navigare în interfață
TAB → Mută focusul între elemente
ENTER → Selectează / Apasă buton
↑ ↓ → Derulează liste
ESC → Ieșire din ecrane modale
📁 Structura Proiectului
1234567891011121314151617181920212223242526272829
🗄️ Baza de Date
Aplicația folosește SQLite cu 3 tabele principale:
Tabela Carti
Stochează informațiile despre cărți cu stocul disponibil.
Tabela Cititori
Conturi utilizatori cu parole hash-uite SHA-256.
Tabela Imprumuturi
Înregistrează fiecare împrumut cu dată și status.
Organizare pe raioane
A-LitRom → Literatură Română
B-LitUniv → Literatură Universală
C-SFF → Science Fiction & Fantasy
D-IstBio → Istorie & Biografii
E-PsiFil → Psihologie & Filosofie
F-CopiiYA → Copii & Young Adult
🔒 Securitate
✅ Parole hash-uite cu SHA-256 (OpenSSL)
✅ Protecție la conturi duplicate (UNIQUE constraint)
✅ Parameterized queries (anti SQL Injection)
✅ Fișier .env pentru chei API (ignorat de Git)
✅ Foreign keys pentru integritate date
🎯 Concepte POO Demonstrate
Encapsulare → Atribute private cu getteri/setteri
Moștenire → Ierarhii de clase pentru ecrane
Polimorfism → Metode virtuale pentru renderizare
Clase generice → Template-uri pentru containere
Gestionare memorie → Smart pointers FTXUI
Excepții → Try-catch pentru input invalid
