#include "../include/repl.h"
#include "../include/commands.h"

volatile sig_atomic_t sigint_recieved = 0;

void handle_sigint(int) {
    sigint_recieved = 1;
    // rl_done = 1;
}

int check_signals() {
    if (sigint_recieved) {
        rl_replace_line("", 0);
        rl_done = 1;
    }
    return 0;
}

REPL::REPL() {
}

void REPL::shell_startup() {
    rl_catch_signals = 0;
    rl_catch_sigwinch = 0;
    rl_event_hook = check_signals;
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, nullptr) == -1) {
        perror("\033[31m[!]\033[0m sigaction failed");
    }
    const char *home = getenv("HOME");
    if (!home) {
        struct passwd *pw = getpwuid(getuid());
        if (pw) home = pw->pw_dir;
    }
    const char *state = getenv("XDG_STATE_HOME");
    std::string state_dir;

    if (state) {
        state_dir = state;
    } else {
        state_dir = std::string(home) + "/.local/state";
    }
    std::string pangolin_dir = state_dir + "/pangolin";
    if (mkdir(state_dir.c_str(), 0700) == -1 && errno != EEXIST) {
        perror("\033[31m[!]\033[0mmkdir state_dir");
    }

    if (mkdir(pangolin_dir.c_str(), 0700) == -1 && errno != EEXIST) {
        perror("\033[31m[!]\033[0mmkdir pangolin_dir");
    }

    load_history();
    //  cout << " \033[48;2;158;72;68m " << logo << "     \033[0m\n";
    std::string RED = "\033[38;2;158;72;68m";
    std::string BLUE = "\033[38;2;66;63;79m";
    std::string RESET = "\033[0m";
    std::string block = "█";
    std::string blues[7] = {"╗", "║", "╝", "═", "╚", "╔", "░"};

    signal(SIGINT, handle_sigint);
    for (size_t i = 0; i < logo.size(); i++) {
        bool matchFound = false;

        if (logo.substr(i, block.length()) == block) {
            std::cout << RED << block << RESET;
            i += (block.length() - 1);
            matchFound = true;
        }

        if (!matchFound) {
            for (const std::string &shape : blues) {
                if (logo.substr(i, shape.length()) == shape) {
                    std::cout << BLUE << shape << RESET;
                    i += (shape.length() - 1);
                    matchFound = true;
                    break;
                }
            }
        }
        if (!matchFound) {
            std::cout << logo[i];
        }
    }

    std::vector<std::string> cal = get_month_vector();

    size_t max_lines = std::max(icon_vector.size(), cal.size());
    int calendar_start_column = 63;

    std::cout << "\n";
    for (size_t i = 0; i < max_lines; i++) {

        if (i < icon_vector.size()) {
            std::cout << icon_vector[i];
        }

        std::cout << "\033[" << calendar_start_column << "G";

        if (i < cal.size()) {
            std::cout << cal[i];
        }

        std::cout << "\n";
    }
    std::cout << "\n";
    // std::cout << "\033[38;2;158;72;68m\n"
    //              "⣀⠀⣀⣀\n"
    //              "⣿⡀⢻⣿⣿⣿⣿⣿⣶⠀⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
    //              "⠛⠓⠀⢻⣿⡿⠋⣉⣀⣀⢘⣿⣿⣿⡇⢠⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
    //              "⣾⣿⣿⣿⣿⠀⣾⣿⣿⣿⣿⡿⠟⠛⠧⠈⢿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
    //              "⣿⣿⣿⡿⠿⠄⠹⣿⣿⣿⡃⢰⣶⣶⣶⣦⣴⣿⣿⠇⣀⠀⠀⠀⠀⠀⠀⠀⠀\n"
    //              "⣿⡏⢠⣴⣶⣦⣤⣼⣿⣿⡷⠄⠉⠉⣉⡉⠛⢿⡏⢰⣿⣦⡀⠀⠀⠀\n"
    //              "⣿⡀⢿⣿⣿⣿⣿⣿⣿⠋⣠⣶⣧⡈⠛⠻⣷⣄⠁⠘⣿⣿⣷⡀⠀⠀\033[97m \u256D\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u256E\n\033[38;2;158;72;68m"
    //              "⢉⠁⠘⢿⣿⣿⣿⣿⡇⢰⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⠈⠛⢿⣷⡀⠀\033[97m \u2502 Welcome to the Pangolin Shell \u2502\n\033[38;2;158;72;68m"
    //              "⣿⣿⣷⣿⡿⠛⣉⡉⠁⢸⠃⠈⠙⠻⢿⣿⣿⣿⣷⣶⣾⣷⣄⠙⠳\033[97m  \u2570\u252c\u252c\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u256F\n\033[38;2;158;72;68m"
    //              "⣿⣿⣿⣿⡀⢾⣿⣿⡇⠘⠀⠀⠀⠀⠀⠈⠉⠛⠻⠿⣿⣿⣿⣿⣦⡀⠀⠀\033[97m\u2575\u256F\033[38;2;158;72;68m\n"
    //              "⠿⠋⣉⣉⡁⠈⠻⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠉⠀⠀⠀⠀\n"
    //              "⠀⣾⣿⣿⣿⣷⣶⣿⡏⢡⣤⣤⠀⢀⣤⣄⡐⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
    //              "⠀⠙⢿⣿⣿⡿⠛⣉⣁⣀⣿⠃⠐⠛⠿⣿⣷⠀⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
    //              "⠀⠀⠀⠀⠈⠉⠃⠘⠿⠟⠛⠋⠀⠀⠀⠀⢹⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
    //              "\033[37m";
}

