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
    
    void eat(TokenType type);
    
    // 解析方法
    std::unique_ptr<CompUnit> parseCompUnit();
    std::unique_ptr<Decl> parseDecl();
    std::unique_ptr<FuncDef> parseFuncDef();
    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Expr> parseAddExp();
    std::unique_ptr<Expr> parseMulExp();
    std::unique_ptr<Expr> parseUnaryExp();
    
    // 辅助方法
    void parseFuncParams(std::vector<std::unique_ptr<FuncFParam>>& params);
    void parseVarDef();
    void consumeToken(TokenType expectedType);
    
public:
    Parser(Lexer& lexer);
    std::unique_ptr<CompUnit> parse();
};