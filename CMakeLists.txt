cmake_minimum_required(VERSION 3.14)
project(Library-Manager-APP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# FTXUI
FetchContent_Declare(ftxui 
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI 
    GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

# CPR
FetchContent_Declare(cpr 
    GIT_REPOSITORY https://github.com/libcpr/cpr.git 
    GIT_TAG 1.10.x
)
FetchContent_MakeAvailable(cpr)

# JSON
FetchContent_Declare(json 
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

find_package(SQLite3 REQUIRED)

set(SOURCES 
    src/main.cpp
    src/Carte.cpp
    src/Biblioteca.cpp
    src/Theme.cpp
    src/BiblioAI.cpp
    src/screens/StartScreen.cpp
    src/screens/LoginScreen.cpp
    src/screens/UserScreen.cpp
    src/screens/BiblioScreen.cpp
    src/screens/AdminScreen.cpp
)

add_executable(biblioteca_app ${SOURCES})

configure_file(.env ${CMAKE_BINARY_DIR}/.env COPYONLY)

target_link_libraries(biblioteca_app PRIVATE 
    ftxui::screen 
    ftxui::dom 
    ftxui::component
    cpr::cpr 
    nlohmann_json::nlohmann_json
    SQLite::SQLite3
)

target_include_directories(biblioteca_app PRIVATE ${CMAKE_SOURCE_DIR}/include)