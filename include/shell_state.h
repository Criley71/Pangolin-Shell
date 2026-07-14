#pragma once
#include <unordered_map>
#include <string>

struct ShellState {
    std::unordered_map<std::string, std::string> aliases;
};