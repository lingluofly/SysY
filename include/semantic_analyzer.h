#pragma once
#include "ast.h"
#include "symbol_table.h"
#include <string>
#include <vector>

class SemanticAnalyzer : public ASTVisitor {
private:
    SymbolTable symbolTable;
    std::string currentFunction;
    Type currentReturnType;
    
    bool isInLoop;
    bool hasReturnStmt;
    
public:
    SemanticAnalyzer() : isInLoop(false), hasReturnStmt(false) {}
    
    void visit(CompUnit& node) override;
    void visit(FuncDef& node) override;
    void visit(VarDecl& node) override;
    void visit(IfStmt& node) override;
    void visit(WhileStmt& node) override;
    void visit(ReturnStmt& node) override;
    void visit(BinaryExpr& node) override;
    void visit(UnaryExpr& node) override;
    void visit(CallExpr& node) override;
    void visit(IndexExpr& node) override;
    void visit(NumberExpr& node) override;
    void visit(VariableExpr& node) override;
    void visit(Block& node) override;
    void visit(VarDef& node) override;
    void visit(FuncFParam& node) override;
    void visit(ExprStmt& node) override;
    
    void checkTypeCompatibility(Type t1, Type t2, const std::string& context);
    void checkArrayDimensions(const std::vector<std::unique_ptr<Expr>>& indices, 
                             const std::vector<int>& dims);
};