#pragma once
#include "calendar.h"
#include "commands.h"
#include "executor.h"
#include "lexer.h"
#include <atomic>
#include <csignal>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <pwd.h>
#include <queue>
#include <readline/history.h>
#include <readline/keymaps.h>
#include <readline/readline.h>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>
struct REPL {
    REPL();
    void repl_loop();
    void shell_startup();
    void repl_dir_print();
    bool is_built_in(std::string command);
    bool is_aliased(std::string command);
    void check_dup_add_history(const char *user_input_command);
    void init_readline();
    void setup_signals();
    bool unknown_command_contains_slash(const std::string &command);
    bool found_in_path(const std::string &command);
    std::string tilde_translation(std::string arg);
    void load_history();
    std::string get_history_dir();
    void repl2();
    bool ends_in_backslash(std::string &s);
    void print_side_by_side(const std::vector<std::string> &logo, const std::vector<std::string> &cal);
    std::string expand_aliases(const std::string& input, ShellState& state);
    
    
    
    std::string logo = R"(
██████╗░░█████╗░███╗░░░██╗░██████╗░░██████╗░██╗░░░░░██╗███╗░░░██╗
██╔═██║░██╔══██╗████╗░░██║██╔════╝░██╔═══██╗██║░░░░░██║████╗░░██║
██████║░███████║██╔██╗░██║██║░░███╗██║░░░██║██║░░░░░██║██╔██╗░██║
██╔═══╝░██╔══██║██║╚██╗██║██║░░░██║██║░░░██║██║░░░░░██║██║╚██╗██║
██║░░░░░██║░░██║██║░╚████║╚██████╔╝╚██████╔╝███████╗██║██║░╚████║
╚═╝░░░░░╚═╝░░╚═╝╚═╝░░╚═══╝░╚═════╝░░╚═════╝░╚══════╝╚═╝╚═╝░░╚═══╝

    )";
    std::vector<std::string> icon_vector = {
        "\033[38;2;158;72;68m⣀⠀⣀⣀⡀ ",
        "⣿⡀⢻⣿⣿⣿⣿⣿⣶⠀⣤⣄⣀⠀⠀",
        "⠛⠓⠀⢻⣿⡿⠋⣉⣀⣀⢘⣿⣿⣿⡇⢠⡀",
        "⣾⣿⣿⣿⣿⠀⣾⣿⣿⣿⣿⡿⠟⠛⠧⠈⢿⣷⣄⠀",
        "⣿⣿⣿⡿⠿⠄⠹⣿⣿⣿⡃⢰⣶⣶⣶⣦⣴⣿⣿⠇⣀⠀",
        "⣿⡀⢿⣿⣿⣿⣿⣿⣿⠋⣠⣶⣧⡈⠛⠻⣷⣄⠁⠘⣿⣿⣷⡀⠀⠀\033[97m \u256D\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u256E\033[38;2;158;72;68m ",
        "⢉⠁⠘⢿⣿⣿⣿⣿⡇⢰⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⠈⠛⢿⣷⡀⠀\033[97m \u2502 Welcome to the Pangolin Shell \u2502\033[38;2;158;72;68m ",
        "⣿⣿⣷⣿⡿⠛⣉⡉⠁⢸⠃⠈⠙⠻⢿⣿⣿⣿⣷⣶⣾⣷⣄⠙⠳ \033[97m \u2570\u252c\u252c\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u256F\033[38;2;158;72;68m ",
        "⣿⣿⣿⣿⡀⢾⣿⣿⡇⠘⠀⠀⠀⠀⠀⠈⠉⠛⠻⠿⣿⣿⣿⣿⣦⡀⠀⠀\033[97m\u2575\u256F\033[38;2;158;72;68m",
        "⠿⠋⣉⣉⡁⠈⠻⣿⣿⡀",
        " ⣾⣿⣿⣿⣷⣶⣿⡏⢡⣤⣤⠀⢀⣤⣄⡐⢦⡀",
        "⠀⠙⢿⣿⣿⡿⠛⣉⣁⣀⣿⠃⠐⠛⠿⣿⣷⠀⠇",
        "⠀⠀⠀⠀⠈⠉⠃⠘⠿⠟⠛⠋⠀⠀⠀⠀⢹⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"};

    std::string icon = R"(
      \033[38;2;158;72;68m⠀⣀⠀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                   
      ⣿⡀⢻⣿⣿⣿⣿⣿⣶⠀⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      ⠛⠓⠀⢻⣿⡿⠋⣉⣀⣀⢘⣿⣿⣿⡇⢠⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      ⣾⣿⣿⣿⣿⠀⣾⣿⣿⣿⣿⡿⠟⠛⠧⠈⢿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      ⣿⣿⣿⡿⠿⠄⠹⣿⣿⣿⡃⢰⣶⣶⣶⣦⣴⣿⣿⠇⣀⠀⠀⠀⠀⠀⠀⠀⠀
      ⣿⡏⢠⣴⣶⣦⣤⣼⣿⣿⡷⠄⠉⠉⣉⡉⠛⢿⡏⢰⣿⣦⡀⠀⠀⠀\033[37m⠀⠀⠀\033[4m                               \033[24m\033[38;2;158;72;68m
      ⣿⡀⢿⣿⣿⣿⣿⣿⣿⠋⣠⣶⣧⡈⠛⠻⣷⣄⠁⠘⣿⣿⣷⡀⠀⠀\033[37m⠀⠀/⠀                              \\\033[38;2;158;72;68m
      ⢉⠁⠘⢿⣿⣿⣿⣿⡇⢰⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⠈⠛⢿⣷⡀⠀\033[37m⠀\uff5c Welcome to the Pangolin Shell \uff5c\033[38;2;158;72;68m
      ⣿⣿⣷⣿⡿⠛⣉⡉⠁⢸⠃⠈⠙⠻⢿⣿⣿⣿⣷⣶⣾⣷⣄⠙⠳⠀⠀\033[37m⠀\\\033[4m                               \033[24m/\033[38;2;158;72;68m
      ⣿⣿⣿⣿⡀⢾⣿⣿⡇⠘⠀⠀⠀⠀⠀⠈⠉⠛⠻⠿⣿⣿⣿⣿⣦⡀⠀⠀\033[37m⠀|/\033[38;2;158;72;68m"
      ⠿⠋⣉⣉⡁⠈⠻⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠉⠀⠀⠀⠀
      ⠀⣾⣿⣿⣿⣷⣶⣿⡏⢡⣤⣤⠀⢀⣤⣄⡐⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      ⠀⠙⢿⣿⣿⡿⠛⣉⣁⣀⣿⠃⠐⠛⠿⣿⣷⠀⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠈⠉⠃⠘⠿⠟⠛⠋⠀⠀⠀⠀⢹⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      \033[37m
      )";

  private:
    std::vector<std::string> builtin_commands{"cd", "exit", "help", "pwd"};
    std::unordered_map<std::string, std::vector<std::string>> aliases = {
        {"ls", {"--color=auto"}},
        {"grep", {"--color=auto"}}};
};
