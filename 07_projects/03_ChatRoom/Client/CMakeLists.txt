cmake_minimum_required(VERSION 3.20)

project(CHATROOM_CLIENT CXX)

find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HADS msg.proto)

add_executable(client src/main.cpp src/client.cpp ${PROTO_SRCS})

target_link_libraries(client PUBLIC Threads::Threads ${Protobuf_LIBRARIES})