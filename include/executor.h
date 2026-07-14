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

class Executor {
  public:
    int execute(ASTNode *node);
    void expand_commands(std::vector<std::string> &argv);

  private:
    int execute_command(ASTNode* node);
    int execute_pipe(ASTNode* node);
};