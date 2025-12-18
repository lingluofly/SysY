#pragma once

#include <string>
#include <variant>

enum class TokenType {
    // 关键字
    INT, FLOAT, CONST, VOID, IF, ELSE, WHILE, BREAK, CONTINUE, RETURN,
    
    // 类型
    INT_TYPE, FLOAT_TYPE, VOID_TYPE,
    
    // 标识符和常量
    IDENT, INT_CONST, FLOAT_CONST,
    
    // 运算符
    PLUS, MINUS, MUL, DIV, MOD,
    ASSIGN, EQ, NE, LT, GT, LE, GE,
    AND, OR, NOT,
    INCREMENT, DECREMENT,
    
    // 分隔符
    LPAREN, RPAREN, LBRACKET, RBRACKET,
    LBRACE, RBRACE, SEMICOLON, COMMA,
    
    // 特殊
    END_OF_FILE, UNKNOWN
};

struct Token {
public:  // 添加公共访问修饰符
    TokenType type;
    enum class ValueType { INT_VAL, FLOAT_VAL, STRING_VAL } valueType;
    union {
        int intValue;
        float floatValue;
    };
    std::string stringValue;
    std::string errorMessage; // 错误信息
    int line;
    int column;
    
    std::string toString() const {
        switch (type) {
            // 关键字
            case TokenType::INT: return "INTTK int";
            case TokenType::FLOAT: return "FLOAT float";
            case TokenType::CONST: return "CONST const";
            case TokenType::VOID: return "VOID void";
            case TokenType::IF: return "IF if";
            case TokenType::ELSE: return "ELSE else";
            case TokenType::WHILE: return "WHILE while";
            case TokenType::BREAK: return "BREAK break";
            case TokenType::CONTINUE: return "CONTINUE continue";
            case TokenType::RETURN: return "RETURN return";
            
            // 类型
            case TokenType::INT_TYPE: return "INT_TYPE int";
            case TokenType::FLOAT_TYPE: return "FLOAT_TYPE float";
            case TokenType::VOID_TYPE: return "VOID_TYPE void";
            
            // 标识符和常量
            case TokenType::IDENT: return "ID " + stringValue;
            case TokenType::INT_CONST: return "INTCON " + std::to_string(intValue);
            case TokenType::FLOAT_CONST: return "FLOATCON " + std::to_string(floatValue);
            
            // 运算符
            case TokenType::PLUS: return "PLUS +";
            case TokenType::MINUS: return "MINUS -";
            case TokenType::MUL: return "MUL *";
            case TokenType::DIV: return "DIV /";
            case TokenType::MOD: return "MOD %";
            case TokenType::ASSIGN: return "ASSIGN =";
            case TokenType::EQ: return "EQ ==";
            case TokenType::NE: return "NE !=";
            case TokenType::LT: return "LT <";
            case TokenType::GT: return "GT >";
            case TokenType::LE: return "LE <=";
            case TokenType::GE: return "GE >=";
            case TokenType::AND: return "AND &&";
            case TokenType::OR: return "OR ||";
            case TokenType::NOT: return "NOT !";
            case TokenType::INCREMENT: return "INCREMENT ++";
            case TokenType::DECREMENT: return "DECREMENT --";
            
            // 分隔符
            case TokenType::LPAREN: return "LPARENT (";
            case TokenType::RPAREN: return "RPARENT )";
            case TokenType::LBRACKET: return "LBRACKET [";
            case TokenType::RBRACKET: return "RBRACKET ]";
            case TokenType::LBRACE: return "LBRACE (";
            case TokenType::RBRACE: return "RBRACE )";
            case TokenType::SEMICOLON: return "SEMICN ;";
            case TokenType::COMMA: return "COMMA ,";
            
            // 特殊
            case TokenType::END_OF_FILE: return "END_OF_FILE";
            case TokenType::UNKNOWN: return "UNKNOWN";
            
            default: return "UNKNOWN";
        }
    }
};