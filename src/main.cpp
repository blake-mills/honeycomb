#include <iostream>
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

int main(int argc, char* argv[]) {
    char* fileName = nullptr;

    if (argc > 1) {
        fileName = argv[1];
    }
    else {
        printf("Usage:\n");
        printf("./honeycomb <filename>\n");
    }

    try {
        Lexer lexer(fileName);
        lexer.lex();
        std::vector<Token> tokens = lexer.get();

        const std::string grammarFileName = "../src/hc/grammar/grammar.bnf";
        Parser parser(grammarFileName, tokens);

        std::optional<ASTNode> ast = parser.parse({ .value = "<program>" });
        if (ast.has_value()) {
            ast.value().prettyPrint();
        } else {
            std::cout << "Failed" << std::endl;
        }

    } catch (std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
    } catch (std::ios_base::failure& e) {
        std::cout << e.what() << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}