#include "print_visitor.h"
#include "ast.h"
#include "token.h"
#include <iostream>

// 构造函数
PrintVisitor::PrintVisitor() : indentation(0) {}

// 打印当前缩进
void PrintVisitor::printIndentation() {
    for (int i = 0; i < indentation; ++i) {
        std::cout << "  ";
    }
}

// 根据TokenType获取操作符的字符串表示
std::string PrintVisitor::getOpString(TokenType op) {
    switch (op) {
        case TokenType::PLUS: return "+";
        case TokenType::MINUS: return "-";
        case TokenType::MUL: return "*";
        case TokenType::DIV: return "/";
        case TokenType::MOD: return "%";
        case TokenType::LT: return "<";
        case TokenType::LE: return "<=";
        case TokenType::GT: return ">";
        case TokenType::GE: return ">=";
        case TokenType::EQ: return "==";
        case TokenType::NE: return "!=";
        case TokenType::AND: return "&&";
        case TokenType::OR: return "||";
        case TokenType::ASSIGN: return "=";
        default: return "unknown operator";
    }
}

// 根据Type获取数据类型的字符串表示
std::string PrintVisitor::getTypeString(Type type) {
    switch (type) {
        case Type::INT: return "int";
        case Type::FLOAT: return "float";
        case Type::VOID: return "void";
        default: return "unknown type";
    }
}

// 访问编译单元节点
void PrintVisitor::visit(CompUnit& node) {
    printIndentation();
    std::cout << "CompUnit (1)" << std::endl;
    indentation++;
    
    for (auto& decl : node.getDecls()) {
        decl->accept(*this);
    }
    
    for (auto& funcDef : node.getFuncDefs()) {
        funcDef->accept(*this);
    }
    
    indentation--;
}

// 访问函数定义节点
void PrintVisitor::visit(FuncDef& node) {
    printIndentation();
    std::cout << "FuncDef (1)" << std::endl;
    indentation++;
    
    printIndentation();
    std::cout << "FuncType (1)" << std::endl;
    indentation++;
    printIndentation();
    std::cout << "Type: " << getTypeString(node.getReturnType()) << std::endl;
    indentation--;
    
    printIndentation();
    std::cout << "Ident: " << node.getName() << std::endl;
    
    printIndentation();
    std::cout << "LPARENT" << std::endl;
    
    for (auto& param : node.getParams()) {
        param->accept(*this);
    }
    
    printIndentation();
    std::cout << "RPARENT" << std::endl;
    
    if (node.getBody()) {
        node.getBody()->accept(*this);
    }
    
    indentation--;
}

// 访问变量声明节点
void PrintVisitor::visit(VarDecl& node) {
    printIndentation();
    std::cout << "VarDecl (1)" << std::endl;
    indentation++;
    
    printIndentation();
    std::cout << "BType (1)" << std::endl;
    indentation++;
    printIndentation();
    std::cout << "Type: " << getTypeString(node.getType()) << std::endl;
    indentation--;
    
    for (auto& varDef : node.getVarDefs()) {
        varDef->accept(*this);
    }
    
    indentation--;
}

// 访问if语句节点
void PrintVisitor::visit(IfStmt& node) {
    printIndentation();
    std::cout << "IfStmt (1)" << std::endl;
    indentation++;
    
    printIndentation();
    std::cout << "LPARENT" << std::endl;
    
    node.getCondition()->accept(*this);
    
    printIndentation();
    std::cout << "RPARENT" << std::endl;
    
    node.getThenStmt()->accept(*this);
    
    if (node.getElseStmt()) {
        printIndentation();
        std::cout << "Else" << std::endl;
        node.getElseStmt()->accept(*this);
    }
    
    indentation--;
}

// 访问while语句节点
void PrintVisitor::visit(WhileStmt& node) {
    printIndentation();
    std::cout << "WhileStmt (1)" << std::endl;
    indentation++;
    
    printIndentation();
    std::cout << "LPARENT" << std::endl;
    
    node.getCondition()->accept(*this);
    
    printIndentation();
    std::cout << "RPARENT" << std::endl;
    
    node.getBody()->accept(*this);
    
    indentation--;
}

// 访问return语句节点
void PrintVisitor::visit(ReturnStmt& node) {
    printIndentation();
    std::cout << "ReturnStmt (1)" << std::endl;
    indentation++;
    
    if (node.getExpr()) {
        node.getExpr()->accept(*this);
    }
    
    indentation--;
}

