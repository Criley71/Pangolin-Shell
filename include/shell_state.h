#pragma once
#include <unordered_map>
#include <string>

struct RGB {
    int r = 0, g = 0, b = 0;
    std::string ansi() const {
        return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
    }
};

struct ShellState {
    std::unordered_map<std::string, std::string> aliases;
    std::unordered_map<std::string, RGB> colors;
};