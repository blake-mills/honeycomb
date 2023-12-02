#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <optional>

#include "Grammar.hpp"
#include "./ast/ASTNode.hpp"

class Parser {
public:
    Parser(const std::string& grammarFile, const std::vector<Token>& tokens);
    std::optional<ASTNode> parse(Symbol s);

private:


private:
    std::vector<std::string> split(const std::string& input, const std::string& delimiter);
    void trim(std::string& str);
    void trimVector(std::vector<std::string>& vec);
private:
    Grammar grammar;
    std::optional<ASTNode> ast;
    std::vector<Token> tokens;
    std::size_t currentIndex = 0;
};