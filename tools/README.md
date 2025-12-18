# SysY 编译器测试工具

这个目录包含SysY编译器的自动化测试工具，用于测试编译器的各个功能模块。

## 测试工具

### test_runner.ps1

这是一个PowerShell脚本，用于自动化运行所有测试用例或指定work的测试用例。

#### 功能特性

- ✅ 运行所有work的测试用例
- ✅ 根据指定的work过滤测试
- ✅ 支持预期失败的测试用例（如语法错误和语义错误）
- ✅ 详细的测试报告和结果汇总
- ✅ 支持命令行参数

#### 使用方法

1. **确保你已经在build目录中**
   ```powershell
   cd ..\build
   ```

2. **运行所有测试**
   ```powershell
   ..\tools\test_runner.ps1
   ```

3. **只运行特定work的测试**
   ```powershell
   ..\tools\test_runner.ps1 -Work 3
   ```

4. **查看帮助信息**
   ```powershell
   ..\tools\test_runner.ps1 -Help
   ```

#### 命令行参数

- `-Help`: 显示帮助信息
- `-Work`: 指定要运行的work测试（1, 2, 3, 4或all）

#### 测试结果说明

- ✅ [通过]: 测试用例预期通过且实际通过
- ✅ [预期失败]: 测试用例预期失败且实际失败
- ✗ [失败]: 测试用例预期通过但实际失败
- ✗ [预期失败但通过]: 测试用例预期失败但实际通过

## 测试用例目录结构

测试用例位于项目根目录的`tests`文件夹下，按work任务分类：

```
tests/
├── work1_test/    # 词法分析测试用例
├── work2_test/    # 语法分析测试用例
├── work3_test/    # 语法分析测试用例
└── work4_test/    # 语义分析测试用例
```

## 环境要求

- Windows系统
- PowerShell 5.0及以上
- CMake 4.2.0及以上
- C++编译器（推荐安装Visual Studio 2019及以上版本，或MinGW-w64）
- 已构建的SysY编译器（sysy_compiler.exe）

## 构建编译器

在运行测试之前，确保你已经构建了SysY编译器：

```powershell
# 创建build目录
mkdir build
cd build

# 使用Visual Studio构建
cmake ..
cmake --build .

# 或使用MinGW-w64构建
cmake .. -G "MinGW Makefiles"
mingw32-make
```

## 注意事项

1. 确保`sysy_compiler.exe`位于`build`目录中
2. 确保测试用例位于正确的目录中
3. 某些测试用例预期会失败（如语法错误和语义错误），这是正常的
4. 测试结果会显示详细的测试报告和汇总信息