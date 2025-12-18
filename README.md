# SysY Compiler

HDU编译原理课程实验四,使用C++语言,基于SysY语言的编译器实现。

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
│   ├── work1.md
│   ├── work2.md
│   ├── work3.md
│   └── work4.md
├── include/           # 头文件目录
│   ├── Lexer.h
│   ├── Parser.h
│   ├── ast.h
│   ├── ir.h
│   ├── semantic_analyzer.h
│   ├── symbol_table.h
│   └── token.h
├── src/               # 源代码目录
│   ├── ast.cpp
│   ├── lexer.cpp
│   ├── main.cpp
│   ├── parser.cpp
│   ├── scanner.l
│   ├── semantic_analyzer.cpp
│   └── symbol_table.cpp
├── tests/             # 测试文件目录
│   ├── work1_test/   # 第一阶段测试用例
│   │   ├── array_loop_test.sy
│   │   ├── basic_test.sy
│   │   ├── condition_test.sy
│   │   ├── function_test.sy
│   │   ├── nested_loop_test.sy
│   │   ├── variable_test.sy
│   │   └── while_loop_test.sy
│   ├── work2_test/   # 第二阶段测试用例
│   │   ├── example1.sy
│   │   ├── example2.sy
│   │   ├── example3.sy
│   │   ├── example3_error1.sy
│   │   └── example3_error2.sy
│   ├── work3_test/   # 第三阶段测试用例
│   │   ├── array_program.sy
│   │   ├── basic_variables.sy
│   │   ├── control_flow.sy
│   │   ├── correct_syntax.sy
│   │   ├── empty_program.sy
│   │   ├── function_program.sy
│   │   ├── mismatched_brackets.sy
│   │   ├── missing_semicolon.sy
│   │   ├── multiple_declarations.sy
│   │   └── number_constants.sy
│   └── work4_test/   # 第四阶段测试用例
│       ├── const_assignment_error.sy
│       ├── non_integer_array_index.sy
│       ├── redefined_function.sy
│       ├── redefined_variable.sy
│       ├── return_type_mismatch.sy
│       ├── type_mismatch_assignment.sy
│       ├── type_mismatch_operands.sy
│       ├── type_mismatch_return.sy
│       ├── undefined_function.sy
│       ├── undefined_variable.sy
│       └── wrong_argument_count.sy
└── tools/             # 工具目录
    └── test_runner.ps1
```

## 功能特性

- 词法分析：直接用C++编写
- 语法分析：直接用C++编写
- 语义分析：实现类型检查、作用域管理等
- 中间代码生成：生成LLVM IR

## 构建方法

### 前提条件
- 安装CMake 4.2.0及以上
- 安装C++编译器（Visual Studio 2019+ 或 MinGW-w64）

### 使用 Visual Studio 构建
1. 创建build目录：`mkdir build`
2. 进入build目录：`cd build`
3. 运行CMake：`cmake ..`
4. 编译项目：`cmake --build .`

### 使用 MinGW-w64 构建 (推荐)
1. **配置环境变量**：将 MinGW-w64 的 `bin` 目录添加到系统 PATH（如：`E:\mingw64\bin`）
2. **清理旧构建（可选）**：如果之前使用过其他编译器，删除旧的 build 目录：`rm -rf build`
3. 创建build目录：`mkdir build`
4. 进入build目录：`cd build`
5. 运行CMake（指定MinGW生成器）：`cmake .. -G "MinGW Makefiles"`
6. 编译项目（使用MinGW的make命令）：`mingw32-make`

## 快速启动

### 环境要求
- Windows系统
- PowerShell 5.0及以上
- CMake 4.2.0及以上
- C++编译器（推荐安装Visual Studio 2019及以上版本，或MinGW-w64）

### 自动化测试
1. 打开PowerShell终端 
2. 导航到项目根目录：`cd e:\alwaysused\c_c++\c++\SysY` 
3. 运行测试脚本：`tools\test_runner.ps1` 

### 预期结果 
根据指定的work参数，测试脚本会运行相应的测试用例。例如运行Work 2测试：
``` 
[Work 2] 语法分析测试
-----------------------------------------------------
运行 ..\tests\work2_test\example2.sy...
输出:
  INTTK int
  ID inc
  LPARENT (
  RPARENT )
  LBRACE (
  INTTK int
  ID i
  SEMICN ;
  ID i
  ASSIGN =
  ID i
  PLUS +
  INTCON 1
  SEMICN ;
  RBRACE )
  Warning: function 'inc' should return a value
退出码: 0
 ✓ [通过]
运行 ..\tests\work2_test\example3.sy...
输出:
  INTTK int
  ID main
  LPARENT (
  RPARENT )
  LBRACE (
  INTTK int
  ID i
  ASSIGN =
  INTCON 83
  SEMICN ;
  INTTK int
  ID j
  ASSIGN =
  INTCON 63
  SEMICN ;
  RBRACE )
  Warning: function 'main' should return a value
退出码: 0
 ✓ [通过]
-----------------------------------------------------
[Work 2] 语法分析测试: 2/2 通过

[Work 2] 语法分析错误测试
-----------------------------------------------------
运行 ..\tests\work2_test\example1.sy...
输出:
  Error type A at line 4 : Invalid character '~'
退出码: 1
 ✓ [预期失败]
运行 ..\tests\work2_test\example3_error1.sy...
输出:
  Error type A at line 3 : illegal octal number '09'
退出码: 1
 ✓ [预期失败]
运行 ..\tests\work2_test\example3_error2.sy...
输出:
  Error type A at line 4 : illegal hexadecimal number '0x3G'
退出码: 1
 ✓ [预期失败]
-----------------------------------------------------
[Work 2] 语法分析错误测试: 3/3 通过

=====================================================
测试汇总报告
=====================================================
[Work 2] 语法分析测试: 2/2 (100%)
[Work 2] 语法分析错误测试: 3/3 (100%)
-----------------------------------------------------
总测试结果: 5/5 (100%) 通过
=====================================================

🎉 所有测试通过！
```

## 使用方法

```bash
./sysy_compiler <input_file.sy>
```


## 参考文档

- [SysY2022语言定义-V1.pdf](docx/SysY2022语言定义-V1.pdf)
- [Flex词法分析实验工具指导.pdf](docx/Flex词法分析实验工具指导.pdf)