cmake_minimum_required(VERSION 3.22)
project(pangolin)

set(CMAKE_CXX_STANDARD 23)

find_package(Curses REQUIRED)

add_executable(pangolin 
    src/main.cpp 
    src/repl.cpp 
    src/commands.cpp 
    src/lexer.cpp 
    src/parser.cpp
    src/executor.cpp
    src/calendar.cpp
    include/shell_state.h)
target_link_libraries(pangolin PRIVATE readline ncurses)
target_compile_options(pangolin PRIVATE -Wall -Wextra -Wpedantic)