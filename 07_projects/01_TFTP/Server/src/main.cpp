#include"server.h"
#include<iostream>

int main(int argc,const char*argv[]){
    try{
        std::string dir=(argc>1)?argv[1]:".";
        server s(dir);
        s.run();
    }catch(const std::exception&e){
        std::cerr<<e.what()<<std::endl;
        return -1;
    }
    return 0;
}