// void REPL::repl_loop() {
//     shell_startup(); // call start up logo on boot

//     char cwd[2048]; // current working dir array
//     int last_status = 0;
//     Commands commands;
//     while (true) {
//         sigint_recieved = 0;
//         if (!getcwd(cwd, sizeof(cwd))) {
//             perror("\033[31m[!]\033[0m getcwd");
//             continue;
//         }
//         // prompt of pangolin and cwd, colored text used
//         std::string prompt = "\033[34m(pangolin)\033[32m" + std::string(cwd) + "\033[34m$\033[0m ";

//         char *line = readline(prompt.c_str());
//         if (!line) {
//             break; // ctrl+d
//         }
//         if (sigint_recieved) {
//             free(line);
//             continue;
//         }

//         std::vector<std::vector<std::string>> user_input_commands;
//         user_input_commands.push_back({});
//         std::stringstream ss(line);
//         std::string token;
//         while (ss >> token) {
//             if (token == "&&") {
//                 user_input_commands.push_back({}); // new set of commands seperated by empty vector
//             } else {
//                 user_input_commands.back().push_back(token);
//             }
//         }
//         // for (auto &cmd : user_input_commands){
//         //     for(auto &arg : cmd){
//         // tilde translation
//         //     }
//         // }
//         for (auto &cmd : user_input_commands) {
//             if (cmd.empty()) {
//                 continue;
//             }
//             if (is_built_in(cmd[0])) {
//                 bool success = commands.determine_command(cmd);
//                 if (success) {
//                     last_status = 0;
//                 } else {
//                     last_status = 1;
//                     break;
//                 }
//                 continue;
//             }
//             if (is_aliased(cmd[0])) {
//                 for (auto &c : aliases[cmd[0]]) {
//                     cmd.push_back((const_cast<char *>(c.c_str())));
//                 }
//             }
//             std::vector<char *> argv;
//             for (auto &v : cmd) {
//                 argv.push_back(const_cast<char *>(v.c_str()));
//             }
//             argv.push_back(nullptr); // command arguments have to be ended with a null for execvp

//             pid_t pid = fork(); // make new process for the command to run on
//             if (pid < 0) {
//                 perror("\033[31m[!]\033[0m fork error");
//                 last_status = 1;
//                 break;
//             }
//             if (pid == 0) { // forked process id
//                 signal(SIGINT, SIG_DFL);
//                 execvp(argv[0], argv.data());
//                 switch (errno) {
//                 case ENOENT: // no file or dir
//                     std::cerr << "\033[31m[!]\033[0m no file or dir error\n";
//                     _exit(127);
//                     break;
//                 case EACCES: // access denied
//                     _exit(126);
//                 default:
//                     std::cerr << "\033[31m[!]\033[0m unknown command\n";
//                     _exit(1);
//                 }
//             }
//             int status;
//             waitpid(pid, &status, 0);

//             if (WIFEXITED(status)) {
//                 last_status = WEXITSTATUS(status);
//             } else {
//                 last_status = 1;
//             }
//             if (last_status != 0) {
//                 break;
//             }
//         }
//         check_dup_add_history(line);
//         free(line);
//     }
// }

std::string REPL::get_history_dir() {
    const char *home = getenv("HOME");
    if (!home) {
        struct passwd *pw = getpwuid(getuid());
        if (pw) home = pw->pw_dir;
    }
    const char *state = getenv("XDG_STATE_HOME");
    std::string state_dir;
    if (state) {
        state_dir = state;
    } else {
        state_dir = std::string(home) + "/.local/state";
    }
    return state_dir + "/pangolin/history";
}

void REPL::load_history() {
    std::ifstream fin(get_history_dir());
    if (!fin.good()) {
        perror("\033[31m[!]\033[0m History file open error");
    }
    std::queue<std::string> history;
    std::string buffer;
    while (getline(fin, buffer)) {
        history.push(buffer);
    }
    while (!history.empty()) {
        add_history((history.front()).c_str());
        history.pop();
    }
}

void REPL::setup_signals() {
    struct sigaction sa{};
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, nullptr);
    setenv("TERM", "xterm-256color", 1);
    setenv("LS_COLORS", "di=34:fi=0:ln=36:ex=32", 1);
    // cout << isatty(STDOUT_FILENO) << "\n";
}

void REPL::init_readline() {
    rl_catch_signals = 0;
    rl_catch_sigwinch = 0;
}

