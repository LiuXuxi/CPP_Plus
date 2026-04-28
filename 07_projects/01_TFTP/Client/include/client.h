#pragma once
#include<string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<arpa/inet.h>
#include <netinet/in.h>

class client
{
private:
    //the port of server
    static const int server_port=6666;
    //the max size of data or buf
    static const int BUF_SIZE=516;
    //the structure of server
    struct sockaddr_in sin;
    //the socket along  with server
    int sockfd;

private:
    //菜单界面：处理用户输入和展示菜单界面
    void MenuView();
    //用户从服务端下载文件
    int DownLoad();
    //用户向服务端上传文件
    int UpLoad();
    
public:
    //@server_ip:the ip of server
    //function:init the sin
    client(const std::string&server_ip);
    ~client();

    //客户端主业务
    void run();
};

