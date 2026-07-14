#include "../include/lexer.h"

Token::Token(TokenType tt, std::string i) {
    type = tt;
    lexeme = i;
}

std::vector<Token> Lexer::lex_input(std::string &input) {
    std::vector<Token> lex_return = {};
    std::string buffer = "";
    current_state = LexerState::DEFAULT;
    LexerState prev_state;
    size_t n = input.size();
    for (size_t i = 0; i < n; i++) {
        char c = input[i];
        switch (current_state) {
        case LexerState::DEFAULT: // we are in state between words
            if (c == '\'') {      // begin single quotes
                current_state = LexerState::IN_SINGLE_QUOTE;
            } else if (c == '"') { // begin double quotes
                current_state = LexerState::IN_DOUBLE_QUOTE;
            } else if (c == '\\') { // escape character
                current_state = LexerState::ESCAPE;
            } else if (c == '&') {     // possible and_if or background
                if (!buffer.empty()) { // push buffer word to ast
                    lex_return.push_back(Token(TokenType::WORD, buffer));
                    buffer.clear();
                }
                if (i + 1 < n && input[i + 1] == '&') {
                    lex_return.push_back(Token(TokenType::AND_IF, ""));
                    i++; // consume second &
                } else {
                    lex_return.push_back(Token(TokenType::BACKGROUND, ""));
                }
            } else if (c == '|') { // possible or_if or a pipe
                if (!buffer.empty()) {
                    lex_return.push_back(Token(TokenType::WORD, buffer));
                    buffer.clear();
                }
                if (i + 1 < n && input[i + 1] == '|') {
                    lex_return.push_back(Token(TokenType::OR_IF, ""));
                    i++; // consume second |
                } else {
                    lex_return.push_back(Token(TokenType::PIPE, ""));
                }
            } else if (c == ';') {
                lex_return.push_back(Token(TokenType::SEMICOLON, ""));
            } else if (c == '<') {
                lex_return.push_back(Token(TokenType::WORD, "<"));
            } else if (c == '>') {
                // Check for >>
                if (i + 1 < n && input[i + 1] == '>') {
                    lex_return.push_back(Token(TokenType::WORD, ">>"));
                    i++; // consume second >
                } else {
                    lex_return.push_back(Token(TokenType::WORD, ">"));
                }
            } else { // we are starting a word, add char to buffer and swap states
                if (c != ' ') {
                    buffer += c;
                }
                current_state = LexerState::WORD;
            }
            break;
        case LexerState::WORD:
            if (c == ' ') { // word done, push and reset and buffer
                if (!buffer.empty()) {
                    lex_return.push_back(Token(TokenType::WORD, buffer));
                    buffer = "";
                }
                current_state = LexerState::DEFAULT; // reset state
            } else if (c == '&' || c == '|' || c == ';' || c == '<' || c == '>') {
                if (!buffer.empty()) {
                    lex_return.push_back(Token(TokenType::WORD, buffer));
                    buffer = "";
                }
                i--; // decrement to process the and_if, or_if, semicolon, redirects,
                     // pipe or background in the DEFAULT state switch case above
                current_state = LexerState::DEFAULT;
            } else if (c == '\'') {
                current_state = LexerState::IN_SINGLE_QUOTE;
            } else if (c == '"') {
                current_state = LexerState::IN_DOUBLE_QUOTE;
            }
            // else if (c== ';'){
            //     lex_return.push_back(Token(TokenType::WORD, buffer));
            //     buffer = "";
            //     lex_return.push_back(Token(TokenType::SEMICOLON, ""));
            //     current_state = LexerState::DEFAULT;
            // }
            else {
                buffer += c;
            }
            break;
        case LexerState::IN_SINGLE_QUOTE:
            if (c == '\'') { // reached the end of quotes, set state to word
                current_state = LexerState::WORD;
            } else { // still in quotes, add to buffer
                buffer += c;
            }
            break;
        case LexerState::IN_DOUBLE_QUOTE:
            if (c == '"') { // reached the end of quotes, set state to word
                current_state = LexerState::WORD;
            } else { // still in quotes, add to buffer
                buffer += c;
            }
            break;
        case LexerState::ESCAPE:
            if (c == '\n') {
                current_state = LexerState::DEFAULT;
            } else {
                buffer += c;
                current_state = LexerState::WORD;
            }
            break;
        }
        prev_state = current_state;
    }
    if (!buffer.empty()) {
        lex_return.push_back(Token(TokenType::WORD, buffer));
    }
    return lex_return;
}
