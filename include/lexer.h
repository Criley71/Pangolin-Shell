#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
enum class TokenType {
    WORD,
    PIPE,
    AND_IF,
    OR_IF,
    REDIRECT_OUT,
    REDIRECT_IN,
    SEMICOLON,
    NLINE,
    BACKGROUND,
    LPAREN,
    RPAREN,
    EOI,
    INVALID
};

enum class LexerState {
    DEFAULT,         // between tokens
    WORD,            // unquoted text
    IN_SINGLE_QUOTE, // single quote
    IN_DOUBLE_QUOTE, // double quotes
    ESCAPE_IN_QUOTE, // backslash in quotes
    OP,
    WHITESPACE,
    COMMENT,
    ESCAPE,
    ERROR
};

struct Token{
    TokenType type;
    std::string lexeme;
    Token(TokenType, std::string);
};

struct Lexer{
    LexerState current_state = LexerState::DEFAULT;
    std::vector<Token> lexed_tokens = {};
    std::vector<Token> lex_input(std::string &input);
};