// 访问二元表达式节点
void PrintVisitor::visit(BinaryExpr& node) {
    printIndentation();
    std::cout << "BinaryExpr (1)" << std::endl;
    indentation++;
    
    printIndentation();
    std::cout << "Op: " << getOpString(node.getOp()) << std::endl;
    
    node.getLeft()->accept(*this);
    node.getRight()->accept(*this);
    
    indentation--;
}

// 访问一元表达式节点
void PrintVisitor::visit(UnaryExpr& node) {
    printIndentation();
    std::cout << "UnaryExpr (1)" << std::endl;
    indentation++;
    
    printIndentation();
    std::cout << "Op: " << getOpString(node.getOp()) << std::endl;
    
    node.getOperand()->accept(*this);
    
    indentation--;
}

// 访问函数调用表达式节点
void PrintVisitor::visit(CallExpr& node) {
    printIndentation();
    std::cout << "CallExpr (1)" << std::endl;
    indentation++;
    
    printIndentation();
    std::cout << "Ident: " << node.getCallee() << std::endl;
    
    printIndentation();
    std::cout << "LPARENT" << std::endl;
    
    for (auto& arg : node.getArgs()) {
        arg->accept(*this);
    }
    
    printIndentation();
    std::cout << "RPARENT" << std::endl;
    
    indentation--;
}

// 访问数组索引表达式节点
void PrintVisitor::visit(IndexExpr& node) {
    printIndentation();
    std::cout << "IndexExpr (1)" << std::endl;
    indentation++;
    
    node.getBase()->accept(*this);
    
    printIndentation();
    std::cout << "LBRACK" << std::endl;
    
    node.getIndex()->accept(*this);
    
    printIndentation();
    std::cout << "RBRACK" << std::endl;
    
    indentation--;
}

// 访问数字表达式节点
void PrintVisitor::visit(NumberExpr& node) {
    printIndentation();
    std::cout << "Number (1)" << std::endl;
    indentation++;
    
    printIndentation();
    if (node.getType() == Type::INT) {
        std::cout << "INTCON: " << node.getIntValue() << std::endl;
    } else {
        std::cout << "FLOATCON: " << node.getFloatValue() << std::endl;
    }
    
    indentation--;
}

// 访问变量表达式节点
void PrintVisitor::visit(VariableExpr& node) {
    printIndentation();
    std::cout << "Lval (1)" << std::endl;
    indentation++;
    
    printIndentation();
    std::cout << "Ident: " << node.getName() << std::endl;
    
    indentation--;
}

// 访问代码块节点
void PrintVisitor::visit(Block& node) {
    printIndentation();
    std::cout << "Block (1)" << std::endl;
    indentation++;
    
    printIndentation();
    std::cout << "LBRACE" << std::endl;
    
    for (auto& stmt : node.getStatements()) {
        stmt->accept(*this);
    }
    
    printIndentation();
    std::cout << "RBRACE" << std::endl;
    
    indentation--;
}

// 访问变量定义节点
void PrintVisitor::visit(VarDef& node) {
    printIndentation();
    std::cout << "VarDef (1)" << std::endl;
    indentation++;
    
    printIndentation();
    std::cout << "Ident: " << node.getName() << std::endl;
    
    if (node.getInitExpr()) {
        printIndentation();
        std::cout << "ASSIGN" << std::endl;
        
        printIndentation();
        std::cout << "InitVal (1)" << std::endl;
        indentation++;
        
        node.getInitExpr()->accept(*this);
        
        indentation--;
    }
    
    printIndentation();
    std::cout << "SEMICN" << std::endl;
    
    indentation--;
}

// 访问函数形参节点
void PrintVisitor::visit(FuncFParam& node) {
    printIndentation();
    std::cout << "FuncFParam (1)" << std::endl;
    indentation++;
    
    printIndentation();
    std::cout << "Type: " << getTypeString(node.getType()) << std::endl;
    
    printIndentation();
    std::cout << "Ident: " << node.getName() << std::endl;
    
    indentation--;
}

// 访问表达式语句节点
void PrintVisitor::visit(ExprStmt& node) {
    printIndentation();
    std::cout << "Stmt (1)" << std::endl;
    indentation++;
    
    node.getExpr()->accept(*this);
    
    printIndentation();
    std::cout << "SEMICN" << std::endl;
    
    indentation--;
}

// 访问声明语句节点
void PrintVisitor::visit(DeclStmt& node) {
    printIndentation();
    std::cout << "Stmt (1)" << std::endl;
    indentation++;
    
    node.getDecl()->accept(*this);
    
    indentation--;
}