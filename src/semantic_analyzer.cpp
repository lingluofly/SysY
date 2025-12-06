#include "../include/semantic_analyzer.h"
#include "../include/symbol_table.h"
#include "../include/ast.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

// 将Type枚举转换为字符串表示
// 便于在错误和警告消息中显示类型名称
std::string typeToString(Type type) {
    switch (type) {
        case Type::INT: return "int"; // 整数类型
        case Type::FLOAT: return "float"; // 浮点数类型
        case Type::VOID: return "void"; // 空类型
        default: return "unknown"; // 未知类型
    }
}

// 访问编译单元节点
// 遍历并访问编译单元中的所有声明和函数定义
void SemanticAnalyzer::visit(CompUnit& node) {
    // 遍历所有声明（变量声明等）
    for (auto& decl : node.getDecls()) {
        decl->accept(*this);
    }
    
    // 遍历所有函数定义
    for (auto& func : node.getFuncDefs()) {
        func->accept(*this);
    }
}

// 访问函数定义节点
// 处理函数的符号表条目、参数、函数体和返回值检查
void SemanticAnalyzer::visit(FuncDef& node) {
    // 设置当前函数的信息
    currentFunction = node.getName();
    currentReturnType = node.getReturnType();
    hasReturnStmt = false; // 初始化是否有返回语句的标志
    
    // 添加函数到全局符号表
    SymbolEntry funcEntry(SymbolEntry::Kind::FUNCTION, node.getReturnType());
    symbolTable.insert(node.getName(), funcEntry);
    
    // 进入函数的局部作用域
    symbolTable.enterScope();
    
    // 处理函数的所有参数
    for (auto& param : node.getParams()) {
        param->accept(*this);
    }
    
    // 处理函数体
    if (node.getBody()) {
        node.getBody()->accept(*this);
    }
    
    // 检查非void函数是否有返回语句
    if (node.getReturnType() != Type::VOID && !hasReturnStmt) {
        std::cerr << "Warning: function '" << node.getName() 
                  << "' should return a value" << std::endl;
    }
    
    // 退出函数的局部作用域
    symbolTable.exitScope();
}

// 访问变量声明节点
// 遍历并处理所有变量定义
void SemanticAnalyzer::visit(VarDecl& node) {
    // 处理所有变量定义
    for (auto& varDef : node.getVarDefs()) {
        varDef->accept(*this);
    }
}

// 访问if语句节点
// 处理条件表达式、then语句块和else语句块
void SemanticAnalyzer::visit(IfStmt& node) {
    // 检查条件表达式
    if (node.getCondition()) {
        node.getCondition()->accept(*this);
    }
    
    // 处理then语句块
    if (node.getThenStmt()) {
        node.getThenStmt()->accept(*this);
    }
    
    // 处理else语句块
    if (node.getElseStmt()) {
        node.getElseStmt()->accept(*this);
    }
}

// 访问while语句节点
// 处理循环条件和循环体
void SemanticAnalyzer::visit(WhileStmt& node) {
    bool wasInLoop = isInLoop; // 保存之前的循环状态
    isInLoop = true; // 设置当前在循环中
    
    // 检查循环条件表达式
    if (node.getCondition()) {
        node.getCondition()->accept(*this);
    }
    
    // 处理循环体
    if (node.getBody()) {
        node.getBody()->accept(*this);
    }
    
    isInLoop = wasInLoop; // 恢复之前的循环状态
}

// 访问return语句节点
// 处理返回表达式，并检查返回类型是否匹配
void SemanticAnalyzer::visit(ReturnStmt& node) {
    hasReturnStmt = true; // 标记函数有返回语句
    
    // 处理返回表达式
    if (node.getExpr()) {
        node.getExpr()->accept(*this);
        
        // 检查void函数是否返回值
        if (currentReturnType == Type::VOID) {
            std::cerr << "Error: cannot return a value from a void function" << std::endl;
        }
    } else {
        // 没有返回表达式
        // 检查非void函数是否没有返回值
        if (currentReturnType != Type::VOID) {
            std::cerr << "Error: must return a value from non-void function" << std::endl;
        }
    }
}

// 访问二元表达式节点
// 处理左操作数和右操作数
void SemanticAnalyzer::visit(BinaryExpr& node) {
    // 处理左操作数
    if (node.getLeft()) {
        node.getLeft()->accept(*this);
    }
    
    // 处理右操作数
    if (node.getRight()) {
        node.getRight()->accept(*this);
    }
}

