#include <arpa/inet.h>
#include<iostream>
#include<cstdio>

int main(){
    int num=0x12345678;
    //主机字节序
    printf("host:%#x\n",num);
    int res=htonl(num);
    //网络字节序
    printf("network:%#x\n",res);
    return 0;
}