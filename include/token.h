#pragma once

#include <string>
#include <variant>

// Token类型枚举 - 定义SysY语言中所有可能的词法单元类型
enum class TokenType {
    // 关键字：SysY语言的保留字
    INT, FLOAT, CONST, VOID, IF, ELSE, WHILE, BREAK, CONTINUE, RETURN,
    
    // 类型：用于声明和定义的类型说明符
    INT_TYPE, FLOAT_TYPE, VOID_TYPE,
    
    // 标识符和常量：程序中的变量名、函数名和字面量
    IDENT, INT_CONST, FLOAT_CONST,
    
    // 运算符：算术、比较、逻辑和赋值运算符
    PLUS, MINUS, MUL, DIV, MOD,
    ASSIGN, EQ, NE, LT, GT, LE, GE,
    AND, OR, NOT,
    INCREMENT, DECREMENT,
    
    // 分隔符：用于分隔代码结构的符号
    LPAREN, RPAREN, LBRACKET, RBRACKET,
    LBRACE, RBRACE, SEMICOLON, COMMA,
    
    // 特殊：文件结束标记和未知标记
    END_OF_FILE, UNKNOWN
};

// Token结构体 - 表示从源代码中解析出的单个词法单元
struct Token {
public:  // 公共访问修饰符
    TokenType type;          // Token的类型
    enum class ValueType { INT_VAL, FLOAT_VAL, STRING_VAL } valueType; // 值的类型
    union {
        int intValue;        // 整数值（用于整数常量）
        float floatValue;    // 浮点数值（用于浮点常量）
    };
    std::string stringValue; // 字符串值（用于标识符、关键字等）
    std::string errorMessage; // 错误信息（当Token类型为UNKNOWN时使用）
    int line;               // Token所在的行号（用于错误报告）
    int column;             // Token所在的列号（用于错误报告）
    
    // 将Token转换为字符串表示
    // 用于调试和输出词法分析结果
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