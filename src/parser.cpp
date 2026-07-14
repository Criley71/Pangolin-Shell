#include "../include/parser.h"

// ast handles order of operations for command lines

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), pos(0) {
}

std::unique_ptr<ASTNode> Parser::parse_command() {
    if (at_end() || peek().type != TokenType::WORD) {
        throw std::runtime_error("Expected command");
    }
    auto node = std::make_unique<ASTNode>();
    node->type = NodeType::COMMAND;

    auto is_valid_command_token = [&](const Token &t) {
        return t.type == TokenType::WORD ||
               t.lexeme == "<" ||
               t.lexeme == ">" ||
               t.lexeme == ">>";
    };
    while (!at_end() && is_valid_command_token(peek())) {
        if (tokens[pos].lexeme == "<") {
            if (pos + 1 >= tokens.size() || tokens[pos + 1].type != TokenType::WORD) {
                throw std::runtime_error(" syntax error near unexpected token `newline'");
            }
            node->redirect_in = tokens[pos + 1].lexeme;
            pos += 2;
            continue;
        }
        if (tokens[pos].lexeme == ">" || tokens[pos].lexeme == ">>") {
            if (pos + 1 >= tokens.size() || tokens[pos + 1].type != TokenType::WORD) {
                throw std::runtime_error("syntax error near unexpected token `newline'");
            }
            node->redirect_out = tokens[pos + 1].lexeme;
            if (tokens[pos].lexeme == ">>") {
                node->append_out = true;
            }
            pos += 2;
            continue;
        }
        node->argv.push_back(tokens[pos].lexeme);

        pos++;
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::parse_pipeline() {
    auto left = parse_command(); // we have already read the left command
    while (match(TokenType::PIPE)) {
        auto pipe = std::make_unique<ASTNode>();
        pipe->type = NodeType::PIPE;
        pipe->left = std::move(left);
        pipe->right = parse_command(); // now we need to get the right command
        left = std::move(pipe);
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parse_logical() {
    auto left = parse_pipeline(); // get left side of expression
    while (true) {                // loop making a node for the specific type
        if (match(TokenType::AND_IF)) {
            auto node = std::make_unique<ASTNode>();
            node->type = NodeType::AND_IF;
            node->left = std::move(left);   // made left above
            node->right = parse_pipeline(); // parse the right side
            left = std::move(node);
        } else if (match(TokenType::OR_IF)) {
            auto node = std::make_unique<ASTNode>();
            node->type = NodeType::OR_IF;
            node->left = std::move(left);   // made left above
            node->right = parse_pipeline(); // parse the right side
            left = std::move(node);
        } else {
            break;
        }
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parse() {
    auto node = parse_sequence();
    if (!at_end()) {
        throw std::runtime_error("Unexpected token at end of input");
    }
    return node;
}

bool Parser::match(TokenType type) {
    if (!at_end() && tokens[pos].type == type) {
        pos++;
        return true;
    }
    return false;
}

const Token &Parser::peek() const {
    return tokens[pos];
}

bool Parser::at_end() const {
    return pos >= tokens.size();
}

std::unique_ptr<ASTNode> Parser::parse_sequence() {
    // grab left side
    auto left = parse_logical();
    while (match(TokenType::SEMICOLON)) {
        auto node = std::make_unique<ASTNode>();
        node->type = NodeType::SEQUENCE;
        node->left = std::move(left);
        node->right = parse_logical();
        left = std::move(node);
    }
    return left;
}