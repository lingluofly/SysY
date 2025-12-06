#pragma once
#include <string>
#include <vector>
#include "ast.h"

enum class IROp {
    ADD, SUB, MUL, DIV, MOD,
    AND, OR, NOT,
    EQ, NE, LT, GT, LE, GE,
    ASSIGN, LOAD, STORE,
    CALL, RET, GOTO,
    PHI, ALLOCA
};

struct IRInstruction {
    IROp op;
    std::string result;
    std::vector<std::string> operands;
    std::string label;
    
public:
    IRInstruction(IROp op, const std::string& result, 
                  const std::vector<std::string>& operands = {}, 
                  const std::string& label = "")
        : op(op), result(result), operands(operands), label(label) {}
    
    bool isTerminator() const;
    bool isPhi() const;
    
    IROp getOp() const { return op; }
    const std::string& getResult() const { return result; }
    const std::vector<std::string>& getOperands() const { return operands; }
    const std::string& getLabel() const { return label; }
};

class BasicBlock {
private:
    std::string name;
    std::vector<IRInstruction> instructions;
    std::vector<std::string> predecessors;
    std::vector<std::string> successors;

public:
    BasicBlock(const std::string& name) : name(name) {}
    
    const std::string& getName() const { return name; }
    const std::vector<IRInstruction>& getInstructions() const { return instructions; }
    const std::vector<std::string>& getPredecessors() const { return predecessors; }
    const std::vector<std::string>& getSuccessors() const { return successors; }
    
    void addInstruction(const IRInstruction& instr) { instructions.push_back(instr); }
    void addPredecessor(const std::string& pred) { predecessors.push_back(pred); }
    void addSuccessor(const std::string& succ) { successors.push_back(succ); }
};

class FunctionIR {
private:
    std::string name;
    Type returnType;
    std::vector<std::string> params;
    std::vector<BasicBlock> blocks;

public:
    FunctionIR(const std::string& name, Type returnType) 
        : name(name), returnType(returnType) {}
        
    const std::string& getName() const { return name; }
    Type getReturnType() const { return returnType; }
    const std::vector<std::string>& getParams() const { return params; }
    const std::vector<BasicBlock>& getBlocks() const { return blocks; }
    
    void addParam(const std::string& param) { params.push_back(param); }
    void addBlock(const BasicBlock& block) { blocks.push_back(block); }
};