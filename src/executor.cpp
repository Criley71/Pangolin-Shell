#include "../include/executor.h"

void Executor::expand_commands(std::vector<std::string> &argv) {
    for (auto &arg : argv) {
        if (!arg.empty() && arg[0] == '$') {
            const char *val = getenv(arg.c_str() + 1);
            arg = val ? val : "";
        }
    }
    const char *home = getenv("HOME");
    for (auto &arg : argv) {
        if (!arg.empty() && arg[0] == '~') {
            if (arg.size() == 1 || arg[1] == '/') {
                std::string home_str = home ? home : "";
                arg = home_str + arg.substr(1);
            }
        }
    }
    // ls color flag, should add to aliases
    // if(!argv.empty() && argv[0] == "ls"){
    //     bool has_color_flag = false;
    //     for(const auto &arg : argv){
    //         if(arg.find('--color') != std::string::npos){
    //             has_color_flag = true;
    //             break;
    //         }
    //     }
    //     if(!has_color_flag){
    //         argv.insert(argv.begin()+1, "--color=auto");
    //     }
    // }
}

int Executor::execute_command(ASTNode *node) {
    if (node->argv.empty()) {
        return 0;
    }
    Commands command;
    expand_commands(node->argv);
    if (command.determine_command(node->argv)) {
        return 0;
    }
    std::vector<char *> argv;
    for (const auto &arg : node->argv) {
        argv.push_back(const_cast<char *>(arg.c_str()));
    }
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid == 0) {
        if (!node->redirect_in.empty()) {
            int fd = open(node->redirect_in.c_str(), O_RDONLY);
            if (fd < 0) {
                perror("\033[31m[!]\033[0m open input");
                _exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if (!node->redirect_out.empty()) {
            int flags = O_WRONLY | O_CREAT;
            flags |= node->append_out ? O_APPEND : O_TRUNC; // append or truncate
            int fd = open(node->redirect_out.c_str(), flags, 0644);
            if (fd < 0) {
                perror("\033[31m[!]\033[0m open output");
                _exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execvp(argv[0], argv.data());
        std::cerr << "\033[31m[!]\033[0m execvp: unknown command - " << argv[0] << "\n";
        _exit(127);
    }

    int status = 0;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    return 0;
}

int Executor::execute_pipe(ASTNode *node) {
    int fd[2];
    if (pipe(fd) == -1) {
        std::cerr << "\033[31m[!]\033[0m pipe";
        return 1;
    }
    pid_t left = fork();
    if (left == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        _exit(execute(node->left.get()));
    }
    pid_t right = fork();
    if (right == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        close(fd[0]);
        _exit(execute(node->right.get()));
    }
    close(fd[0]);
    close(fd[1]);

    int status = 0;
    waitpid(left, &status, 0);
    waitpid(right, &status, 0);

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    return 0;
}

int Executor::execute(ASTNode *node) {
    if (!node) return 0;
    switch (node->type) {
    case NodeType::COMMAND:
        return execute_command(node);

    case NodeType::PIPE:
        return execute_pipe(node);

    case NodeType::AND_IF: {
        int status = execute(node->left.get());
        if (status == 0) {
            return execute(node->right.get());
        }
        return status;
    }
    case NodeType::OR_IF: {
        int status = execute(node->left.get());
        if (status != 0)
            return execute(node->right.get());
        return status;
    }
    case NodeType::SEQUENCE: {
        execute(node->left.get());
        // Execute and return the right side
        return execute(node->right.get());
    }
    }
    throw std::runtime_error("Unknown AST Type");
}