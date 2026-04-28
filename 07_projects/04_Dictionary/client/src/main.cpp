#include<iostream>
#include"client.hpp"

int main(int argc,const char*argv[]){
    if(argc!=3){
        std::cerr<<"please input <ip> <port>"<<std::endl;
        return -1;
    }
    std::string ip=argv[1];
    int port=std::stoi(argv[2]);
    try{
        client c(ip,port);
        c.run();
    }catch(const std::exception&e){
        std::cerr<<e.what()<<std::endl;
        return -1;
    }
    return 0;
}