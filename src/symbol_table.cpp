#include "../include/symbol_table.h"
#include <stdexcept>

// 进入新的作用域
// 向作用域栈中添加一个新的空作用域
void SymbolTable::enterScope() {
    scopes.push_back({});
}

// 退出当前作用域
// 从作用域栈中移除最顶层的作用域
// 如果栈为空，则抛出运行时错误
void SymbolTable::exitScope() {
    if (scopes.empty()) {
        throw std::runtime_error("Cannot exit scope: no active scope");
    }
    scopes.pop_back();
}

// 向当前作用域插入符号
// 尝试在当前作用域中插入指定名称和条目的符号
// 如果符号已存在或作用域栈为空，则返回失败
// 否则插入成功并返回true
bool SymbolTable::insert(const std::string& name, SymbolEntry entry) {
    if (scopes.empty()) {
        throw std::runtime_error("Cannot insert symbol: no active scope");
    }

    auto& currentScope = scopes.back();
    if (currentScope.find(name) != currentScope.end()) {
        return false; // 符号已存在
    }

    currentScope[name] = entry;
    return true;
}

// 查找符号
// 从最内层作用域开始向外查找指定名称的符号
// 如果找到，则返回符号条目指针
// 否则返回nullptr
SymbolEntry* SymbolTable::lookup(const std::string& name) {
    // 从最内层作用域开始查找
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto entryIt = it->find(name);
        if (entryIt != it->end()) {
            return &entryIt->second;
        }
    }

    return nullptr; // 符号未找到
}

// 在当前作用域中查找符号
// 仅在当前最内层作用域中查找指定名称的符号
// 如果找到，则返回符号条目指针
// 否则返回nullptr
// 如果作用域栈为空，则抛出运行时错误
SymbolEntry* SymbolTable::lookupCurrentScope(const std::string& name) {
    if (scopes.empty()) {
        throw std::runtime_error("Cannot lookup symbol: no active scope");
    }

    auto& currentScope = scopes.back();
    auto entryIt = currentScope.find(name);
    if (entryIt != currentScope.end()) {
        return &entryIt->second;
    }

    return nullptr; // 符号未找到
}
