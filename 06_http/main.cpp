#include<iostream>
#include<thread>
#include <unistd.h>
#include"http.h"
#include <sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

//工作线程处理函数-处理客户端
void work(int fd){
    //处理客户端
    cli_handler(fd);

    //关闭客户端
    close(fd);
}

int main(int argc,char* argv[]){
    //默认80端口
    int port=80;
    //用户指定端口
    if(argc>1){
        //将字符串端口转换为数字端口
        port=atoi(argv[1]);
    }
    //1.初始化服务器
    int sockfd=init_server(port);

    //接受客户端的信息结构体
    struct sockaddr_in cli;
    socklen_t len=sizeof(cli);

    //2.服务器处理循环
    while(1){
        //2.1接受客户端
        int newfd=accept(sockfd,(sockaddr*)&cli,&len);
        if(newfd==-1){
            perror("accept error");
            return -1;
        }
        std::cout<<"new client has been connect"<<std::endl;
        //2.2并发处理客户端
        //使用多线程进行并发处理
        std::thread worker(work,newfd);
        worker.detach();
    }
       
    //3.关闭服务器
    close(sockfd);
    
    return 0;
}
