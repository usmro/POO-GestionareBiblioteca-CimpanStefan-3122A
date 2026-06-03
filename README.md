# 📚 BiblioManager Pro

> Sistem modern de gestiune a bibliotecii cu interfață terminal, bază de date SQLite și asistent AI.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![SQLite](https://img.shields.io/badge/Database-SQLite3-green)
![FTXUI](https://img.shields.io/badge/UI-FTXUI-orange)
![OpenSSL](https://img.shields.io/badge/Security-SHA256-red)

---

## 📖 Despre Proiect

**BiblioManager Pro** este o aplicație dezvoltată în **C++17** care simulează funcționarea unei biblioteci reale. Aplicația oferă gestionarea cărților, utilizatorilor și împrumuturilor printr-o interfață modernă în terminal.

Proiectul a fost realizat pentru cursul de **Programare Orientată pe Obiecte** și demonstrează utilizarea conceptelor avansate de dezvoltare software:

* 🏛️ Programare Orientată pe Obiecte
* 🗄️ Baze de date SQLite
* 🔒 Securitate și hashing SHA-256
* 🌐 Integrare API-uri externe
* 🎨 Interfață terminal modernă cu FTXUI
* 🤖 Integrare AI prin Groq

---

# ✨ Funcționalități

## 👤 Pentru Utilizatori

* 🔐 Creare cont și autentificare securizată
* 🔍 Căutare cărți după titlu sau autor
* 📥 Împrumut cărți
* 📤 Returnare cărți
* ⏰ Prelungire termen împrumut
* 📋 Vizualizare cărți împrumutate
* 🌍 Explorare tendințe globale prin Open Library

---

## 📚 Pentru Bibliotecari

* ➕ Adăugare cărți în inventar
* 📦 Recepție marfă (adăugare multiplă exemplare)
* 📊 Gestionare împrumuturi active
* 🔄 Forțare returnare cărți
* 📈 Statistici în timp real

---

## 🤖 Funcționalități Avansate

### BiblioAI

Asistent virtual bazat pe **Groq** pentru recomandări și informații despre cărți.

### Open Library Integration

Acces la milioane de cărți disponibile în catalogul Open Library.

### Persistență Date

Toate informațiile sunt stocate local folosind SQLite.

### Sistem Multi-Rol

* 👥 Guest
* 👤 User
* 📚 Bibliotecar
* 🛠️ Administrator

---

# 🛠️ Tehnologii Utilizate

| Tehnologie    | Descriere             |
| ------------- | --------------------- |
| C++17         | Limbaj principal      |
| FTXUI         | Interfață terminal    |
| SQLite3       | Bază de date          |
| CPR           | Client HTTP           |
| OpenSSL       | SHA-256 și securitate |
| nlohmann/json | Parsare JSON          |
| CMake         | Sistem de build       |

---

# 📦 Instalare

## 1️⃣ Cerințe

* C++17 Compiler
* CMake 3.15+
* SQLite3
* OpenSSL
* CPR
* FTXUI

---

## 2️⃣ Clonare proiect

```bash
git clone https://github.com/username/BiblioManager-Pro.git

cd BiblioManager-Pro
```

---

## 3️⃣ Compilare

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

---

## 4️⃣ Populare bază de date (opțional)

```bash
./seed_database
```

---

## 5️⃣ Configurare AI (opțional)

Creează fișierul `.env` în directorul `build/`:

```env
GEMINI_API_KEY=YOUR_API_KEY
```

---

# 🚀 Utilizare

## Pornire aplicație

```bash
cd build

./BiblioManagerPro
```

---

# 🔑 Conturi Implicite

| Username | Parolă   | Rol           |
| -------- | -------- | ------------- |
| admin    | admin123 | Administrator |
| biblio   | 1234     | Bibliotecar   |

---

# ⌨️ Navigare

| Tastă | Acțiune                   |
| ----- | ------------------------- |
| TAB   | Mută focusul              |
| ENTER | Selectează                |
| ↑ ↓   | Navigare liste            |
| ESC   | Închide fereastra curentă |

---

# 📁 Structura Proiectului

```text
BiblioManager-Pro/
│
├── include/
│   ├── models/
│   ├── services/
│   ├── database/
│   └── ui/
│
├── src/
│   ├── models/
│   ├── services/
│   ├── database/
│   └── ui/
│
├── assets/
│
├── database/
│   └── library.db
│
├── docs/
│
├── CMakeLists.txt
└── README.md
```

---

# 🗄️ Baza de Date

Aplicația utilizează SQLite și conține trei tabele principale:

## 📚 Carti

Stochează informațiile despre cărți și numărul de exemplare disponibile.

## 👤 Cititori

Conturi utilizatori cu parole securizate prin SHA-256.

## 📋 Imprumuturi

Înregistrează toate împrumuturile efectuate.

---

# 🏛️ Organizare pe Raioane

| Cod       | Secțiune                  |
| --------- | ------------------------- |
| A-LitRom  | Literatură Română         |
| B-LitUniv | Literatură Universală     |
| C-SFF     | Science Fiction & Fantasy |
| D-IstBio  | Istorie & Biografii       |
| E-PsiFil  | Psihologie & Filosofie    |
| F-CopiiYA | Copii & Young Adult       |

---

# 🔒 Securitate

* ✅ Hashing parole cu SHA-256
* ✅ Protecție împotriva conturilor duplicate
* ✅ Parameterized Queries (anti SQL Injection)
* ✅ Fișiere `.env` pentru chei API
* ✅ Foreign Keys pentru integritatea datelor

---

# 🎯 Concepte POO Demonstrate

## Encapsulare

Atribute private și metode getter/setter.

## Moștenire

Ierarhii de clase pentru ecrane și servicii.

## Polimorfism

Metode virtuale pentru randarea componentelor UI.

## Template-uri

Clase generice pentru colecții și utilitare.

## Smart Pointers

Gestionarea sigură a memoriei.

## Excepții

Tratarea erorilor și validarea inputului.

---

# 📸 Screenshots

## 🏠 Ecran Principal

```text
![Ecran Principal](https://i.postimg.cc/gJyvWStW/image.png)
```

## 📚 Panou Bibliotecar

```text
![Ecran Bibliotecar](https://i.postimg.cc/8zVNFZtK/image.png)
```

---

⭐ Dacă proiectul ți-a fost util, oferă un star repository-ului.
