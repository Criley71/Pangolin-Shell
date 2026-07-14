#pragma once
#include "../include/shell_state.h"
#include "repl.h"
#include <chrono>
#include <iostream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <vector>

struct Commands {
    bool determine_command(std::vector<std::string> &command, ShellState &state);
    bool cd(std::string path);
    void exit_shell();
    bool builtin_alias(std::vector<std::string> &argv, ShellState &state);
    bool builtin_unalias(std::vector<std::string> &argv, ShellState &state);
};