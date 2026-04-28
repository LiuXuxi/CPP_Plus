cmake_minimum_required(VERSION 3.20)

project(06_HTTP)

find_package(Threads REQUIRED)

link_libraries(Threads::Threads)

add_executable(http_server main.cpp http.cpp)
