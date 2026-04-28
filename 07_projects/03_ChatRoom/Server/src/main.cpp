#include"server.h"
#include<iostream>

int main(int argc,const char*argv[]){
    if(argc<3){
        std::cout<<"please input the ip and the port of the server"<<std::endl;
        return -1;
    }
    try{
        std::string ip=argv[1];
        int port=atoi(argv[2]);
        server s(ip,port);
        s.run();
    }catch(const std::exception&e){
        std::cout<<e.what()<<std::endl;
        return -1;
    }
    return 0;
}