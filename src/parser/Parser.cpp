#include "Parser.hpp"

Parser::Parser(const std::string& grammarFile, const std::vector<Token>& tokens) {
    this->tokens = tokens;
    
    std::ifstream fStream(grammarFile);
    if (!fStream) {
        throw std::runtime_error("Error: Cannot open the file " + grammarFile);
    }

    std::string line;
    while (std::getline(fStream, line)) {
        if (line.empty()) {
            continue;
        }

        std::vector<std::string> tokens = split(line, "::=");
        trimVector(tokens);

        std::string id = tokens[0];
        trim(id);

        tokens.erase(tokens.begin());

        std::vector<std::string> derivationList = split(tokens[0], "|");
        for (auto& s : derivationList) {
            std::vector<std::string> derivation;
            std::istringstream iss(s);
            std::string item;
            while (iss >> item) {
                if (!item.empty()) {
                    derivation.push_back(item);
                }
            }
            grammar.addDerivation(id, derivation);
        }
    }
}

std::vector<std::string> Parser::split(const std::string& input, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
        end = input.find(delimiter, start);
    }

    tokens.push_back(input.substr(start, end));
    return tokens;
}

void Parser::trim(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), [](char& c) { return std::isspace(c); }));
}

void Parser::trimVector(std::vector<std::string>& vec) {
    for (auto s : vec) {
        trim(s);
    }
}

std::optional<ASTNode> Parser::parse(Symbol s) {
    if (s.isNonTerminal()) {
        Derivations derivations = grammar[s.value];
        if (!ast.has_value()) {
            ast = { { .value = s.value }, {} };
        }

        ASTNode originalAST = *ast;
        originalAST.prettyPrint();

        std::size_t initialIndex = currentIndex;

        for (Derivation d : derivations.derivations) {
            currentIndex = initialIndex;
            bool match = true;
            ASTNode temp = { { .value = s.value }, .children = {} };
            for (Symbol symbol : d.symbols) {
                if (currentIndex >= tokens.size()) {
                    match = false;
                    break;
                }

                if (symbol.isLiteral()) {
                    printf("Literal symbol check: %s == %s?\t%d\n",
                            symbol.value.c_str(), 
                            tokens[currentIndex].value.c_str(),
                            symbol.cmp(tokens[currentIndex].value));

                    if (symbol.cmp(tokens[currentIndex].value)) {
                        ++currentIndex;
                        continue;
                    } else {
                        printf("Did not match, so returning nullopt\n");
                        match = false;
                        return std::nullopt;
                    }
                } else if (symbol.isNonTerminal()) {
                    printf("Expanding nonterminal: %s\n", symbol.value.c_str());
                    std::optional<ASTNode> childNode = parse(symbol);
                    if (childNode.has_value()) {
                        temp.children.push_back(*childNode);
                    } else {
                        match = false;
                        return std::nullopt;
                    }
                } else if (!symbol.isNonTerminal()) {
                    printf("Type symbol check: %s == %s?\t%d\n",
                            symbol.value.c_str(), 
                            Token::str(tokens[currentIndex].type).c_str(),
                            symbol.value == Token::str(tokens[currentIndex].type));

                    // Symbol is terminal in this case, and if it matches
                    // we add it to the AST
                    if (symbol.value == Token::str(tokens[currentIndex].type)) {
                        ASTNode node = { .value = tokens[currentIndex], .children = {} };
                        temp.children.push_back(node);
                        break;
                    } else {
                        match = false;
                        break;
                    }
                }
            }


            printf("Match?\t%d\n", match);
            if (match) {
                *ast = temp;
                return ast;
            }
        }

        currentIndex = initialIndex;
        return std::nullopt;
    }

    throw std::runtime_error("No tree");
}