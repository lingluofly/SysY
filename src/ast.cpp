#include "../include/ast.h"

// 函数形参节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void FuncFParam::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 函数定义节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void FuncDef::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 编译单元节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void CompUnit::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 变量声明节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void VarDecl::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 变量定义节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void VarDef::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// if语句节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void IfStmt::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// while语句节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void WhileStmt::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// return语句节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void ReturnStmt::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 二元表达式节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void BinaryExpr::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 一元表达式节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void UnaryExpr::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 函数调用表达式节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void CallExpr::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 数组索引表达式节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void IndexExpr::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 数字表达式节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void NumberExpr::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 变量表达式节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void VariableExpr::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 代码块节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void Block::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 表达式语句节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void ExprStmt::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// 声明语句节点的accept方法实现
// 调用访问者的visit方法，实现访问者模式
void DeclStmt::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}