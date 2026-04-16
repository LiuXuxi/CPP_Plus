cmake_minimum_required(VERSION 3.20)

project(05_SOCKET)

# 1. 查找线程库
find_package(Threads REQUIRED)

# 2. 全局链接：此行之后的所有可执行文件都会自动链接 pthread
link_libraries(Threads::Threads)

add_executable(01_bytes_ptr 01_bytes_ptr.cpp)
add_executable(01_bytes_union 01_bytes_union.cpp)
add_executable(02_htonl 02_htonl.cpp)
add_executable(03_address 03_address.cpp)
add_executable(06_tcp_loop_server 06_tcp_loop_server.cpp)
add_executable(07_tcp_loop_proc_server 07_tcp_loop_proc_server.cpp)
add_executable(08_tcp_loop_thread_server 08_tcp_loop_thread_server.cpp)
add_executable(09_select 09_select.cpp)
add_executable(10_select_loop_server 10_select_loop_server.cpp)
add_executable(12_epoll_tcp_server 12_epoll_tcp_server.cpp)
add_executable(13_epoll_LT 13_epoll_LT.cpp)
add_executable(14_epoll_ET 14_epoll_ET.cpp)
add_executable(15_sockopt 15_sockopt.cpp)
