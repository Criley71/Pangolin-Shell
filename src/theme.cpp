#include "../include/theme.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <pwd.h>
#include <unistd.h>

std::string get_colors_dir() {
    const char *home = getenv("HOME");
    if (!home) {
        struct passwd *pw = getpwuid(getuid());
        if (pw) home = pw->pw_dir;
    }
    const char *state = getenv("XDG_STATE_HOME");
    std::string state_dir = state ? state : std::string(home) + "/.local/state";
    return state_dir + "/pangolin/colors.txt";
}

void load_colors(ShellState& state) {
    std::ifstream fin(get_colors_dir());
    if (!fin.is_open()) return;

    std::string line;
    while (std::getline(fin, line)) {
        size_t equals_pos = line.find('=');
        if (equals_pos == std::string::npos) continue;

        std::string key = line.substr(0, equals_pos);
        std::string value = line.substr(equals_pos + 1);

        int r, g, b;
        if (sscanf(value.c_str(), "%d,%d,%d", &r, &g, &b) == 3) {
            state.colors[key] = RGB{r, g, b};
        }
    }
}

void save_colors(ShellState& state) {
    std::ofstream fout(get_colors_dir());
    if (!fout.is_open()) {
        std::cerr << "\033[31m[!]\033[0m Could not open colors file for saving.\n";
        return;
    }
    for (const auto& pair : state.colors) {
        fout << pair.first << "=" << pair.second.r << "," << pair.second.g << "," << pair.second.b << "\n";
    }
}

void apply_default_colors(ShellState& state) {
    auto ensure = [&](const std::string& key, RGB def) {
        if (state.colors.find(key) == state.colors.end())
            state.colors[key] = def;
    };
    ensure("logo_block",     RGB{158, 72, 68});
    ensure("logo_border",    RGB{66, 63, 79});
    ensure("calendar_text",  RGB{158, 72, 68});
    ensure("calendar_today", RGB{36, 200, 200});
    ensure("dir_color",      RGB{0, 200, 0});
}

RGB get_color(ShellState& state, const std::string& key, RGB fallback) {
    auto it = state.colors.find(key);
    return it != state.colors.end() ? it->second : fallback;
}