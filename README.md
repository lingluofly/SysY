# SysY Compiler

一个基于SysY语言的编译器实现。

## 项目结构

```
SysY/
├── .vscode/          # VS Code配置文件
├── CMakeLists.txt    # CMake构建配置
├── README.md          # 项目说明文档
├── bin/               # 可执行文件输出目录
├── build/             # 构建中间文件目录
├── docx/              # 文档目录
│   ├── Flex词法分析实验工具指导.pdf
│   ├── SysY2022语言定义-V1.pdf
│   └── 任务4.1 SysY语言的语法和语义规范理解.pdf
├── include/           # 头文件目录
│   ├── Lexer.h
│   ├── Parser.h
│   ├── ast.h
│   ├── ir.h
│   ├── semantic_analyzer.h
│   ├── symbol_table.h
│   └── token.h
├── src/               # 源代码目录
│   ├── lexer.cpp
│   ├── parser.cpp
│   ├── scanner.l
│   ├── semantic_analyzer.cpp
│   └── symbol_table.cpp
├── test/              # 测试文件目录
│   ├── array_loop_test.sy
│   └── basic_test.sy
└── tools/             # 工具目录
    ├── test_runner.ps1
    └── test_runner.py
```

## 功能特性

- 词法分析：基于Flex实现
- 语法分析：基于Bison实现
- 语义分析：实现类型检查、作用域管理等
- 中间代码生成：生成LLVM IR

## 快速启动

### 环境要求
- Windows系统
- PowerShell 5.0及以上
- CMake 4.2.0及以上

### 启动步骤
1. 打开PowerShell终端
2. 导航到项目根目录：`cd e:\alwaysused\c_c++\c++\SysY`
3. 进入build目录：`cd build`
4. 运行测试脚本：`..\tools\test_runner.ps1`

### 预期结果
所有7个测试用例都应该编译成功，输出如下：
```
✓ work1_test/basic_test.sy 编译成功
✓ work1_test/array_loop_test.sy 编译成功
✓ work1_test/condition_test.sy 编译成功
✓ work1_test/function_test.sy 编译成功
✓ work1_test/nested_loop_test.sy 编译成功
✓ work1_test/variable_test.sy 编译成功
✓ work1_test/while_loop_test.sy 编译成功
```

## 构建方法

1. 安装CMake、Flex和Bison
2. 创建build目录：`mkdir build && cd build`
3. 运行CMake：`cmake ..`
4. 编译项目：`make`

## 使用方法

```bash
./sysy_compiler <input_file.sy>
```

## 测试

运行自动化测试：

```bash
..\tools\test_runner.ps1
```

## 参考文档

- [SysY2022语言定义-V1.pdf](docx/SysY2022语言定义-V1.pdf)
- [Flex词法分析实验工具指导.pdf](docx/Flex词法分析实验工具指导.pdf)