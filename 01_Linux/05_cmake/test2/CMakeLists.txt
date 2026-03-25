cmake_minimum_required(VERSION 3.20)

project(MYSWAP CXX)

include_directories(./include)

add_executable(myswap main.cpp ./scr/myswap.cpp)