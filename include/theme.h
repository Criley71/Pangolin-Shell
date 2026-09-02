#pragma once
#include "shell_state.h"
#include <string>
#include <unordered_set>

// The only keys shell_startup/prompt actually read — keeps typos from
// silently creating dead entries in state.colors.
inline const std::unordered_set<std::string> VALID_COLOR_KEYS = {
    "logo_block", "logo_border", "calendar_text", "calendar_today", "dir_color"
};

std::string get_colors_dir();
void load_colors(ShellState& state);
void save_colors(ShellState& state);
void apply_default_colors(ShellState& state);
RGB get_color(ShellState& state, const std::string& key, RGB fallback);