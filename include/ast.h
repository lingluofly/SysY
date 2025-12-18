#pragma once
#include <vector>
#include <memory>
#include <string>
#include "token.h"

// 前向声明ASTVisitor类，用于实现访问者模式
class ASTVisitor;

// 数据类型枚举，表示SysY语言中的基本数据类型
enum class Type {
    INT,     // 整型
    FLOAT,   // 浮点型
    VOID     // 空类型
};

// 抽象语法树(AST)节点的基类
class ASTNode {
public:
    virtual ~ASTNode() = default; // 虚析构函数，确保子类正确析构
    virtual void accept(ASTVisitor& visitor) = 0; // 接受访问者，实现访问者模式
    virtual int getLine() const = 0; // 获取节点所在行号
};

// 其他AST节点类的前向声明
class Decl;
class FuncDef;
class Stmt;
class Expr;
class VarDef;
class FuncFParam;
class Block;

// 声明类的基类，继承自ASTNode
class Decl : public ASTNode {
public:
    virtual ~Decl() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

// 语句类的基类，继承自ASTNode
class Stmt : public ASTNode {
public:
    virtual ~Stmt() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

// 表达式类的基类，继承自ASTNode
class Expr : public ASTNode {
public:
    virtual ~Expr() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
    virtual Type getType() const = 0; // 获取表达式类型
};

// 变量定义节点类
class VarDef : public ASTNode {
private:
    std::string name;              // 变量名
    std::unique_ptr<Expr> initExpr; // 变量初始化表达式
    bool isArray;                  // 是否为数组
    int line;                      // 节点所在行号

public:
    // 构造函数
    VarDef(const std::string& name, std::unique_ptr<Expr> initExpr = nullptr, bool isArray = false, int line = 1)
        : name(name), initExpr(std::move(initExpr)), isArray(isArray), line(line) {}

    // 获取变量名
    const std::string& getName() const { return name; }
    // 获取初始化表达式
    Expr* getInitExpr() const { return initExpr.get(); }
    // 判断是否为数组
    bool getIsArray() const { return isArray; }
    // 获取节点所在行号
    int getLine() const override { return line; }

    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 函数形参节点类
class FuncFParam : public ASTNode {
private:
    Type type;           // 形参类型
    std::string name;    // 形参名
    bool isArray;        // 是否为数组类型
    int arraySize;       // 数组大小
    int line;            // 节点所在行号

public:
    FuncFParam() = default; // 默认构造函数
    // 带参构造函数
    FuncFParam(Type type, const std::string& name, bool isArray = false, int line = 1)
        : type(type), name(name), isArray(isArray), arraySize(0), line(line) {}

    // 获取形参类型
    Type getType() const { return type; }
    // 获取形参名
    const std::string& getName() const { return name; }
    // 判断是否为数组类型
    bool getIsArray() const { return isArray; }
    // 获取数组大小
    int getArraySize() const { return arraySize; }
    // 获取节点所在行号
    int getLine() const override { return line; }
    
    // 设置形参类型
    void setType(Type value) { type = value; }
    // 设置形参名
    void setName(const std::string& value) { name = value; }
    // 设置是否为数组类型
    void setIsArray(bool value) { isArray = value; }
    // 设置数组大小
    void setArraySize(int value) { arraySize = value; }

    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 编译单元节点类，代表整个程序
class CompUnit : public ASTNode {
private:
    std::vector<std::unique_ptr<Decl>> decls;     // 全局声明列表
    std::vector<std::unique_ptr<FuncDef>> funcDefs; // 函数定义列表

public:
    // 添加全局声明
    void addDecl(std::unique_ptr<Decl> decl) { decls.push_back(std::move(decl)); }
    // 添加函数定义
    void addFuncDef(std::unique_ptr<FuncDef> funcDef) { funcDefs.push_back(std::move(funcDef)); }
    
