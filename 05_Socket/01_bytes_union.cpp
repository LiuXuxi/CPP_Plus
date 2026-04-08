#include<iostream>

union Info{
    int num;
    char c;
};

int main(){
    union Info info;
    info.num=0x12345678;
    if(info.c==0x12)std::cout<<"big endian"<<std::endl;
    else if(info.c==0x78)std::cout<<"little endian"<<std::endl;
    return 0;
}