#include <iostream>
#include <fstream>
#include <string>
#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/semantic_analyzer.h"
#include "../include/print_visitor.h"

// 编译器主函数
// 负责处理命令行参数、读取源代码文件、执行编译流程并输出结果
int main(int argc, char* argv[]) {
    // 检查命令行参数数量是否正确
    if (argc != 2) {
        std::cerr << "Usage: sysy_compiler <input_file>" << std::endl;
        return 1; // 错误码1表示参数错误
    }

    std::string filename = argv[1];
    std::ifstream file(filename);

    // 检查文件是否成功打开
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file \"" << filename << "\"" << std::endl;
        return 1; // 错误码1表示文件打开失败
    }

    // 读取文件内容到字符串
    std::string source((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    file.close(); // 关闭文件

    // 创建词法分析器实例
    Lexer lexer(source);
    
    // 遍历所有Token，检查是否有词法错误
    bool hasError = false;
    std::vector<Token> tokens;
    Token token;
    
    while (true) {
        token = lexer.getNextToken();
        
        if (token.type == TokenType::END_OF_FILE) {
            break;
        }
        
        if (token.type == TokenType::UNKNOWN) {
            // 词法错误，按照实验要求输出错误信息
            if (token.errorMessage.empty()) {
                std::cout << "Error type A at line " << token.line << " : Invalid token" << std::endl;
            } else {
                std::cout << "Error type A at line " << token.line << " : " << token.errorMessage << std::endl;
            }
            hasError = true;
        } else {
            // 没有词法错误，保存Token信息
            tokens.push_back(token);
        }
    }
    
    // 如果有词法错误，返回错误码1
    if (hasError) {
        return 1;
    }
    
    // 如果没有词法错误，继续执行语法和语义分析
    // 输出词法单元列表
    for (const auto& t : tokens) {
        std::cout << t.toString() << std::endl;
    }
    try {
        // 重置词法分析器
        Lexer lexer2(source);
        
        // 创建语法分析器实例
        Parser parser(lexer2);
        
        // 执行语法分析，生成编译单元
        auto compUnit = parser.parse();

        // 创建语义分析器实例
        SemanticAnalyzer analyzer;
        
        // 执行语义分析
        compUnit->accept(analyzer);
        
        // 不打印语法树，只保留错误输出

        // 编译成功，不输出额外提示，只输出词法单元列表
    } catch (const std::exception& e) {
        // 捕获并处理编译过程中的异常
        std::string errorMsg = e.what();
        size_t firstColon = errorMsg.find(':');
        if (firstColon != std::string::npos) {
            // 提取错误类型
            std::string errorType = errorMsg.substr(0, firstColon);
            size_t secondColon = errorMsg.find(':', firstColon + 1);
            if (secondColon != std::string::npos) {
                // 提取行号
                std::string lineStr = errorMsg.substr(firstColon + 1, secondColon - firstColon - 1);
                // 提取错误说明
                std::string description = errorMsg.substr(secondColon + 1);
                // 按照实验要求的格式输出错误信息
                std::cout << "Error type " << errorType << " at line " << lineStr << " : " << description << std::endl;
            } else {
                // 无法解析的异常格式
                std::cout << "Error: " << errorMsg << std::endl;
            }
        } else {
            // 无法解析的异常格式
            std::cout << "Error: " << errorMsg << std::endl;
        }
        return 1; // 错误码1表示编译失败
    }

    return 0; // 返回0表示编译成功
}