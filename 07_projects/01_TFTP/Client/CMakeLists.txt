#指定cmake的最低版本
cmake_minimum_required(VERSION 3.20)

#项目名称以及项目所用语言
project(TFTP_CLIENT CXX)

#指定C++标准
set(CMAKE_CXX_STANDARD 11)

#全局添加路径-项目二进制目录下
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#生成可执行文件client
add_executable(client src/client.cpp src/main.cpp)

#给client加入include目录下的头文件
target_include_directories(client PUBLIC ${PROJECT_SOURCE_DIR}/include)