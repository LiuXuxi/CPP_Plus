#pragma once

#include <sys/socket.h>

//@port:服务器绑定的端口号
//return：返回服务器的套接字
//功能：初始化服务器
int init_server(int port);

//fd:和客户端通信的文件描述符
//return:
//  0:处理成功
//  1:处理错误
//功能：处理客户端
int cli_handler(int fd);