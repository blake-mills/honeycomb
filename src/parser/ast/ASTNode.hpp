#pragma once
#include "../../token/Token.hpp"

struct ASTNode {
    Token value;
    std::vector<ASTNode> children;

    void prettyPrint(const std::string& prefix = "", bool isLast = true) const {
        std::cout << prefix;
        std::cout << (isLast ? "└── " : "├── ");
        std::cout << value.value << std::endl;

        const std::string newPrefix = prefix + (isLast ? "    " : "│   ");

        for (size_t i = 0; i < children.size(); ++i) {
            children[i].prettyPrint(newPrefix, (i == children.size() - 1));
        }
    }
};