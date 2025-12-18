#include "../include/semantic_analyzer.h"
#include "../include/symbol_table.h"
#include "../include/ast.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <unordered_set>

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
    // 检查函数是否已定义
    SymbolEntry* existingEntry = symbolTable.lookup(node.getName());
    if (existingEntry) {
        std::cerr << "Error type 4 at line " << node.getLine() << " : redefinition of function '" << node.getName() << "'" << std::endl;
    }
    
    // 设置当前函数的信息
    currentFunction = node.getName();
    currentReturnType = node.getReturnType();
    hasReturnStmt = false; // 初始化是否有返回语句的标志
    
    // 检查参数是否重复并收集参数类型
    std::unordered_set<std::string> paramNames;
    std::vector<Type> paramTypes;
    for (const auto& param : node.getParams()) {
        if (paramNames.find(param->getName()) != paramNames.end()) {
            std::cerr << "Error type 2 at line " << param->getLine() << " : duplicate parameter name '" << param->getName() << "' in function '" << node.getName() << "'" << std::endl;
        } else {
            paramNames.insert(param->getName());
            paramTypes.push_back(param->getType());
        }
    }
    
    // 添加函数到全局符号表
    SymbolEntry funcEntry(SymbolEntry::Kind::FUNCTION, node.getReturnType());
    funcEntry.paramCount = node.getParams().size();
    funcEntry.paramTypes = paramTypes;
    symbolTable.insert(node.getName(), funcEntry);
    
    // 进入函数的局部作用域
    symbolTable.enterScope();
    
    // 处理参数（添加到函数的局部作用域）
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
    Type varType = node.getType(); // 获取变量类型
    
    // 检查是否声明void类型变量
    if (varType == Type::VOID) {
        std::cerr << "Error type 11 at line " << node.getLine() << " : variable declaration with void type" << std::endl;
        return;
    }
    
    // 处理所有变量定义
    for (auto& varDef : node.getVarDefs()) {
        // 直接处理变量定义，而不是调用accept，这样可以传递类型信息
        std::string varName = varDef->getName();
        
        // 创建变量的符号表项
        SymbolEntry varEntry(SymbolEntry::Kind::VARIABLE, varType);
        varEntry.isArray = varDef->getIsArray(); // 设置是否为数组
        
        // 添加变量到当前作用域的符号表
        if (!symbolTable.insert(varName, varEntry)) {
                std::cerr << "Error type 2 at line " << varDef->getLine() << " : redefinition of variable '" << varName << "'" << std::endl;
            }
        
        // 处理初始化表达式
        if (varDef->getInitExpr()) {
            varDef->getInitExpr()->accept(*this);
            
            // 检查初始化表达式类型是否匹配
            Type initType = varDef->getInitExpr()->getType();
            if (initType != varType) {
                std::cerr << "Error type 11 at line " << varDef->getInitExpr()->getLine() << " : type mismatch in initialization of variable '" << varName 
                          << "': expected '" << typeToString(varType) 
                          << "', got '" << typeToString(initType) << "'" << std::endl;
            }
        }
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
        std::cerr << "Error type 10 at line " << node.getLine() << " : cannot return a value from a void function" << std::endl;
    } else {
            // 检查返回值类型是否匹配
            Type returnType = node.getExpr()->getType();
            if (returnType != currentReturnType) {
                std::cerr << "Error type 10 at line " << node.getLine() << " : return type mismatch: expected '" 
                          << typeToString(currentReturnType) 
                          << "', got '" << typeToString(returnType) << "'" << std::endl;
            }
        }
    } else {
        // 没有返回表达式
        // 检查非void函数是否没有返回值
    if (currentReturnType != Type::VOID) {
        std::cerr << "Error type 10 at line " << node.getLine() << " : must return a value from non-void function" << std::endl;
    }
    }
}

