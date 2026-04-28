#include<iostream>
#include"client.h"

int main(int argc,const char *argv[]){
    //argv[1]:服务端IP地址，由用户输入
    if(argc!=2){
        std::cout<<"please input the server IP"<<'\n';
        return -1;
    }
    //如果运行过程中有错误则捕获错误
    try{
        //创建客户端
        client c(argv[1]);
        //开始运行客户端
        c.run();
    }
    //捕获异常
    catch(const std::exception&e){
        std::cerr<<e.what()<<'\n';
        return -1;
    }
    return 0;
}