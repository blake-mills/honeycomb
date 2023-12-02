#pragma once
#include <unordered_map>

struct Token {
    enum Type {
        IDENTIFIER,
        FLOAT,
        INTEGER,
        STRING,
        COMMA,
        COLON,
        LBRACE,
        RBRACE,
        LPAREN,
        RPAREN,
        MINUS,
        RETURN,
        END_OF_FILE
    };

    static std::string str(Token::Type t) {
        std::unordered_map<Token::Type, std::string> m = {
            std::pair<Token::Type, std::string>(IDENTIFIER, "IDENTIFIER"),
            std::pair<Token::Type, std::string>(FLOAT, "FLOAT"),
            std::pair<Token::Type, std::string>(INTEGER, "INTEGER"),
            std::pair<Token::Type, std::string>(STRING, "STRING"),
            std::pair<Token::Type, std::string>(COMMA, "COMMA"),
            std::pair<Token::Type, std::string>(COLON, "COLON"),
            std::pair<Token::Type, std::string>(LBRACE, "LBRACE"),
            std::pair<Token::Type, std::string>(RBRACE, "RBRACE"),
            std::pair<Token::Type, std::string>(LPAREN, "LPAREN"),
            std::pair<Token::Type, std::string>(RPAREN, "RPAREN"),
            std::pair<Token::Type, std::string>(MINUS, "MINUS"),
            std::pair<Token::Type, std::string>(RETURN, "RETURN"),
            std::pair<Token::Type, std::string>(END_OF_FILE, "END_OF_FILE")
        };

        return m[t];
    }

    Token::Type type;
    std::string value;

    bool operator ==(const Token& rhs) {
        return type == rhs.type && value == rhs.value;
    }

    bool operator !=(const Token& rhs) {
        return type != rhs.type || value != rhs.value;
    }

    friend std::ostream& operator<<(std::ostream& out, const Token& token) {
        out << "Token(type = " << Token::str(token.type) << ", value = " << token.value << ")";
        return out;
    }
};