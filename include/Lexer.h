#pragma once
#include "token.h"
#include <string>

// Lexer类 - 词法分析器，负责将源代码字符串转换为Token流
class Lexer {
private:
    std::string source;   // 源代码字符串
    size_t position;      // 当前字符位置
    size_t line;          // 当前行号
    size_t column;        // 当前列号
    char currentChar;     // 当前字符
    
    // 私有方法：字符处理函数
    void advance();       // 前进到下一个字符
    char peek();          // 预览下一个字符
    void skipWhitespace();// 跳过空白字符（空格、制表符、换行符等）
    void skipComment();   // 跳过注释
    bool isDigit(char c); // 检查字符是否为数字
    bool isAlpha(char c); // 检查字符是否为字母
    bool isAlnum(char c); // 检查字符是否为字母或数字
    
    // 私有方法：Token解析函数
    Token parseNumber();  // 解析数字常量（整数或浮点数）
    Token parseIdentifier(); // 解析标识符或关键字
    Token parseString();  // 解析字符串常量
    
public:
    // 构造函数：初始化词法分析器
    // 参数：source - 要分析的源代码字符串（常量引用）
    Lexer(const std::string& source);
    
    // 获取下一个Token
    // 返回：解析出的下一个Token
    Token getNextToken();
    
    // 预览下一个Token（不消耗Token）
    // 返回：预览到的Token
    Token peekToken();
    
    // 预览第n个Token（不消耗Token）
    // 参数：n - 要预览的Token索引（从0开始）
    // 返回：预览到的Token
    Token peekToken(int n);
    
    // 获取当前行号
    // 返回：当前处理的行号
    size_t getLine() const { return line; }
};