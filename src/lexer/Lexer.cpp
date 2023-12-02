#include <stdexcept>
#include <sstream>
#include <fstream>

#include "Lexer.hpp"

Lexer::Lexer(const std::string& src)
:   position( 0 )
{
    if (src.empty()) {
        throw std::invalid_argument("File name cannot be empty.\n");
    }

    std::ifstream fileStream(src);
    if (!fileStream) {
        throw std::ios_base::failure("Cannot open the file: " + src);
    }

    contents = "";
    std::string line;
    while (std::getline(fileStream, line)) {
        contents += line + "\n";
    }

    if (!contents.empty()) {
        current = contents[0];
    }
    fileStream.close();
}

void Lexer::lex() {
    Token token;
    
    do {
        token = next();
        tokens.push_back(token);
    } while (token != EOF_TOKEN);
}

void Lexer::error(std::string& msg) {
    throw std::runtime_error(msg);
}

void Lexer::advance() {
    ++position;
    if (position < contents.size()) {
        current = contents[position];
    } else {
        current = '\0';
    }
}

void Lexer::skip() {
    while (current != '\0' && std::isspace(current)) {
        advance();
    }
}

char Lexer::peek() {
    if (current != '\0' && position + 1 < contents.size()) {
        return contents[position + 1];
    }
    else {
        return '\0';
    }
}

bool Lexer::contains(const std::vector<std::string>& list, const std::string& t) {
    for (auto element : list) {
        if (element == t) {
            return true;
        }
    }
    return false;
}

Token Lexer::getIdentifier() {
    std::string id = "";
    while (std::isalnum(current) && current != '\0') {
        id += current;
        advance();
    }

    return { .type = Token::Type::IDENTIFIER, .value = id };
}

Token Lexer::getNumber() {
    std::string num = "";
    short dotCounter = 0;
    short negativeCounter = 0;
    short localPosition = 0;

    while ( 
            (std::isdigit(current) || current == '.') &&
            dotCounter <= 1 &&
            negativeCounter <= 1 &&
            current != '\0'
        ) {
            if (current == '-') {
                ++negativeCounter;
                ++localPosition;
                if (localPosition > 1) {
                    throw std::runtime_error("Error: Invalid negative sign placement in number");
                }
                if (negativeCounter > 1) {
                    throw std::runtime_error("Error: Too many negative signs in number");
                }
            }
            else if (current == '.') {
                ++dotCounter;
                num += current;
                if (dotCounter > 1) {
                    throw std::runtime_error("Error: Invalid floating point number.");
                }
            }
            else {
                num += current;
            }

            advance();
    }

    Token::Type type;
    if (dotCounter > 0) {
        type = Token::Type::FLOAT;
    }
    else {
        type = Token::Type::INTEGER;
    }

    return { .type = type, .value = num };
}

Token Lexer::getString() {
    std::string str = "";

    // Skips the initial " at the start of the string
    advance();
    while (current != '\0' && current != '"') {
        str += current;
        advance();
    }
    if (current == '\0') {
        throw std::runtime_error("Error: String is not closed. Missing quotation marks.");
    }
    
    // Consume the last " that closes the string
    advance();
    return { .type = Token::Type::STRING, .value = str };
}

Token Lexer::getSymbol() {
    std::string value = "";
    if (current == ',') {
        value += current;
        advance();
        return { .type = Token::Type::COMMA, .value = value };
    }
    else if (current == ':') {
        value += current;
        advance();
        return { .type = Token::Type::COLON, .value = value };
    }
    else if (current == '{') {
        value += current;
        advance();
        return { .type = Token::Type::LBRACE, .value = value };
    }
    else if (current == '}') {
        value += current;
        advance();
        return { .type = Token::Type::RBRACE, .value = value };
    }
    else if (current == '(') {
        value += current;
        advance();
        return { .type = Token::Type::LPAREN, .value = value };
    }
    else if (current == ')') {
        value += current;
        advance();
        return { .type = Token::Type::RPAREN, .value = value };
    }
    else {
        throw std::runtime_error("Invalid smybol: " + value);
    }
}

Token Lexer::next() {
    do {
        if (std::isspace(current)) {
            skip();
            continue;
        }
        else if (std::isalpha(current)) {
            return getIdentifier();
        }
        else if (std::isdigit(current)) {
            return getNumber();
        }
        else if (current == '"') {
            return getString();
        }
        else if (current == '-') {
            std::string value = "";
            value += current;
            char nextChar = peek();
            if (nextChar != '\0') {
                advance();
                value += current;
                return { .type = Token::Type::RETURN, .value = value };
            }
            return { .type = Token::Type::MINUS, .value = std::to_string(current) };
        }
        else if (contains(Lexer::STATIC_SYMBOLS, std::string(&current, 1))) {
            return getSymbol();
        }
        advance();
    } while (current != '\0');
    return EOF_TOKEN;
}