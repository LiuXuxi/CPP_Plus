cmake_minimum_required(VERSION 3.20)
project(CHATROOM_SERVER CXX)

# 1. 查找依赖包
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED) # 推荐用这种方式处理 pthread

# 2. 自动生成 Protobuf 源代码
# 注意顺序：第一个是生成的 .cc 列表，第二个是生成的 .h 列表
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS msg.proto)

# 3. 设置头文件搜索路径
# 必须包含 CMAKE_CURRENT_BINARY_DIR，因为生成的 msg.pb.h 在这里
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 4. 生成可执行程序
# 必须把生成的 ${PROTO_SRCS} 丢进去一起编译
add_executable(server 
    src/main.cpp 
    src/server.cpp 
    ${PROTO_SRCS}
)

# 5. 链接库
# 使用官方推荐的变量名
target_link_libraries(server 
    PUBLIC 
    ${Protobuf_LIBRARIES} 
    Threads::Threads
)