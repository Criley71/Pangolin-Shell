#include "../include/commands.h"

bool Commands::determine_command(std::vector<std::string> &command, ShellState &state) {
    if (command[0] == "cd") {
        if (command.size() < 2) {
            std::cerr << "\033[31m[!]\033[0m cd: missing argument\n";
            return false;
        }
        cd(command[1]);
        return true;
    }
    if (command[0] == "exit") {
        std::cout << "Exiting... Thanks for using the pangolin shell! Until next time, so long.\n";
        exit(0);
    }
    if (command[0] == "alias") {
        return builtin_alias(command, state);
    }
    if (command[0] == "unalias") {
        return builtin_unalias(command, state);
    }

    return false;
}

bool Commands::cd(std::string path) {
    int rc = chdir(path.c_str());
    if (rc < 0) {
        perror("\033[31m[!]\033[0m cd command failed");
        return false;
    }
    return true;
}

void Commands::exit_shell() {
}

bool Commands::builtin_alias(std::vector<std::string> &argv, ShellState &state) {
    if (argv.size() == 1) {
        for (const auto &pair : state.aliases) {
            std::cout << "alias " << pair.first << "='" << pair.second << "'\n";
        }
        return true;
    }

    for (size_t i = 1; i < argv.size(); i++) {
        std::string arg = argv[i];
        size_t equals_pos = argv[i].find('=');
        if (equals_pos != std::string::npos) {
            std::string key = arg.substr(0, equals_pos);
            std::string value = arg.substr(equals_pos + 1);

            if (value.size() >= 2 &&
                (value.front() == '"' || value.front() == '\'') &&
                (value.back() == '"' || value.back() == '\'')) {
                value = value.substr(1, value.size() - 2);
            }

            state.aliases[key] = value;
        } else {
            auto it = state.aliases.find(arg);
            if (it != state.aliases.end()) {
                std::cout << "alias " << it->first << "='" << it->second << "'\n";
            } else {
                std::cerr << "alias: " << arg << ": not found\n";
            }
        }
    }
    return true;
}
bool Commands::builtin_unalias(std::vector<std::string> &argv, ShellState &state) {
    if (argv.size() < 2) {
        std::cerr << "\033[31m[!]\033[0m unalias : missing argument. Use --help for information on the command\n";
    }
    if (argv[1] == "--h" || argv[1] == "--help") {
        std::cout << "unalias: unalias [-a] name [name ...]\n   Remove each NAME from a list of defined aliases. \n\n   Options:\n      -a     remove all alias defintions\n\n";
    }
    state.aliases.erase(argv[1]);
    return true;
}