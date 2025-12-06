#include "../include/token.h"
#include "../include/Lexer.h"
#include <cctype>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <string>

// 词法分析器构造函数
// 初始化词法分析器的状态，包括源代码、当前位置、行号和列号
Lexer::Lexer(const std::string& source) 
    : source(source), position(0), line(1), column(1) {
    if (!source.empty()) {
        currentChar = source[position];
    } else {
        currentChar = '\0';
    }
}

// 前进到下一个字符
// 更新当前位置、列号，并获取下一个字符
void Lexer::advance() {
    position++;
    column++;
    if (position < source.size()) {
        currentChar = source[position];
    } else {
        currentChar = '\0';
    }
}

// 跳过空白字符
// 处理空格、制表符、换行符等空白字符，并更新行号和列号
void Lexer::skipWhitespace() {
    while (currentChar != '\0' && isspace(currentChar)) {
        if (currentChar == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        advance();
    }
}

// 解析数字常量
// 处理整数和浮点数常量，并返回相应的Token对象
Token Lexer::parseNumber() {
    Token token;
    token.type = TokenType::INT_CONST;
    token.line = line;
    token.valueType = Token::ValueType::INT_VAL;
    
    std::string numStr;
    
    // 检查是否是八进制数（以0开头，后面跟0-7的数字）
    if (currentChar == '0') {
        numStr += currentChar;
        advance();
        
        // 检查八进制数是否包含非法字符（8-9）
        while (currentChar != '\0' && isdigit(currentChar)) {
            if (currentChar >= '8') {
                // 非法八进制数
                token.type = TokenType::UNKNOWN;
                // 保存完整的错误数值（包括所有已解析的部分和非法字符）
                numStr += currentChar;
                token.errorMessage = "illegal octal number '" + numStr + "'";
                advance();
                return token;
            }
            numStr += currentChar;
            advance();
        }
        
        // 检查是否是十六进制数（以0x或0X开头）
        if ((currentChar == 'x' || currentChar == 'X') && numStr == "0") {
            numStr += currentChar;
            advance();
            
            // 检查十六进制数是否包含非法字符（不是0-9、a-f、A-F）
            while (currentChar != '\0' && 
                   (isdigit(currentChar) || 
                    (currentChar >= 'a' && currentChar <= 'f') || 
                    (currentChar >= 'A' && currentChar <= 'F'))) {
                numStr += currentChar;
                advance();
            }
            
            // 如果十六进制数后面跟的不是有效的十六进制字符，说明是非法十六进制数
            if (currentChar != '\0' && 
                !isdigit(currentChar) && 
                !(currentChar >= 'a' && currentChar <= 'f') && 
                !(currentChar >= 'A' && currentChar <= 'F') && 
                !isspace(currentChar) && 
                !strchr("+-*/=<>!;(),[]{} ", currentChar)) {
                // 非法十六进制数
                token.type = TokenType::UNKNOWN;
                // 保存完整的错误数值（包括所有已解析的部分和非法字符）
                numStr += currentChar;
                token.errorMessage = "illegal hexadecimal number '" + numStr + "'";
                advance();
                return token;
            }
            
            token.intValue = std::stoi(numStr, nullptr, 16);
        } else {
            token.intValue = std::stoi(numStr, nullptr, 8);
        }
    } else {
        // 十进制数
        while (currentChar != '\0' && isdigit(currentChar)) {
            numStr += currentChar;
            advance();
        }
        
        // 检查是否是浮点数
        if (currentChar == '.') {
            token.type = TokenType::FLOAT_CONST;
            token.valueType = Token::ValueType::FLOAT_VAL;
            numStr += '.';
            advance();
            
            while (currentChar != '\0' && isdigit(currentChar)) {
                numStr += currentChar;
                advance();
            }
            
            token.floatValue = atof(numStr.c_str());
        } else {
            token.intValue = atoi(numStr.c_str());
        }
    }
    
    return token;
}

// 解析标识符或关键字
// 处理标识符（变量名、函数名等），并检查是否是关键字
Token Lexer::parseIdentifier() {
    Token token;
    token.type = TokenType::IDENT;
    token.line = line;
    token.valueType = Token::ValueType::STRING_VAL;
    
    std::string idStr;
    while (currentChar != '\0' && (isalnum(currentChar) || currentChar == '_')) {
        idStr += currentChar;
        advance();
    }
    
    token.stringValue = idStr;
    
    // 检查是否是关键字
    if (idStr == "int") {
        token.type = TokenType::INT;
    } else if (idStr == "float") {
        token.type = TokenType::FLOAT;
    } else if (idStr == "void") {
        token.type = TokenType::VOID;
    } else if (idStr == "if") {
        token.type = TokenType::IF;
    } else if (idStr == "else") {
        token.type = TokenType::ELSE;
    } else if (idStr == "while") {
        token.type = TokenType::WHILE;
    } else if (idStr == "return") {
        token.type = TokenType::RETURN;
    }
    
    return token;
}

// 获取下一个Token
// 跳过空白字符，然后根据当前字符类型调用相应的解析函数
Token Lexer::getNextToken() {
    skipWhitespace();
    
    if (currentChar == '\0') {
        Token token;
        token.type = TokenType::END_OF_FILE;
        token.line = line;
        return token;
    }
    
    if (isdigit(currentChar)) {
        return parseNumber();
    }
    
    if (isalpha(currentChar) || currentChar == '_') {
        return parseIdentifier();
    }
    
    // 处理运算符和分隔符
    Token token;
    token.line = line;
    
    switch (currentChar) {
        case '=':
            advance();
            if (currentChar == '=') {
                token.type = TokenType::EQ; // 等于运算符
                advance();
            } else {
                token.type = TokenType::ASSIGN; // 赋值运算符
            }
            break;
        case '+':
            token.type = TokenType::PLUS; // 加法运算符
            advance();
            break;
        case '-':
            token.type = TokenType::MINUS; // 减法运算符
            advance();
            break;
        case '*':
            token.type = TokenType::MUL; // 乘法运算符
            advance();
            break;
        case '/':
            advance();
            if (currentChar == '/') {
                // 处理单行注释
                while (currentChar != '\0' && currentChar != '\n') {
                    advance();
                }
                return getNextToken(); // 注释处理完后继续获取下一个Token
            } else if (currentChar == '*') {
                // 处理多行注释
                advance();
                while (currentChar != '\0') {
                    if (currentChar == '*' && source[position + 1] == '/') {
                        advance();
                        advance();
                        break; // 找到注释结束符
                    }
                    if (currentChar == '\n') {
                        line++;
                        column = 1;
                    }
                    advance();
                }
                return getNextToken(); // 注释处理完后继续获取下一个Token
            } else {
                token.type = TokenType::DIV; // 除法运算符
            }
            break;
        case '<':
            advance();
            if (currentChar == '=') {
                token.type = TokenType::LE; // 小于等于运算符
                advance();
            } else {
                token.type = TokenType::LT; // 小于运算符
            }
            break;
        case '>':
            advance();
            if (currentChar == '=') {
                token.type = TokenType::GE; // 大于等于运算符
                advance();
            } else {
                token.type = TokenType::GT; // 大于运算符
            }
            break;
        case '!':
            advance();
            if (currentChar == '=') {
                token.type = TokenType::NE; // 不等于运算符
                advance();
            } else {
                token.type = TokenType::UNKNOWN; // 无效的Token
                token.errorMessage = "Invalid character '!'";
                advance(); // 前进到下一个字符，避免无限循环
            }
            break;
        case ';':
            token.type = TokenType::SEMICOLON; // 分号分隔符
            advance();
            break;
        case ',':
            token.type = TokenType::COMMA; // 逗号分隔符
            advance();
            break;
        case '(':
            token.type = TokenType::LPAREN; // 左括号
            advance();
            break;
        case ')':
            token.type = TokenType::RPAREN; // 右括号
            advance();
            break;
        case '[':
            token.type = TokenType::LBRACKET; // 左方括号
            advance();
            break;
        case ']':
            token.type = TokenType::RBRACKET; // 右方括号
            advance();
            break;
        case '{':
            token.type = TokenType::LBRACE; // 左花括号
            advance();
            break;
        case '}':
            token.type = TokenType::RBRACE; // 右花括号
            advance();
            break;
        default:
            token.type = TokenType::UNKNOWN; // 无效的Token
            token.errorMessage = "Invalid character '" + std::string(1, currentChar) + "'";
            advance(); // 前进到下一个字符，避免无限循环
    }
    
    return token;
}

// 预取下一个Token（不改变词法分析器状态）
// 保存当前状态，获取下一个Token，然后恢复状态
Token Lexer::peekToken() {
    // 保存当前的lexer状态
    size_t savedPosition = position;
    size_t savedLine = line;
    size_t savedColumn = column;
    char savedCurrentChar = currentChar;
    
    // 获取下一个token
    Token token = getNextToken();
    
    // 恢复lexer状态
    position = savedPosition;
    line = savedLine;
    column = savedColumn;
    currentChar = savedCurrentChar;
    
    return token;
}