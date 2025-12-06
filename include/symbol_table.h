#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include "ast.h"

struct SymbolEntry {
public:
    enum class Kind { VARIABLE, CONSTANT, FUNCTION, PARAMETER };
    
    Kind kind;
    Type type;
    bool isArray;
    std::vector<int> dimensions; // 数组维度
    union {
        int intValue;
        float floatValue;
    } value;
    enum class ValueType { NONE, INT, FLOAT } valueType;
    
    // 默认构造函数
    SymbolEntry() : kind(Kind::VARIABLE), type(Type::INT), isArray(false), valueType(ValueType::NONE) {} 
    
    SymbolEntry(Kind kind, Type type, bool isArray = false)
        : kind(kind), type(type), isArray(isArray), valueType(ValueType::NONE) {}
};

class SymbolTable {
private:
    std::vector<std::unordered_map<std::string, SymbolEntry>> scopes;
    
public:
    SymbolTable() { enterScope(); } // 构造时进入全局作用域
    
    void enterScope();
    void exitScope();
    bool insert(const std::string& name, SymbolEntry entry);
    SymbolEntry* lookup(const std::string& name);
    SymbolEntry* lookupCurrentScope(const std::string& name);
    
    bool isEmpty() const { return scopes.empty(); }
    size_t getCurrentScopeLevel() const { return scopes.size() - 1; }
};