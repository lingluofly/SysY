#pragma once
#include "token.h"
#include "ast.h"
#include <string>
#include <memory>

// 使用前向声明替代包含Lexer.h
class Lexer;

class Parser {
private:
    Lexer& lexer;
    Token currentToken;
    
    // 解析方法
    std::unique_ptr<FuncDef> parseFuncDef();
    std::unique_ptr<Expr> parseExpression();
    std::unique_ptr<Expr> parseBinaryExpression();
    std::unique_ptr<Expr> parseUnaryExpression();
    std::unique_ptr<Expr> parsePrimaryExpression();
    std::unique_ptr<Stmt> parseStatement();
    
    // 辅助方法
    void parseFuncParams(std::vector<std::unique_ptr<FuncFParam>>& params);
    std::unique_ptr<VarDecl> parseVarDef();
    void parseStatementList(Block& block);
    void consumeToken(TokenType expectedType);
    
public:
    Parser(Lexer& lexer);
    std::unique_ptr<CompUnit> parse();
    size_t getLine() const { return lexer.getLine(); }
};