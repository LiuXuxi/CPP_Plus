#include"client.h"

int main(int argc,const char*argv[]){
    if(argc<4){
        std::cout<<"please input the ip and the port of the server,and your name"<<std::endl;
        return -1;
    }
    try{
        std::string ip=argv[1];
        int port=atoi(argv[2]);
        std::string name=argv[3];
        client s(ip,port,name);
        s.run();
    }catch(const std::exception&e){
        std::cerr<<e.what()<<std::endl;
        return -1;
    }
    return 0;
}