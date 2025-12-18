#pragma once
#include "token.h"
#include <string>

class Lexer {
private:
    std::string source;
    size_t position;
    size_t line;
    size_t column;
    char currentChar;
    
    void advance();
    char peek();
    void skipWhitespace();
    void skipComment();
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlnum(char c);
    
    Token parseNumber();
    Token parseIdentifier();
    Token parseString();
    
public:
    Lexer(const std::string& source);
    Token getNextToken();
    Token peekToken();
    Token peekToken(int n);
    size_t getLine() const { return line; }
};