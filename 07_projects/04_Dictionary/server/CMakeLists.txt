cmake_minimum_required(VERSION 3.20)

project(DICTIONARY_SERVER CXX)

# 设置 C++ 标准（Connector/C++ 8.0 要求至少 C++11，建议使用 17）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS Msg.proto)

# --- 手动指定 MySQL Connector 路径 ---
set(CONNECTOR_DIR "/usr/local/mysql-connector-cpp")

# 1. 指定头文件搜索路径
include_directories(${CONNECTOR_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR})

# 2. 指定库文件搜索路径
link_directories(${CONNECTOR_DIR}/lib64)

# 3. 生成可执行文件
add_executable(server src/main.cpp src/DataManager.cpp src/main.cpp ${PROTO_SRCS})

# 4. 链接核心库
# mysqlcppconn8 是 X DevAPI 库（如果用的是旧版 JDBC 风格，则链接 mysqlcppconn）
target_link_libraries(server PUBLIC 
    mysqlcppconn8 
    ${Protobuf_LIBRARIES} 
    Threads::Threads)