# 指定支持的最低版本的cmake
cmake_minimum_required(VERSION 3.20)

# 项目名称
project(HELLO CXX)

# 生成可执行文件
add_executable(hello hello.cpp)