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
        if (currentToken.type == TokenType::INT || 
            currentToken.type == TokenType::FLOAT || 
            currentToken.type == TokenType::VOID) {
            
            // 保存当前类型Token
            TokenType typeTokenType = currentToken.type;
            
            // 预取下一个Token
            Token nextToken = lexer.peekToken();
            
            // 判断下一个Token是否是IDENT
            bool isIdent = (nextToken.type == TokenType::IDENT);
            
            // 如果是IDENT，进一步判断是否是函数定义
            bool isFuncDef = false;
            if (isIdent) {
                // 预取下下一个Token
                Token nextNextToken = lexer.peekToken(2);
                isFuncDef = (nextNextToken.type == TokenType::LPAREN);
            }
            
            if (isFuncDef) {
                // 是函数定义
                // 消费类型Token
                consumeToken(typeTokenType);
                
                // 消费标识符Token（函数名）
                std::string name = currentToken.stringValue;
                consumeToken(TokenType::IDENT);
                
                // 解析函数返回类型
                Type returnType;
                if (typeTokenType == TokenType::INT) {
                    returnType = Type::INT;
                } else if (typeTokenType == TokenType::FLOAT) {
                    returnType = Type::FLOAT;
                } else { // VOID
                    returnType = Type::VOID;
                }
                
                // 创建函数定义节点
                auto funcDef = std::make_unique<FuncDef>();
                funcDef->setReturnType(returnType);
                funcDef->setName(name);
                
                // 解析函数参数
                consumeToken(TokenType::LPAREN);
                if (currentToken.type != TokenType::RPAREN) {
                    parseFuncParams(funcDef->getParamsRef());
                }
                consumeToken(TokenType::RPAREN);
                
                // 解析函数体
                consumeToken(TokenType::LBRACE);
                auto body = std::make_unique<Block>();
                parseStatementList(*body);
                consumeToken(TokenType::RBRACE);
                funcDef->setBody(std::move(body));
                
                // 添加到编译单元
                compUnit->addFuncDef(std::move(funcDef));
            } else {
                // 不是函数定义，是变量声明
                if (auto varDecl = parseVarDef()) {
                    compUnit->addDecl(std::move(varDecl));
                }
            }
        } else if (currentToken.type == TokenType::IDENT) {
            // 可能是赋值语句
            try {
                // 尝试解析表达式语句
                auto expr = parseExpression();
                consumeToken(TokenType::SEMICOLON);
            } catch (const std::exception& e) {
                // 如果解析失败，跳过这个Token
                currentToken = lexer.getNextToken();
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
        std::string errorMsg = "B:" + std::to_string(currentToken.line) + ":Unexpected token type, expected type: " + std::to_string(static_cast<int>(expectedType)) + ", got: " + std::to_string(static_cast<int>(currentToken.type));
        throw std::runtime_error(errorMsg);
    }
}

