#pragma once
#include "ast.h"
#include <string>
#include <iostream>

// 打印访问者类，用于实现语法树的先序遍历打印
class PrintVisitor : public ASTVisitor {
private:
    int indentation; // 缩进级别，用于格式化输出
    
    // 打印当前缩进
    void printIndentation();
    
    // 根据TokenType获取操作符的字符串表示
    std::string getOpString(TokenType op);
    
    // 根据Type获取数据类型的字符串表示
    std::string getTypeString(Type type);
    
public:
    // 构造函数
    PrintVisitor();
    
    // 访问编译单元节点
    void visit(CompUnit& node) override;
    // 访问函数定义节点
    void visit(FuncDef& node) override;
    // 访问变量声明节点
    void visit(VarDecl& node) override;
    // 访问if语句节点
    void visit(IfStmt& node) override;
    // 访问while语句节点
    void visit(WhileStmt& node) override;
    // 访问return语句节点
    void visit(ReturnStmt& node) override;
    // 访问二元表达式节点
    void visit(BinaryExpr& node) override;
    // 访问一元表达式节点
    void visit(UnaryExpr& node) override;
    // 访问函数调用表达式节点
    void visit(CallExpr& node) override;
    // 访问数组索引表达式节点
    void visit(IndexExpr& node) override;
    // 访问数字表达式节点
    void visit(NumberExpr& node) override;
    // 访问变量表达式节点
    void visit(VariableExpr& node) override;
    // 访问代码块节点
    void visit(Block& node) override;
    // 访问变量定义节点
    void visit(VarDef& node) override;
    // 访问函数形参节点
    void visit(FuncFParam& node) override;
    // 访问表达式语句节点
    void visit(ExprStmt& node) override;
    // 访问声明语句节点
    void visit(DeclStmt& node) override;
};