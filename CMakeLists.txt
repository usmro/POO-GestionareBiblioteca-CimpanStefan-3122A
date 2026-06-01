cmake_minimum_required(VERSION 3.14)
project(Library-Manager-APP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Descarcă interfața grafică FTXUI
include(FetchContent)
FetchContent_Declare(ftxui GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI GIT_TAG v5.0.0)
FetchContent_MakeAvailable(ftxui)

# Caută baza de date SQLite3 în Linux
find_package(SQLite3 REQUIRED)

# Include folderul cu headere
include_directories(include)

# Setați fișierele sursă
set(SOURCES 
    src/main.cpp
    src/Carte.cpp
    src/Biblioteca.cpp
)

# Creează aplicația
add_executable(biblioteca_app ${SOURCES})

# Leagă bibliotecile
target_link_libraries(biblioteca_app PRIVATE 
    ftxui::screen ftxui::dom ftxui::component
    SQLite::SQLite3
)