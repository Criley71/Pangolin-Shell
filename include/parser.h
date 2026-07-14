#pragma once

#include "lexer.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

enum class NodeType { // types the shell understands
    COMMAND,
    PIPE,
    AND_IF,
    OR_IF,
    SEQUENCE
};

struct ASTNode {
    NodeType type;
    std::vector<std::string> argv;  // if a command then this is used
    std::unique_ptr<ASTNode> left;  // if a binary (||, |, &&) then they need a
    std::unique_ptr<ASTNode> right; // left and right of the structure

    std::string redirect_in; // <
    std::string redirect_out; // > or >>
    bool append_out = false;
};

class Parser { // build the tree
  public:
    explicit Parser(const std::vector<Token> &tokens);
    std::unique_ptr<ASTNode> parse();

  private:
    const std::vector<Token> &tokens; // vector of tokens
    std::size_t pos;                  // position in tokens array

    std::unique_ptr<ASTNode> parse_logical();  // handles && ||
    std::unique_ptr<ASTNode> parse_pipeline(); // handles |
    std::unique_ptr<ASTNode> parse_command();  // check if token is a word, if so make a command node
                                               // loop, grab consecutive words into argv until a non word type is hit
    std::unique_ptr<ASTNode> parse_sequence();

    bool match(TokenType type); // check if current token matches a desired type
    const Token &peek() const;  // check current token
    bool at_end() const;        // check if we are out of tokens
};