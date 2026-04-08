#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<stdio.h>

int main(){
    //点分十进制表示的IP地址
    const char*ip="192.168.31.129";
    //将IP地址转换为无符号整数的网络字节序
    unsigned int ip_net=inet_addr(ip);
    //按照网络字节序（大端）模式输出
    printf("inet_ip is %#x\n",ip_net);
    //按照主机（小端）模式输出
    printf("inet_ip is %#x\n",ntohl(ip_net));
    return 0;
}