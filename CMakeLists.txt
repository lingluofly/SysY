cmake_minimum_required(VERSION 3.10)

# 项目名称
project(SysYCompiler)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(include)

# 源文件
set(SOURCES
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/semantic_analyzer.cpp
    src/symbol_table.cpp
    src/print_visitor.cpp
    src/main.cpp
)

# 头文件
set(HEADERS
    include/Lexer.h
    include/Parser.h
    include/ast.h
    include/ir.h
    include/semantic_analyzer.h
    include/symbol_table.h
    include/token.h
)

# 创建可执行文件
add_executable(sysy_compiler ${SOURCES} ${HEADERS})

# 链接库（无额外依赖）

# 安装目标
install(TARGETS sysy_compiler DESTINATION bin)

# 测试
enable_testing()
add_test(NAME basic_test COMMAND sysy_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test/basic_test.sy)
add_test(NAME array_loop_test COMMAND sysy_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test/array_loop_test.sy)