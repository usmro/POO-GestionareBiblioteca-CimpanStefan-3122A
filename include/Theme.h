#pragma once
#include <ftxui/component/component.hpp>
#include <functional>
#include <string>

ftxui::Component MakeBigBtn(int& limba_selectata, std::string ro, std::string en, std::function<void()> on_click);
ftxui::Component MakeSmallBtn(int& limba_selectata, std::string ro, std::string en, std::function<void()> on_click);