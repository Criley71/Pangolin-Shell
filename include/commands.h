#pragma once
#include "repl.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <chrono>

struct Commands{
  bool determine_command(std::vector<std::string>& command);
  bool cd(std::string path); 
  void exit_shell();
  
};