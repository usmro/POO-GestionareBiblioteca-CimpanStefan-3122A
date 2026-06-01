cmake_minimum_required(VERSION 3.14)
project(Library-Manager-APP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Interfata TUI
FetchContent_Declare(ftxui GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI GIT_TAG v5.0.0)
FetchContent_MakeAvailable(ftxui)

# Internet (HTTP) pentru AI
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 1.10.x)
FetchContent_MakeAvailable(cpr)

# JSON pentru AI
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

find_package(SQLite3 REQUIRED)
include_directories(include)

set(SOURCES src/main.cpp src/Carte.cpp src/Biblioteca.cpp)
add_executable(biblioteca_app ${SOURCES})

# Copiem .env în build automat
configure_file(.env ${CMAKE_BINARY_DIR}/.env COPYONLY)

target_link_libraries(biblioteca_app PRIVATE 
    ftxui::screen ftxui::dom ftxui::component
    cpr::cpr nlohmann_json::nlohmann_json
    SQLite::SQLite3
)