# 版本要求
cmake_minimum_required(VERSION 3.20)

project(Compiler)

# 设置 CXX 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 生成可执行文件 
add_executable(Test test.cpp parser.cpp lex.cpp)