// 访问二元表达式节点
// 检查操作数类型是否匹配
void SemanticAnalyzer::visit(BinaryExpr& node) {
    // 处理左操作数
    if (node.getLeft()) {
        node.getLeft()->accept(*this);
    }
    
    // 处理右操作数
    if (node.getRight()) {
        node.getRight()->accept(*this);
    }
    
    // 检查类型匹配
    if (node.getLeft() && node.getRight()) {
        Type leftType = node.getLeft()->getType();
        Type rightType = node.getRight()->getType();
        
        if (leftType != rightType) {
            std::cerr << "Error type 11 at line " << node.getLine() << " : type mismatch in binary expression: expected '" 
                      << typeToString(leftType) << "', got '" << typeToString(rightType) << "'" << std::endl;
        }
        
        // 设置二元表达式的类型
        node.setType(leftType);
        
        // 检查赋值操作符
        if (node.getOp() == TokenType::ASSIGN) {
            // 检查左操作数是否为变量或数组元素
            if (dynamic_cast<VariableExpr*>(node.getLeft()) || dynamic_cast<IndexExpr*>(node.getLeft())) {
                // 检查是否给常量赋值
                if (dynamic_cast<VariableExpr*>(node.getLeft())) {
                    VariableExpr* varExpr = dynamic_cast<VariableExpr*>(node.getLeft());
                    SymbolEntry* entry = symbolTable.lookup(varExpr->getName());
                    if (entry && entry->kind == SymbolEntry::Kind::CONSTANT) {
                        std::cerr << "Error type 11 at line " << node.getLine() << " : assignment to constant variable '" << varExpr->getName() << "'" << std::endl;
                    }
                }
            } else {
                std::cerr << "Error type 11 at line " << node.getLine() << " : left operand of assignment must be a variable or array element" << std::endl;
            }
        }
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
// 检查函数是否存在，参数类型是否匹配
void SemanticAnalyzer::visit(CallExpr& node) {
    // 检查函数是否已定义
    SymbolEntry* funcEntry = symbolTable.lookup(node.getCallee());
    if (!funcEntry) {
        std::cerr << "Error type 3 at line " << node.getLine() << " : call to undefined function '" << node.getCallee() << "'" << std::endl;
        node.setType(Type::INT); // 默认类型
        return;
    }
    
    if (funcEntry->kind != SymbolEntry::Kind::FUNCTION) {
        std::cerr << "Error type 5 at line " << node.getLine() << " : '" << node.getCallee() << "' is not a function" << std::endl;
        node.setType(Type::INT); // 默认类型
        return;
    }
    
    // 设置函数调用表达式的类型为函数返回类型
    node.setType(funcEntry->type);
    
    // 处理所有参数表达式
    for (auto& arg : node.getArgs()) {
        if (arg) {
            arg->accept(*this);
        }
    }
    
    // 检查参数数量是否匹配
    int actualArgCount = node.getArgs().size();
    if (actualArgCount != funcEntry->paramCount) {
        std::cerr << "Error type 9 at line " << node.getLine() << " : function '" << node.getCallee() << "' expects " 
                  << funcEntry->paramCount << " arguments, but " 
                  << actualArgCount << " were provided" << std::endl;
    }
    
    // 检查参数类型是否匹配
    for (int i = 0; i < std::min(actualArgCount, funcEntry->paramCount); i++) {
        if (node.getArgs()[i]) {
            Type argType = node.getArgs()[i]->getType();
            if (argType != funcEntry->paramTypes[i]) {
            std::cerr << "Error type 9 at line " << node.getArgs()[i]->getLine() << " : argument " << i + 1 << " of function '" 
                      << node.getCallee() << "' has type '" 
                      << typeToString(argType) << "', but expected '" 
                      << typeToString(funcEntry->paramTypes[i]) << "'" << std::endl;
        }
        }
    }
}

// 访问数组索引表达式节点
// 处理数组基址和索引表达式，检查索引类型
void SemanticAnalyzer::visit(IndexExpr& node) {
    // 处理数组基址
    if (node.getBase()) {
        node.getBase()->accept(*this);
    }
    
    // 处理索引表达式
    if (node.getIndex()) {
        node.getIndex()->accept(*this);
        
        // 检查索引是否为整数类型
        if (node.getIndex()->getType() != Type::INT) {
        std::cerr << "Error type 7 at line " << node.getLine() << " : array index must be an integer" << std::endl;
    }
    }
    
    // 设置数组元素的类型
    if (node.getBase()) {
        node.setType(node.getBase()->getType());
    }
}

// 访问数字表达式节点
// 数字表达式无需特殊处理
void SemanticAnalyzer::visit(NumberExpr& node) {
    // 数字表达式无需特殊处理
}

// 访问变量表达式节点
// 检查变量是否已声明，并设置变量类型
void SemanticAnalyzer::visit(VariableExpr& node) {
    // 检查变量是否已在符号表中声明
    SymbolEntry* entry = symbolTable.lookup(node.getName());
    if (!entry) {
        std::cerr << "Error type 1 at line " << node.getLine() << " : use of undeclared variable '" << node.getName() << "'" << std::endl;
        node.setType(Type::INT); // 默认类型，避免后续错误
    } else {
        // 设置变量表达式的类型
        node.setType(entry->type);
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
    // 此方法不应直接调用，变量定义应在VarDecl中处理
    // 这里仅作占位符
}


// 访问函数参数节点
// 处理参数的符号表条目
void SemanticAnalyzer::visit(FuncFParam& node) {
    // 创建参数的符号表项
    SymbolEntry paramEntry(SymbolEntry::Kind::PARAMETER, node.getType());
    paramEntry.isArray = node.getIsArray(); // 设置是否为数组参数
    
    // 添加参数到当前作用域的符号表
    if (!symbolTable.insert(node.getName(), paramEntry)) {
        std::cerr << "Error type 2 at line " << node.getLine() << " : redefinition of parameter '" << node.getName() << "'" << std::endl;
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

// 访问声明语句节点
// 处理声明语句
void SemanticAnalyzer::visit(DeclStmt& node) {
    // 处理包装的声明
    if (node.getDecl()) {
        node.getDecl()->accept(*this);
    }
}

// 检查类型兼容性
// 确保两个类型匹配，如果不匹配则输出错误消息
void SemanticAnalyzer::checkTypeCompatibility(Type t1, Type t2, const std::string& context) {
    // 这个方法不再直接输出错误，错误输出在调用处处理
    // 保持这个方法用于类型检查但不输出错误
}

// 检查数组维度
// 确保数组的索引数量与数组的维度数量匹配
void SemanticAnalyzer::checkArrayDimensions(const std::vector<std::unique_ptr<Expr>>& indices,
                                             const std::vector<int>& dims) {
    // 这个方法不再直接输出错误，错误输出在调用处处理
    // 保持这个方法用于维度检查但不输出错误
}
