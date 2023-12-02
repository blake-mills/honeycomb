#pragma once
#include <string>
#include <regex>
#include <iostream>

#include "../token/Token.hpp"

class Lexer {
public:
    Lexer(const std::string& src);
    void lex();
    std::vector<Token> get() const { return tokens; }

private:
    void error(std::string& msg);
    void advance();
    void skip();
    char peek();
    bool contains(const std::vector<std::string>& list, const std::string& t);
    Token getIdentifier();
    Token getNumber();
    Token getString();
    Token getSymbol();
    Token next();
private:
    std::vector<Token> tokens;
    std::string contents;
    std::size_t position;
    char current;

private:
    const std::vector<std::string> STATIC_SYMBOLS = {
        ",",
        ":",
        "{",
        "}",
        "(",
        ")"
    };

    Token EOF_TOKEN = { .type = Token::Type::END_OF_FILE, .value = "\0" };
};