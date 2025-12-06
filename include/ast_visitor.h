#pragma once
class FuncFParam;
class FuncDef;
class CompUnit;

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    // 访问方法
    virtual void visit(FuncFParam& node) = 0;
    virtual void visit(FuncDef& node) = 0;
    virtual void visit(CompUnit& node) = 0;
};
