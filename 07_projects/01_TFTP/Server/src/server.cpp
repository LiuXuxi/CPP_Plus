#include"server.h"
#include <iostream>       // 用于标准输入输出 (std::cout, std::cerr)
#include <string>         // 用于 std::string 类
#include <cstring>        // 用于 C 风格字符串操作 (memset, strlen, strcpy)
#include <vector>         // 可选，用于缓冲区管理
#include <fstream>        // 用于文件读写操作 (ifstream, ofstream)
#include <unistd.h>       // 用于 POSIX 系统调用 (close, read, write)
#include <sys/types.h>    // 包含基本系统数据类型
#include <sys/socket.h>   // 包含套接字相关函数 (socket, bind, recvfrom, sendto)
#include <netinet/in.h>   // 包含互联网地址族结构体 (sockaddr_in)
#include <arpa/inet.h>    // 包含 IP 地址转换函数 (inet_ntoa, htons)
#include <fcntl.h>

// 用户打印错误信息的宏
#define ERR_LOG(msg)                                                      \
    do                                                                    \
    {                                                                     \
        perror(msg);                                                      \
        std::cout << __LINE__ << "  " << __func__ << "  " << __FILE__ << std::endl; \
    } while (0)

server::server(const std::string&dir){
    //1.创建套接字文件
    this->sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0){
        ERR_LOG("socket error");
        return;
    }
    //2.初始化服务器信息结构体
    sin.sin_addr.s_addr=inet_addr("192.168.31.129");
    sin.sin_family=AF_INET;
    sin.sin_port=htons(port);
    this->root_dir=dir;

    //3.设置端口快速复用
    int reuse=1;
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))<0){
        ERR_LOG("setsockopt error");
        return;
    }

    //4.绑定服务器端口和IP
    if((bind(sockfd,(const sockaddr*)&sin,(socklen_t)sizeof(sin)))<0){
        ERR_LOG("bind error");
        return;
    }

}

server::~server()
{
    if(sockfd>0)close(sockfd);
}

//服务器主业务
void server::run(){
    char buf[BUF_SIZE]="";
    struct sockaddr_in cli;
    socklen_t len=sizeof(cli);
    while(1){
        memset(buf,0,sizeof(buf));
        //1.接受客户端的请求包
        if(recvfrom(sockfd,buf,sizeof(buf),0,(sockaddr*)&cli,&len)<0){
            ERR_LOG("recvfrom error");
            return;
        }
        //检查包协议是否正确
        if(buf[0]!=0)continue;
        //2.解析请求包
        char* filename=buf+2;
        char*mode=filename+strlen(filename)+1;
        //检查传输格式是否正确
        if(strcmp(mode,"octet")!=0){
            sendError("error transfer mode",cli);
            continue;
        }
        //3.处理正常请求
        switch (buf[1])
        {
        case 1: // 读请求
            std::cout << "read request for :" << filename << std::endl;
            // 调用处理读请求的函数
            handleReadRequest(filename, cli, len);
            break;

        case 2: // 写请求
            std::cout << "write request for :" << filename << std::endl;
            // 调用处理读请求的函数
            handleWriteRequest(filename, cli, len);
            break;
        default:
            sendError("Unknown request",cli); // 处理异常请求
            break;
        }
    }
}

// 处理客户端的请求函数
// const char *filename：要发送给客户端的文件名
// client_addr:客户端地址信息结构体
// addr_len:地址信息结构体大小
void server::handleReadRequest(const char *filename, struct sockaddr_in &cli, socklen_t len){
    //1.获取文件路径
    std::string path=root_dir+'/'+filename;
    //判断文件是否存在
    int fd=open(path.c_str(),O_RDONLY);
    if(fd<0){
        sendError("Not found the filename",cli);
        close(fd);
        return;
    }

    char buf[BUF_SIZE]="";
    int recv_len;
    unsigned short num=1;
    while(1){
        memset(buf,0,sizeof(buf));
        //2.打包数据包
        //打包前置协议
        buf[1]=3;
        *(unsigned short*)(buf+2)=htons(num);
        //打包数据
        recv_len=read(fd,buf+4,BUF_SIZE-4);
        if(recv_len<0){
            close(fd);
            ERR_LOG("read error");
            return;
        }

        //3.发送数据包
        if(sendto(sockfd,buf,recv_len+4,0,(const sockaddr*)&cli,len)<0){
            close(fd);
            ERR_LOG("sendto error");
            return ;
        }

        //4.循环等待接受正确的ACK
        do{
            if(recvfrom(sockfd,buf,BUF_SIZE,0,(sockaddr*)&cli,&len)<0){
                close(fd);
                ERR_LOG("recvfrom error");
                return;
            }
        }while(buf[1]!=4||*(unsigned short*)(buf+2)!=htons(num));

        //判断文件是否传输完毕
        if(recv_len<512)break;
        ++num;
    }
    close(fd);
} 

// char *filename：将客户端发来的数据存储到的文件
// client_addr:客户端地址信息结构体
// addr_len:地址信息结构体大小
void server::handleWriteRequest(const char *filename, struct sockaddr_in &cli, socklen_t len){
    //1.获取文件路径
    const std::string path=root_dir+'/'+filename;

    //判断是否能够接受上传文件
    int fd=open(path.c_str(),O_WRONLY|O_CREAT|O_TRUNC);
    if(fd<0){
        close(fd);
        sendError("upload failed",cli);
        ERR_LOG("open error");
        return;
    }

    //2.向客户端发送ACK(0)
    char buf[BUF_SIZE];
    unsigned short num=0;
    buf[0]=0,buf[1]=4;
    *(unsigned short*)(buf+2)=htons(num);
    if(sendto(sockfd,buf,4,0,(const sockaddr*)&cli,len)<0){
        close(fd);
        ERR_LOG("sendto error");
        return;
    }

    int recv_len;
    while(1){
        memset(buf,0,sizeof(buf));
        //3.等待接受数据包
        recv_len=recvfrom(sockfd,buf,BUF_SIZE,0,(sockaddr*)&cli,&len);
        if(recv_len<0){
            close(fd);
            ERR_LOG("recvfrom error");
            return;
        }
        //判断块编号和包类型是否正确
        if(buf[1]==3&&*(unsigned short*)(buf+2)==htons(num+1)){
            //4.将数据写入本地文件
            if(write(fd,buf+4,recv_len-4)<0){
                close(fd);
                sendError("write error",cli);
                ERR_LOG("write error");
                return;
            }

            //5.发送ACK
            ++num;
            buf[0]=0,buf[1]=4;
            *(unsigned short*)(buf+2)=htons(num);
            if(sendto(sockfd,buf,4,0,(const sockaddr*)&cli,len)<0){
                close(fd);
                ERR_LOG("sendto error");
                return;
            }

            //判断是否上传完毕
            if(recv_len<BUF_SIZE)break;
        }
    }
    close(fd);
}

// const char * msg：要发送给客户端的错误信息
// client_addr:客户端地址信息结构体
// addr_len:地址信息结构体大小
void server::sendError(const char *msg, struct sockaddr_in &cli){
    //组装错误包
    char buf[BUF_SIZE];
    buf[0]=0,buf[1]=5;
    buf[2]=0,buf[3]=1;
    strcpy(buf+4,msg);

    //发送错误包
    if(sendto(sockfd,buf,BUF_SIZE,0,(const sockaddr*)&cli,sizeof(cli))<0){
        ERR_LOG("sendto error");
        return;
    }
}
