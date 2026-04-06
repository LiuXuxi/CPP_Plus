cmake_minimum_required(VERSION 3.20)

project(04_PTHREAD)

# 1. 查找线程库
find_package(Threads REQUIRED)

# 2. 全局链接：此行之后的所有可执行文件都会自动链接 pthread
link_libraries(Threads::Threads)

add_executable(01_pthread_create 01_pthread_create.cpp)
add_executable(02_pthead_exit 02_pthead_exit.cpp)
add_executable(03_pthread_exit_argv 03_pthread_exit_argv.cpp)
add_executable(04_pthread_clean 04_pthread_clean.c)
add_executable(05_pthread_mutex 05_pthread_mutex.cpp)