void REPL::check_dup_add_history(const char *user_input_command) {
    HIST_ENTRY *entry = history_get(history_length);
    if (entry && strcmp(entry->line, user_input_command) == 0) {
        return;
    }
    add_history(user_input_command);
    const char *home = getenv("HOME");
    if (!home) {
        struct passwd *pw = getpwuid(getuid());
        if (pw) home = pw->pw_dir;
    }
    const char *state = getenv("XDG_STATE_HOME");
    std::string state_dir;

    if (state) {
        state_dir = state;
    } else {
        state_dir = std::string(home) + "/.local/state";
    }

    std::string history_path = state_dir + "/pangolin/history";

    std::ofstream fout(history_path, std::ios_base::app);
    if (!fout.good()) {
        perror("fout");
    }
    fout << user_input_command << "\n";
    fout.close();
}

bool REPL::is_built_in(std::string command) {
    for (size_t i = 0; i < builtin_commands.size(); i++) {
        if (builtin_commands[i] == command) {
            return true;
        }
    }
    return false;
}

bool REPL::is_aliased(std::string command) {
    for (auto c : aliases) {
        if (c.first == command) {
            return true;
        }
    }
    return false;
}

void REPL::repl2() {
    ShellState state;
    shell_startup();
    char cwd[2048];
    Lexer lexer;
    Executor executor;

    while (true) {
        sigint_recieved = 0;
        if (!getcwd(cwd, sizeof(cwd))) {
            std::cerr << "\033[31m[!]\033[0m getcwd\n";
            perror("getcwd: ");
            continue;
        }

        std::string prompt = "\033[34m(pangolin)\033[32m" + std::string(cwd) + "\033[34m$\033[0m ";
        std::string user_input_commands;
        bool first_line = true;

        while (true) { // read multiple lines
            char *line = readline(first_line ? prompt.c_str() : "> ");
            if (sigint_recieved) {
                if (line) free(line);
                user_input_commands.clear();
                // std::cout << "\n";
                break;
            }
            if (!line) {
                std::cout << "Exiting... Thanks for using the pangolin shell! Until next time, so long.\n";
                return; // ctrl+d
            }

            std::string current_line(line);
            free(line);

            if (ends_in_backslash(current_line)) {
                current_line.pop_back();
                user_input_commands += current_line;
                first_line = false;
                continue;
            } else {
                user_input_commands += current_line;
                break;
            }
        }

        if (user_input_commands.empty()) {
            continue;
        }
        check_dup_add_history(user_input_commands.c_str());
        std::string expanded_input = expand_aliases(user_input_commands, state);
        try {
            auto tokens = lexer.lex_input(expanded_input);
            Parser parser(tokens);
            auto ast = parser.parse();
            executor.execute(ast.get(), state);
        } catch (const std::exception &e) {
            std::cerr << "\033[31m[!]\033[0m " << e.what() << ": " << user_input_commands << "\n";
        }
    }
}

bool REPL::ends_in_backslash(std::string &s) {
    size_t i = s.size();
    while (i > 0 && isspace((unsigned char)s[i - 1])) {
        i--;
    }
    if (i == 0 || s[i - 1] != '\\') {
        return false;
    }
    size_t count = 0;
    while (i > 0 && s[--i] == '\\') {
        count++;
    }
    return (count % 2) == 1;
}

void REPL::print_side_by_side(const std::vector<std::string> &logo, const std::vector<std::string> &cal) {
    size_t max_lines = std::max(logo.size(), cal.size());
    // int default_length = 70;
    int logo_width = 200; // The max character width of your Pangolin

    for (size_t i = 0; i < max_lines; i++) {
        // Print logo line (or blank spaces if we ran out of logo lines)
        if (i < logo.size()) {
            logo_width = logo[i].size();
        } else {
            logo_width = 0;
        }
        std::cout << "\033[38;2;158;72;68m";
        if (i < logo.size()) {
            std::cout << std::left << std::setw(logo_width) << logo[i];
        } else {
            std::cout << std::setw(logo_width) << " ";
        }

        // Print calendar line (or nothing if we ran out of calendar lines)
        if (i < cal.size()) {
            std::cout << std::setw(200) << cal[i];
        }
        std::cout << "\n";
    }
}

std::string REPL::expand_aliases(const std::string &input, ShellState &state) {
    size_t start = input.find_first_not_of(" \t");
    if (start == std::string::npos) {
        return input; // just spaces for input
    }
    size_t end = input.find_first_of(" \t|&;<>", start); // word end

    std::string first_word = input.substr(start, end - start);

    auto it = state.aliases.find(first_word);
    if (it != state.aliases.end()) { 
        std::string alias_value = it->second;
        //check if alias starts with the same first word
        if (alias_value.substr(0, first_word.length()) == first_word && (alias_value.length() == first_word.length() || alias_value[first_word.length()] == ' ')) {
            std::string remainder = (end == std::string::npos) ? "" : input.substr(end);
            return alias_value + remainder;
        }

        std::string remainder = (end == std::string::npos) ? "" : input.substr(end);
        return alias_value + remainder;
    }
    return input; //not an alias
}