// 解析函数定义
// 处理函数的返回类型、函数名、参数列表和函数体，并生成函数定义节点
std::unique_ptr<FuncDef> Parser::parseFuncDef() {
    auto funcDef = std::make_unique<FuncDef>(Type::INT, "", nullptr, lexer.getLine());
    
    // 解析函数返回类型
    if (currentToken.type == TokenType::INT) {
        funcDef->setReturnType(Type::INT);
    } else if (currentToken.type == TokenType::FLOAT) {
        funcDef->setReturnType(Type::FLOAT);
    } else if (currentToken.type == TokenType::VOID) {
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
    // 创建函数体的语句块，传递当前行号
    auto body = std::make_unique<Block>(lexer.getLine());
    parseStatementList(*body);
    funcDef->setBody(std::move(body));
    consumeToken(TokenType::RBRACE); // 消费右花括号
    
    return funcDef;
}

// 解析函数参数列表
// 处理多个函数参数，包括参数类型、参数名和数组参数，并添加到参数列表中
void Parser::parseFuncParams(std::vector<std::unique_ptr<FuncFParam>>& params) {
    while (true) {
        // 创建函数参数节点，传递当前行号
        auto param = std::make_unique<FuncFParam>(Type::INT, "", false, lexer.getLine());
        
        // 解析参数类型
        if (currentToken.type == TokenType::INT) {
            param->setType(Type::INT);
        } else if (currentToken.type == TokenType::FLOAT) {
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
// 处理变量声明，包括变量类型、变量名、数组变量和初始化值，并生成变量声明节点
std::unique_ptr<VarDecl> Parser::parseVarDef() {
    // 解析变量类型
    Type varType;
    if (currentToken.type == TokenType::INT) {
        varType = Type::INT;
    } else if (currentToken.type == TokenType::FLOAT) {
        varType = Type::FLOAT;
    } else {
        // 只有INT和FLOAT类型是允许的变量类型
        std::string errorMsg = "B:" + std::to_string(currentToken.line) + ":Invalid variable type, expected int or float"; 
        throw std::runtime_error(errorMsg);
    }
    consumeToken(currentToken.type);
    
    // 创建变量声明节点，传递当前行号
    auto varDecl = std::make_unique<VarDecl>(varType, false, lexer.getLine());
    
    // 解析变量列表
    bool hasVariable = false;
    while (true) {
        // 解析变量名
        std::string varName;
        if (currentToken.type == TokenType::IDENT) {
            varName = currentToken.stringValue;
            consumeToken(TokenType::IDENT);
            hasVariable = true;
        } else {
            // 无效的变量名
            if (hasVariable) {
                // 如果已经解析了变量，检查分号
                break;
            } else {
                // 如果没有解析到任何变量，抛出异常
                std::string errorMsg = "B:" + std::to_string(currentToken.line) + ":Invalid variable declaration"; 
                throw std::runtime_error(errorMsg);
            }
        }
        
        // 检查是否是数组变量
        bool isArray = false;
        int arraySize = 0;
        // 支持多维数组
        while (currentToken.type == TokenType::LBRACKET) {
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
        std::unique_ptr<Expr> initExpr = nullptr;
        if (currentToken.type == TokenType::ASSIGN) {
            consumeToken(TokenType::ASSIGN);
            initExpr = parseExpression();
        }
        
        // 创建变量定义节点，传递当前行号
        auto varDef = std::make_unique<VarDef>(varName, std::move(initExpr), isArray, lexer.getLine());
        
        // 添加变量定义到变量声明
        varDecl->addVarDef(std::move(varDef));
        
        // 检查是否还有下一个变量
        if (currentToken.type != TokenType::COMMA) {
            break; // 没有逗号，变量列表结束
        }
        consumeToken(TokenType::COMMA); // 消费逗号，准备解析下一个变量
    }
    
    // 消费分号，变量定义结束
    // 变量声明通常以分号结束，但也可能遇到其他语法结构
    if (currentToken.type == TokenType::SEMICOLON) {
        consumeToken(TokenType::SEMICOLON);
    } else if (currentToken.type == TokenType::RBRACE || currentToken.type == TokenType::RPAREN || 
               currentToken.type == TokenType::COMMA) {
        // 如果遇到语法结构结束符，不消费并留给调用者处理
        return varDecl;
    } else if (currentToken.type != TokenType::END_OF_FILE) {
        // 如果不是分号且不是文件结束，抛出异常
        std::string errorMsg = "B:" + std::to_string(currentToken.line) + ":Missing semicolon at end of variable declaration"; 
        throw std::runtime_error(errorMsg);
    }
    
    return varDecl;
}

// 解析表达式
// 处理各种类型的表达式，如常量、变量、二元表达式等
std::unique_ptr<Expr> Parser::parseExpression() {
    return parseBinaryExpression();
}

// 解析二元表达式
// 处理二元运算符的表达式，如a + b, x * y等
std::unique_ptr<Expr> Parser::parseBinaryExpression() {
    // 解析左操作数
    auto left = parseUnaryExpression();
    
    // 处理二元运算符
    while (true) {
        TokenType opType = currentToken.type;
        if (opType == TokenType::PLUS || opType == TokenType::MINUS || 
            opType == TokenType::MUL || opType == TokenType::DIV || 
            opType == TokenType::LT || opType == TokenType::GT || 
            opType == TokenType::LE || opType == TokenType::GE || 
            opType == TokenType::EQ || opType == TokenType::NE) {
            
            consumeToken(opType); // 消费运算符
            
            // 解析右操作数
            auto right = parseUnaryExpression();
            
            // 创建二元表达式节点
            left = std::make_unique<BinaryExpr>(std::move(left), opType, std::move(right));
        } else if (opType == TokenType::ASSIGN) {
            consumeToken(opType); // 消费赋值运算符
            
            // 解析右操作数（赋值运算符是右结合的）
            auto right = parseBinaryExpression();
            
            // 创建赋值表达式节点
            left = std::make_unique<BinaryExpr>(std::move(left), opType, std::move(right));
        } else {
            break; // 不是二元运算符，结束解析
        }
    }
    
    return left;
}

// 解析一元表达式
// 处理一元运算符的表达式，如-x, !b等
std::unique_ptr<Expr> Parser::parseUnaryExpression() {
    // 检查是否是一元运算符
    if (currentToken.type == TokenType::MINUS || currentToken.type == TokenType::NOT) {
        TokenType opType = currentToken.type;
        consumeToken(opType); // 消费运算符
        
        // 解析操作数
        auto operand = parseUnaryExpression();
        
        // 创建一元表达式节点
        return std::make_unique<UnaryExpr>(opType, std::move(operand));
    }
    
    // 不是一元表达式，解析基本表达式
    return parsePrimaryExpression();
}

// 解析基本表达式
// 处理常量、变量、函数调用等基本表达式
std::unique_ptr<Expr> Parser::parsePrimaryExpression() {
    std::unique_ptr<Expr> expr;
    
    // 根据当前Token类型解析不同的表达式
    switch (currentToken.type) {
        case TokenType::INT_CONST: {
            // 整数常量表达式，传递当前行号
            expr = std::make_unique<NumberExpr>(currentToken.intValue, lexer.getLine());
            consumeToken(TokenType::INT_CONST);
            break;
        }
        case TokenType::FLOAT_CONST: {
            // 浮点数常量表达式，传递当前行号
            expr = std::make_unique<NumberExpr>(currentToken.floatValue, lexer.getLine());
            consumeToken(TokenType::FLOAT_CONST);
            break;
        }
        case TokenType::IDENT: {
            // 标识符表达式，可能是变量、函数调用或数组访问
            std::string identName = currentToken.stringValue;
            consumeToken(TokenType::IDENT);
            
            // 首先创建变量表达式作为基础，传递当前行号
            expr = std::make_unique<VariableExpr>(identName, lexer.getLine());
            
            // 检查是否是数组访问（可能有多个维度）
            while (currentToken.type == TokenType::LBRACKET) {
                consumeToken(TokenType::LBRACKET);
                
                // 解析数组索引表达式
                auto indexExpr = parseExpression();
                
                consumeToken(TokenType::RBRACKET);
                
                // 创建数组访问表达式节点，将当前表达式作为左操作数
                expr = std::make_unique<IndexExpr>(std::move(expr), std::move(indexExpr));
            }
            
            // 检查是否是函数调用（在数组访问之后检查，因为可能有func()[index]这样的表达式）
            if (currentToken.type == TokenType::LPAREN) {
                consumeToken(TokenType::LPAREN);
                
                // 解析函数参数列表
                std::vector<std::unique_ptr<Expr>> args;
                if (currentToken.type != TokenType::RPAREN) {
                    // 解析第一个参数
                    args.push_back(parseExpression());
                    
                    // 解析后续参数
                    while (currentToken.type == TokenType::COMMA) {
                        consumeToken(TokenType::COMMA);
                        args.push_back(parseExpression());
                    }
                }
                
                consumeToken(TokenType::RPAREN);
                
                // 创建函数调用表达式节点，传递当前行号
                expr = std::make_unique<CallExpr>(identName, std::move(args), lexer.getLine());
            }
            break;
        }
        case TokenType::LPAREN: {
            // 括号表达式
            consumeToken(TokenType::LPAREN);
            expr = parseExpression();
            consumeToken(TokenType::RPAREN);
            break;
        }
        default:
            // 未知的表达式类型，抛出异常
            std::string errorMsg = "B:" + std::to_string(currentToken.line) + ":Unexpected token in expression parsing"; 
            throw std::runtime_error(errorMsg);
    }
    
    return expr;
}

// 解析语句列表
// 处理一系列语句，如变量声明、赋值语句、控制流语句等
void Parser::parseStatementList(Block& block) {
    while (currentToken.type != TokenType::RBRACE && currentToken.type != TokenType::END_OF_FILE) {
        // 解析单个语句
        auto stmt = parseStatement();
        if (stmt) {
            block.addStatement(std::move(stmt));
        }
    }
}

// 解析单个语句
// 处理变量声明、赋值语句、控制流语句等
std::unique_ptr<Stmt> Parser::parseStatement() {
    switch (currentToken.type) {
        case TokenType::INT: 
        case TokenType::FLOAT: {
            // 变量声明语句 - 直接调用parseVarDef，它会处理变量声明并返回
            auto varDecl = parseVarDef();
            // 创建一个声明语句，将VarDecl包装起来添加到Block，传递当前行号
            return std::make_unique<DeclStmt>(std::move(varDecl), lexer.getLine());
        }
        case TokenType::RETURN: {
            // return语句
            consumeToken(TokenType::RETURN);
            
            std::unique_ptr<Expr> expr = nullptr;
            if (currentToken.type != TokenType::SEMICOLON) {
                expr = parseExpression();
            }
            
            consumeToken(TokenType::SEMICOLON);
            
            return std::make_unique<ReturnStmt>(std::move(expr), lexer.getLine());
        }
        case TokenType::IF: {
            // if语句
            consumeToken(TokenType::IF);
            
            // 解析条件表达式
            consumeToken(TokenType::LPAREN);
            auto condition = parseExpression();
            consumeToken(TokenType::RPAREN);
            
            // 解析then语句块
            // 解析then语句块
            std::unique_ptr<Stmt> thenStmt;
            if (currentToken.type == TokenType::LBRACE) {
                // 语句块
                consumeToken(TokenType::LBRACE);
                thenStmt = std::make_unique<Block>(lexer.getLine());
                parseStatementList(*static_cast<Block*>(thenStmt.get()));
                consumeToken(TokenType::RBRACE);
            } else {
                // 单个语句
                thenStmt = parseStatement();
            }
            
            // 解析可选的else语句块
            std::unique_ptr<Stmt> elseStmt = nullptr;
            if (currentToken.type == TokenType::ELSE) {
                consumeToken(TokenType::ELSE);
                if (currentToken.type == TokenType::LBRACE) {
                    // 语句块
                    consumeToken(TokenType::LBRACE);
                    elseStmt = std::make_unique<Block>(lexer.getLine());
                    parseStatementList(*static_cast<Block*>(elseStmt.get()));
                    consumeToken(TokenType::RBRACE);
                } else {
                    // 单个语句
                    elseStmt = parseStatement();
                }
            }
            
            return std::make_unique<IfStmt>(std::move(condition), std::move(thenStmt), std::move(elseStmt), lexer.getLine());
        }
        case TokenType::WHILE: {
            // while语句
            consumeToken(TokenType::WHILE);
            
            // 解析条件表达式
            consumeToken(TokenType::LPAREN);
            auto condition = parseExpression();
            consumeToken(TokenType::RPAREN);
            
            // 解析循环体
            auto body = parseStatement();
            
            return std::make_unique<WhileStmt>(std::move(condition), std::move(body), lexer.getLine());
        }
        case TokenType::LBRACE: {
            // 语句块
            consumeToken(TokenType::LBRACE);
            auto block = std::make_unique<Block>(lexer.getLine());
            parseStatementList(*block);
            consumeToken(TokenType::RBRACE);
            
            return block;
        }
        default: {
            // 表达式语句
            auto expr = parseExpression();
            consumeToken(TokenType::SEMICOLON);
            
            return std::make_unique<ExprStmt>(std::move(expr), lexer.getLine());
        }
    }
}
