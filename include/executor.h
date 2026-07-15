#pragma once

#include "commands.h"
#include "parser.h"
#include "shell_state.h"
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

class Executor {
  public:
    int execute(ASTNode *node, ShellState &state);
    void expand_commands(std::vector<std::string> &argv);

  private:
    int execute_command(ASTNode *node, ShellState &state);
    int execute_pipe(ASTNode *node, ShellState &state);
    std::unordered_map<std::string, std::string> aliases;
};