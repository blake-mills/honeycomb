cmake_minimum_required(VERSION 3.26)
project(honeycomb)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/lexer)
include_directories(src/parser)
include_directories(src/parser/ast)
include_directories(src/token)

add_executable(honeycomb
        src/lexer/Lexer.cpp
        src/lexer/Lexer.hpp
        src/parser/ast/ASTNode.hpp
        src/parser/Grammar.hpp
        src/parser/Parser.cpp
        src/parser/Parser.hpp
        src/token/Token.hpp
        src/main.cpp)
