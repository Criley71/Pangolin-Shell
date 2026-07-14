#include "../include/commands.h"

bool Commands::determine_command(std::vector<std::string>& command){
if(command[0] == "cd" ){
    if(command.size() < 2){
        std::cerr << "\033[31m[!]\033[0m cd: missing argument\n";
        return false; 
      }
    cd(command[1]);
    return true;
  }
if(command[0] == "exit"){
    std::cout << "Exiting... Thanks for using the pangolin shell! Until next time, so long.\n";
    exit(0);
}
  return false;
}

bool Commands::cd(std::string path){
    int rc = chdir(path.c_str());
    if(rc < 0){
        perror("\033[31m[!]\033[0m cd command failed");
        return false;
    }
    return true;
}

void Commands::exit_shell(){
    
}