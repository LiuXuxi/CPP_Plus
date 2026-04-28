cmake_minimum_required(VERSION 3.20)

project(TFTP_SERVER CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(server src/main.cpp src/server.cpp)

target_link_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/include)