// 访问一元表达式节点
// 处理操作数
void SemanticAnalyzer::visit(UnaryExpr& node) {
    // 处理操作数
    if (node.getOperand()) {
        node.getOperand()->accept(*this);
    }
}

// 访问函数调用表达式节点
// 处理所有参数表达式
void SemanticAnalyzer::visit(CallExpr& node) {
    // 处理所有参数表达式
    for (auto& arg : node.getArgs()) {
        if (arg) {
            arg->accept(*this);
        }
    }
}

// 访问数组索引表达式节点
// 处理数组基址和索引表达式
void SemanticAnalyzer::visit(IndexExpr& node) {
    // 处理数组基址
    if (node.getBase()) {
        node.getBase()->accept(*this);
    }
    
    // 处理索引表达式
    if (node.getIndex()) {
        node.getIndex()->accept(*this);
    }
}

// 访问数字表达式节点
// 数字表达式无需特殊处理
void SemanticAnalyzer::visit(NumberExpr& node) {
    // 数字表达式无需特殊处理
}

// 访问变量表达式节点
// 检查变量是否已声明
void SemanticAnalyzer::visit(VariableExpr& node) {
    // 检查变量是否已在符号表中声明
    SymbolEntry* entry = symbolTable.lookup(node.getName());
    if (!entry) {
        std::cerr << "Error: use of undeclared variable '" << node.getName() << "'" << std::endl;
    }
}

// 访问语句块节点
// 处理语句块的作用域和所有语句
void SemanticAnalyzer::visit(Block& node) {
    symbolTable.enterScope(); // 进入语句块的局部作用域
    
    // 处理所有语句
    for (auto& stmt : node.getStatements()) {
        if (stmt) {
            stmt->accept(*this);
        }
    }
    
    symbolTable.exitScope(); // 退出语句块的局部作用域
}

// 访问变量定义节点
// 处理变量的符号表条目和初始化表达式
void SemanticAnalyzer::visit(VarDef& node) {
    // 创建变量的符号表项
    SymbolEntry varEntry(SymbolEntry::Kind::VARIABLE, Type::INT); // 默认为int类型
    varEntry.isArray = node.getIsArray(); // 设置是否为数组
    
    // 添加变量到当前作用域的符号表
    if (!symbolTable.insert(node.getName(), varEntry)) {
        std::cerr << "Error: redefinition of variable '" << node.getName() << "'" << std::endl;
    }
    
    // 处理初始化表达式
    if (node.getInitExpr()) {
        node.getInitExpr()->accept(*this);
    }
}

// 访问函数参数节点
// 处理参数的符号表条目
void SemanticAnalyzer::visit(FuncFParam& node) {
    // 创建参数的符号表项
    SymbolEntry paramEntry(SymbolEntry::Kind::PARAMETER, node.getType());
    paramEntry.isArray = node.getIsArray(); // 设置是否为数组参数
    
    // 添加参数到当前作用域的符号表
    if (!symbolTable.insert(node.getName(), paramEntry)) {
        std::cerr << "Error: redefinition of parameter '" << node.getName() << "'" << std::endl;
    }
}

// 访问表达式语句节点
// 处理表达式语句
void SemanticAnalyzer::visit(ExprStmt& node) {
    // 处理表达式语句
    if (node.getExpr()) {
        node.getExpr()->accept(*this);
    }
}

// 检查类型兼容性
// 确保两个类型匹配，如果不匹配则输出错误消息
void SemanticAnalyzer::checkTypeCompatibility(Type t1, Type t2, const std::string& context) {
    if (t1 != t2) {
        std::cerr << "Error: type mismatch in '" << context << "': expected '" 
                  << typeToString(t1) << "', got '" << typeToString(t2) << "'" << std::endl;
    }
}

// 检查数组维度
// 确保数组的索引数量与数组的维度数量匹配
void SemanticAnalyzer::checkArrayDimensions(const std::vector<std::unique_ptr<Expr>>& indices, 
                                             const std::vector<int>& dims) {
    if (indices.size() != dims.size()) {
        std::cerr << "Error: array dimension mismatch: expected '" 
                  << dims.size() << "' indices, got '" << indices.size() << "'" << std::endl;
    }
}
