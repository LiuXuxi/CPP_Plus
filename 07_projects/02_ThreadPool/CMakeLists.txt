cmake_minimum_required(VERSION 3.20)

project(THREAD_POOL CXX)

add_executable(threadpool main.cpp threadpool.cpp)

target_link_libraries(threadpool PUBLIC pthread)