#include<iostream>

int main(){
    //四个字节
    int num=0x12345678;
    //指向num，并一个字节顺序寻址
    char *ptr=(char*)&num;
    if(*ptr==0x12)std::cout<<"big endian"<<std::endl;
    else if(*ptr==0x78)std::cout<<"little endian"<<std::endl;
    return 0;
}