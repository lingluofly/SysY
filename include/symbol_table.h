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
    
    // 函数相关字段
    int paramCount; // 参数数量
    std::vector<Type> paramTypes; // 参数类型列表
    
    // 默认构造函数
    SymbolEntry() : kind(Kind::VARIABLE), type(Type::INT), isArray(false), valueType(ValueType::NONE), paramCount(0) {} 
    
    SymbolEntry(Kind kind, Type type, bool isArray = false)
        : kind(kind), type(type), isArray(isArray), valueType(ValueType::NONE), paramCount(0) {}
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