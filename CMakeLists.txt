cmake_minimum_required(VERSION 3.10)

# Numele proiectului (poți pune grupa aici)
project(Library-Manager-APP)

# Setăm standardul C++ (11 sau 17 pentru a folosi STL modern)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Spunem unde să caute fișierele de antet (.h)
include_directories(include)

# Colectăm toate fișierele sursă (.cpp)
# Nota: Momentan main.cpp este baza, ulterior adaugi restul
set(SOURCES 
    src/main.cpp
    # src/Carte.cpp      <-- Le vei decomenta când scrii codul în ele
    # src/Biblioteca.cpp
)

# Creăm executabilul numit "biblioteca_app"
add_executable(biblioteca_app ${SOURCES})
