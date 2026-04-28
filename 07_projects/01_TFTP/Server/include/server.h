#pragma once
#include <string>           // 解决 string 报错
#include <sys/socket.h>     // 解决 socket 相关函数
#include <netinet/in.h>     // 解决 sockaddr_in 报错
#include <arpa/inet.h>      // 解决 IP 转换函数

class server
{
private:
    const int port=6666;
    const int BUF_SIZE=516;
    //服务器信息结构体
    struct sockaddr_in sin;
    //服务器与客户端连接的套接字文件
    int sockfd;
    //文件服务的根目录
    std::string root_dir;

private:
    // 处理客户端的请求函数
    // const char *filename：要发送给客户端的文件名
    // client_addr:客户端地址信息结构体
    // addr_len:地址信息结构体大小
    void handleReadRequest(const char *filename, struct sockaddr_in &client_addr, socklen_t addr_len); // 处理读请求

    // char *filename：将客户端发来的数据存储到的文件
    // client_addr:客户端地址信息结构体
    // addr_len:地址信息结构体大小
    void handleWriteRequest(const char *filename, struct sockaddr_in &client_addr, socklen_t addr_len); // 处理写请求

    // const char * msg：要发送给客户端的错误信息
    // client_addr:客户端地址信息结构体
    // addr_len:地址信息结构体大小
    void sendError(const char *msg, struct sockaddr_in &client_addr);
public:
    server(const std::string&dir=".");
    ~server();
    void run();
};