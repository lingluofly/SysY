#include "../include/Lexer.h"
#include "../include/token.h"
#include "../include/ast.h"
#include "../include/parser.h"
#include <stdexcept>
#include "Parser.h"
#include "ast.h"
#include <memory>

// 语法分析器构造函数
// 初始化语法分析器，关联词法分析器并获取第一个Token
Parser::Parser(Lexer& lexer)
    : lexer(lexer) {
    currentToken = lexer.getNextToken();
}

// 解析整个编译单元
// 处理所有的变量声明和函数定义，并生成编译单元节点
std::unique_ptr<CompUnit> Parser::parse() {
    auto compUnit = std::make_unique<CompUnit>();
    
    // 循环解析所有Token，直到文件结束
    while (currentToken.type != TokenType::END_OF_FILE) {
        // 解析声明或函数定义
        if (currentToken.type == TokenType::INT_TYPE || 
            currentToken.type == TokenType::FLOAT_TYPE || 
            currentToken.type == TokenType::VOID_TYPE) {
            
            // 检查是否是函数定义
            bool isFunction = false;
            Token lookaheadToken = lexer.peekToken();
            
            if (lookaheadToken.type == TokenType::LPAREN) {
                // 是函数定义，解析并添加到编译单元
                compUnit->addFuncDef(parseFuncDef());
            } else {
                // 是变量声明，解析变量定义
                parseVarDef();
            }
        } else {
            // 跳过未知Token
            currentToken = lexer.getNextToken();
        }
    }
    
    return compUnit;
}

// 消费指定类型的Token
// 如果当前Token类型与预期类型匹配，则消费并获取下一个Token；否则抛出异常
void Parser::consumeToken(TokenType expectedType) {
    if (currentToken.type == expectedType) {
        currentToken = lexer.getNextToken();
    } else {
        // 错误处理：Token类型不匹配
        throw std::runtime_error("Unexpected token type");
    }
}

// 解析函数定义
// 处理函数的返回类型、函数名、参数列表和函数体，并生成函数定义节点
std::unique_ptr<FuncDef> Parser::parseFuncDef() {
    auto funcDef = std::make_unique<FuncDef>();
    
    // 解析函数返回类型
    if (currentToken.type == TokenType::INT_TYPE) {
        funcDef->setReturnType(Type::INT);
    } else if (currentToken.type == TokenType::FLOAT_TYPE) {
        funcDef->setReturnType(Type::FLOAT);
    } else if (currentToken.type == TokenType::VOID_TYPE) {
        funcDef->setReturnType(Type::VOID);
    }
    consumeToken(currentToken.type);
    
    // 解析函数名
    if (currentToken.type == TokenType::IDENT) {
        funcDef->setName(currentToken.stringValue);
        consumeToken(TokenType::IDENT);
    }
    
    // 解析参数列表
    consumeToken(TokenType::LPAREN); // 消费左括号
    if (currentToken.type != TokenType::RPAREN) {
        parseFuncParams(funcDef->getParamsRef()); // 解析参数
    }
    consumeToken(TokenType::RPAREN); // 消费右括号

    // 解析函数体
    consumeToken(TokenType::LBRACE); // 消费左花括号
    // 这里应该解析函数体的语句块（目前未实现）
    consumeToken(TokenType::RBRACE); // 消费右花括号
    
    return funcDef;
}

// 解析函数参数列表
// 处理多个函数参数，包括参数类型、参数名和数组参数，并添加到参数列表中
void Parser::parseFuncParams(std::vector<std::unique_ptr<FuncFParam>>& params) {
    while (true) {
        auto param = std::make_unique<FuncFParam>();
        
        // 解析参数类型
        if (currentToken.type == TokenType::INT_TYPE) {
            param->setType(Type::INT);
        } else if (currentToken.type == TokenType::FLOAT_TYPE) {
            param->setType(Type::FLOAT);
        }
        consumeToken(currentToken.type);
        
        // 解析参数名
        if (currentToken.type == TokenType::IDENT) {
            param->setName(currentToken.stringValue);
            consumeToken(TokenType::IDENT);
        }
        
        // 检查是否是数组参数
        if (currentToken.type == TokenType::LBRACKET) {
            param->setIsArray(true);
            consumeToken(TokenType::LBRACKET); // 消费左方括号
            
            // 解析数组大小
            if (currentToken.type == TokenType::INT_CONST) {
                param->setArraySize(currentToken.intValue);
                consumeToken(TokenType::INT_CONST);
            }
            
            consumeToken(TokenType::RBRACKET); // 消费右方括号
        }
        
        params.push_back(std::move(param)); // 添加参数到列表
        
        // 检查是否还有下一个参数
        if (currentToken.type != TokenType::COMMA) {
            break; // 没有逗号，参数列表结束
        }
        consumeToken(TokenType::COMMA); // 消费逗号，准备解析下一个参数
    }
}

// 解析变量定义
// 处理变量声明，包括变量类型、变量名、数组变量和初始化值，并添加到符号表
void Parser::parseVarDef() {
    // 解析变量类型
    Type varType;
    if (currentToken.type == TokenType::INT_TYPE) {
        varType = Type::INT;
    } else if (currentToken.type == TokenType::FLOAT_TYPE) {
        varType = Type::FLOAT;
    }
    consumeToken(currentToken.type);
    
    // 解析变量列表
    while (true) {
        // 解析变量名
        std::string varName;
        if (currentToken.type == TokenType::IDENT) {
            varName = currentToken.stringValue;
            consumeToken(TokenType::IDENT);
        }
        
        // 检查是否是数组变量
        bool isArray = false;
        int arraySize = 0;
        if (currentToken.type == TokenType::LBRACKET) {
            isArray = true;
            consumeToken(TokenType::LBRACKET); // 消费左方括号
            
            // 解析数组大小
            if (currentToken.type == TokenType::INT_CONST) {
                arraySize = currentToken.intValue;
                consumeToken(TokenType::INT_CONST);
            }
            
            consumeToken(TokenType::RBRACKET); // 消费右方括号
        }
        
        // 检查是否有初始化值
        if (currentToken.type == TokenType::ASSIGN) {
            consumeToken(TokenType::ASSIGN);
            // 这里应该解析初始化表达式（目前未实现）
        }
        
        // 添加到符号表（目前注释掉了）
        // symbolTable.insert(varName, SymbolEntry{Type::VARIABLE, varType, isArray});
        
        // 检查是否还有下一个变量
        if (currentToken.type != TokenType::COMMA) {
            break; // 没有逗号，变量列表结束
        }
        consumeToken(TokenType::COMMA); // 消费逗号，准备解析下一个变量
    }
    
    consumeToken(TokenType::SEMICOLON); // 消费分号，变量定义结束
}
