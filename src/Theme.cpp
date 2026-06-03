#include "Theme.h"
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

Component MakeBigBtn(int& limba_selectata, std::string ro, std::string en, std::function<void()> on_click) {
    ButtonOption option = ButtonOption::Animated();
    option.transform = [&limba_selectata, ro, en](const EntryState& s) {
        bool is_en = (limba_selectata == 1);
        Element t = text(is_en ? en : ro) | center;
        if (s.focused) t = t | inverted | bold;
        Elements block = {text(""), t, text("")};
        return vbox(block) | border | size(WIDTH, EQUAL, 65) | center; 
    };
    return Button(ro, std::move(on_click), option);
}

Component MakeSmallBtn(int& limba_selectata, std::string ro, std::string en, std::function<void()> on_click) {
    ButtonOption option = ButtonOption::Animated();
    option.transform = [&limba_selectata, ro, en](const EntryState& s) {
        bool is_en = (limba_selectata == 1);
        Element t = text(is_en ? en : ro) | center;
        if (s.focused) t = t | inverted | bold;
        return t | border | size(WIDTH, EQUAL, 25) | center;
    };
    return Button(ro, std::move(on_click), option);
}