#pragma once

#include "parser.h"
#include <cstring>
#include <stdexcept>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include "commands.h"
#include <unistd.h>
#include <sstream>
#include <fcntl.h>
#include <unordered_map>
#include "shell_state.h"

class Executor {
  public:
    int execute(ASTNode *node, ShellState& state);
    void expand_commands(std::vector<std::string> &argv);

  private:
    int execute_command(ASTNode* node, ShellState& state);
    int execute_pipe(ASTNode* node, ShellState& state);
    std::unordered_map<std::string, std::string> aliases;
};