    // 获取全局声明列表
    const std::vector<std::unique_ptr<Decl>>& getDecls() const { return decls; }
    // 获取函数定义列表
    const std::vector<std::unique_ptr<FuncDef>>& getFuncDefs() const { return funcDefs; }
    // 获取节点所在行号（编译单元总是行号1）
    int getLine() const override { return 1; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 函数定义节点类
class FuncDef : public ASTNode {
private:
    Type returnType;                // 函数返回类型
    std::string name;               // 函数名
    std::vector<std::unique_ptr<FuncFParam>> params; // 函数形参列表
    std::unique_ptr<Block> body;   // 函数体
    int line;                       // 节点所在行号

public:
    FuncDef() = default; // 默认构造函数
    // 带参构造函数
    FuncDef(Type returnType, const std::string& name, std::unique_ptr<Block> body, int line = 1)
        : returnType(returnType), name(name), body(std::move(body)), line(line) {}
        
    // 获取函数返回类型
    Type getReturnType() const { return returnType; }
    // 获取函数名
    const std::string& getName() const { return name; }
    // 获取函数形参列表
    const std::vector<std::unique_ptr<FuncFParam>>& getParams() const { return params; }
    // 获取函数体
    Block* getBody() const { return body.get(); }
    // 获取节点所在行号
    int getLine() const override { return line; }
    
    // 设置函数返回类型
    void setReturnType(Type value) { returnType = value; }
    // 设置函数名
    void setName(const std::string& value) { name = value; }
    // 设置函数体
    void setBody(std::unique_ptr<Block> newBody) { body = std::move(newBody); }
    // 添加函数形参
    void addParam(std::unique_ptr<FuncFParam> param) { params.push_back(std::move(param)); }
    // 获取函数形参列表的引用
    std::vector<std::unique_ptr<FuncFParam>>& getParamsRef() { return params; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 变量声明节点类
class VarDecl : public Decl {
private:
    Type type;                       // 变量类型
    bool isConst;                    // 是否为常量
    std::vector<std::unique_ptr<VarDef>> varDefs; // 变量定义列表
    int line;                        // 节点所在行号

public:
    // 构造函数
    VarDecl(Type type, bool isConst, int line = 1) : type(type), isConst(isConst), line(line) {}
    
    // 获取变量类型
    Type getType() const { return type; }
    // 判断是否为常量
    bool getIsConst() const { return isConst; }
    // 获取变量定义列表
    const std::vector<std::unique_ptr<VarDef>>& getVarDefs() const { return varDefs; }
    // 获取节点所在行号
    int getLine() const override { return line; }
    
    // 添加变量定义
    void addVarDef(std::unique_ptr<VarDef> varDef) { varDefs.push_back(std::move(varDef)); }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// if语句节点类
class IfStmt : public Stmt {
private:
    std::unique_ptr<Expr> condition; // if条件表达式
    std::unique_ptr<Stmt> thenStmt;  // if语句块
    std::unique_ptr<Stmt> elseStmt;  // else语句块（可选）
    int line;                        // 节点所在行号

public:
    // 构造函数
    IfStmt(std::unique_ptr<Expr> condition, 
           std::unique_ptr<Stmt> thenStmt, 
           std::unique_ptr<Stmt> elseStmt = nullptr, 
           int line = 1)
        : condition(std::move(condition)), 
          thenStmt(std::move(thenStmt)), 
          elseStmt(std::move(elseStmt)), 
          line(line) {}
          
    // 获取条件表达式
    Expr* getCondition() const { return condition.get(); }
    // 获取if语句块
    Stmt* getThenStmt() const { return thenStmt.get(); }
    // 获取else语句块
    Stmt* getElseStmt() const { return elseStmt.get(); }
    // 获取节点所在行号
    int getLine() const override { return line; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// while语句节点类
class WhileStmt : public Stmt {
private:
    std::unique_ptr<Expr> condition; // while条件表达式
    std::unique_ptr<Stmt> body;      // while语句块
    int line;                        // 节点所在行号

public:
    // 构造函数
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body, int line = 1)
        : condition(std::move(condition)), body(std::move(body)), line(line) {}
    
    // 获取条件表达式
    Expr* getCondition() const { return condition.get(); }
    // 获取while语句块
    Stmt* getBody() const { return body.get(); }
    // 获取节点所在行号
    int getLine() const override { return line; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// return语句节点类
class ReturnStmt : public Stmt {
private:
    std::unique_ptr<Expr> expr; // 返回表达式
    int line;                   // 节点所在行号

public:
    // 构造函数
    ReturnStmt(std::unique_ptr<Expr> expr, int line = 1) : expr(std::move(expr)), line(line) {}
    
    // 获取返回表达式
    Expr* getExpr() const { return expr.get(); }
    // 获取节点所在行号
    int getLine() const override { return line; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 二元表达式节点类
class BinaryExpr : public Expr {
private:
    std::unique_ptr<Expr> left;  // 左操作数
    std::unique_ptr<Expr> right; // 右操作数
    TokenType op;                  // 操作符类型
    Type exprType;                 // 表达式类型

public:
    // 构造函数
    BinaryExpr(std::unique_ptr<Expr> left, TokenType op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)), exprType(Type::INT) {}
    
    // 获取左操作数
    Expr* getLeft() const { return left.get(); }
    // 获取右操作数
    Expr* getRight() const { return right.get(); }
    // 获取操作符类型
    TokenType getOp() const { return op; }
    // 获取表达式类型
    Type getType() const override { return exprType; }
    // 设置表达式类型
    void setType(Type type) { exprType = type; }
    // 获取节点所在行号
    int getLine() const override { return left ? left->getLine() : 1; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 一元表达式节点类
class UnaryExpr : public Expr {
private:
    TokenType op;                  // 操作符类型
    std::unique_ptr<Expr> operand; // 操作数
    Type exprType;                 // 表达式类型

public:
    // 构造函数
    UnaryExpr(TokenType op, std::unique_ptr<Expr> operand)
        : op(op), operand(std::move(operand)), exprType(Type::INT) {}
    
    // 获取操作符类型
    TokenType getOp() const { return op; }
    // 获取操作数
    Expr* getOperand() const { return operand.get(); }
    // 获取表达式类型
    Type getType() const override { return exprType; }
    // 设置表达式类型
    void setType(Type type) { exprType = type; }
    // 获取节点所在行号
    int getLine() const override { return operand ? operand->getLine() : 1; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 函数调用表达式节点类
class CallExpr : public Expr {
private:
    std::string callee;            // 被调用的函数名
    std::vector<std::unique_ptr<Expr>> args; // 函数调用参数列表
    Type exprType;                 // 表达式类型
    int line;                      // 节点所在行号

public:
    // 构造函数
    CallExpr(const std::string& callee, std::vector<std::unique_ptr<Expr>> args, int line = 1)
        : callee(callee), args(std::move(args)), exprType(Type::INT), line(line) {}
    
    // 获取被调用的函数名
    const std::string& getCallee() const { return callee; }
    // 获取函数调用参数列表
    const std::vector<std::unique_ptr<Expr>>& getArgs() const { return args; }
    // 获取表达式类型
    Type getType() const override { return exprType; }
    // 设置表达式类型
    void setType(Type type) { exprType = type; }
    // 获取节点所在行号
    int getLine() const override { return line; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 数组索引表达式节点类
class IndexExpr : public Expr {
private:
    std::unique_ptr<Expr> base;   // 数组基地址表达式
    std::unique_ptr<Expr> index;  // 索引表达式
    Type exprType;                 // 表达式类型

public:
    // 构造函数
    IndexExpr(std::unique_ptr<Expr> base, std::unique_ptr<Expr> index)
        : base(std::move(base)), index(std::move(index)), exprType(Type::INT) {}
    
    // 获取数组基地址表达式
    Expr* getBase() const { return base.get(); }
    // 获取索引表达式
    Expr* getIndex() const { return index.get(); }
    // 获取表达式类型
    Type getType() const override { return exprType; }
    // 设置表达式类型
    void setType(Type type) { exprType = type; }
    // 获取节点所在行号
    int getLine() const override { return base ? base->getLine() : 1; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 数字表达式节点类
class NumberExpr : public Expr {
private:
    int intValue; // 整数值
    float floatValue; // 浮点数值
    Type exprType; // 表达式类型
    int line;      // 节点所在行号

public:
    // 整数构造函数
    NumberExpr(int value, int line = 1) : intValue(value), floatValue(0.0f), exprType(Type::INT), line(line) {}
    // 浮点数构造函数
    NumberExpr(float value, int line = 1) : intValue(0), floatValue(value), exprType(Type::FLOAT), line(line) {}
    // 获取整数值
    int getIntValue() const { return intValue; }
    // 获取浮点数值
    float getFloatValue() const { return floatValue; }
    // 获取表达式类型
    Type getType() const override { return exprType; }
    // 设置表达式类型
    void setType(Type type) { exprType = type; }
    // 获取节点所在行号
    int getLine() const override { return line; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 变量表达式节点类
class VariableExpr : public Expr {
private:
    std::string name; // 变量名
    Type exprType; // 表达式类型
    int line;      // 节点所在行号

public:
    // 构造函数
    VariableExpr(const std::string& name, int line = 1) : name(name), exprType(Type::INT), line(line) {}
    // 获取变量名
    const std::string& getName() const { return name; }
    // 获取表达式类型
    Type getType() const override { return exprType; }
    // 设置表达式类型
    void setType(Type type) { exprType = type; }
    // 获取节点所在行号
    int getLine() const override { return line; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 代码块节点类
class Block : public Stmt {
private:
    std::vector<std::unique_ptr<Stmt>> statements; // 代码块中的语句列表
    int line;                                      // 节点所在行号

public:
    // 构造函数
    Block(int line = 1) : line(line) {}
    
    // 添加语句到代码块
    void addStatement(std::unique_ptr<Stmt> stmt) {
        statements.push_back(std::move(stmt));
    }
    
    // 获取代码块中的语句列表
    const std::vector<std::unique_ptr<Stmt>>& getStatements() const {
        return statements;
    }
    // 获取节点所在行号
    int getLine() const override { return line; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 表达式语句节点类
class ExprStmt : public Stmt {
private:
    std::unique_ptr<Expr> expr; // 语句中的表达式
    int line;                   // 节点所在行号

public:
    // 构造函数
    ExprStmt(std::unique_ptr<Expr> expr, int line = 1) : expr(std::move(expr)), line(line) {}
    // 获取语句中的表达式
    Expr* getExpr() const { return expr.get(); }
    // 获取节点所在行号
    int getLine() const override { return line; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// 声明语句节点类，用于在语句块中包含变量声明
class DeclStmt : public Stmt {
private:
    std::unique_ptr<Decl> decl; // 包装的声明
    int line;                   // 节点所在行号

public:
    // 构造函数
    DeclStmt(std::unique_ptr<Decl> decl, int line = 1) : decl(std::move(decl)), line(line) {}
    // 获取声明
    Decl* getDecl() const { return decl.get(); }
    // 获取节点所在行号
    int getLine() const override { return line; }
    
    // 接受访问者
    void accept(ASTVisitor& visitor) override;
};

// AST访问者基类，用于实现访问者模式
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    // 访问编译单元节点
    virtual void visit(CompUnit& node) = 0;
    // 访问函数定义节点
    virtual void visit(FuncDef& node) = 0;
    // 访问变量声明节点
    virtual void visit(VarDecl& node) = 0;
    // 访问if语句节点
    virtual void visit(IfStmt& node) = 0;
    // 访问while语句节点
    virtual void visit(WhileStmt& node) = 0;
    // 访问return语句节点
    virtual void visit(ReturnStmt& node) = 0;
    // 访问二元表达式节点
    virtual void visit(BinaryExpr& node) = 0;
    // 访问一元表达式节点
    virtual void visit(UnaryExpr& node) = 0;
    // 访问函数调用表达式节点
    virtual void visit(CallExpr& node) = 0;
    // 访问数组索引表达式节点
    virtual void visit(IndexExpr& node) = 0;
    // 访问数字表达式节点
    virtual void visit(NumberExpr& node) = 0;
    // 访问变量表达式节点
    virtual void visit(VariableExpr& node) = 0;
    // 访问代码块节点
    virtual void visit(Block& node) = 0;
    // 访问变量定义节点
    virtual void visit(VarDef& node) = 0;
    // 访问函数形参节点
    virtual void visit(FuncFParam& node) = 0;
    // 访问表达式语句节点
    virtual void visit(ExprStmt& node) = 0;
    // 访问声明语句节点
    virtual void visit(DeclStmt& node) = 0;
};