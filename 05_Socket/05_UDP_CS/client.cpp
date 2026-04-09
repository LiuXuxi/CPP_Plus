#include<stdio.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#define SER_IP "192.168.31.129"
#define SER_PORT 8888
#define CLI_IP "192.168.31.129"
#define CLI_PORT 7788

int main(){
    //1.创建套接字
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd==-1){
        perror("socket error");
        return -1;
    }
    printf("sockfd is %d\n",sockfd);

    //2.绑定IP和PORT
    //2.1创建并初始化地址信息结构体
    struct sockaddr_in cli;
    cli.sin_family=AF_INET;
    cli.sin_port=htons(CLI_PORT);
    cli.sin_addr.s_addr=inet_addr(CLI_IP);
    
    if((bind(sockfd,(const sockaddr*)&cli,sizeof(cli)))==-1){
        perror("bind error");
        return -1;
    }
    printf("bind succeed\n");

    //3.连接服务端
    //3.1创建并初始化服务端的通信信息结构体
    struct sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_port=htons(SER_PORT);
    sin.sin_addr.s_addr=inet_addr(SER_IP);

    //4.向服务端发送消息，并接受服务端回应的消息
    char wbuf[128];
    char rbuf[128];
    while(1){
        //4.0清空容器
        memset(wbuf,0,sizeof(wbuf));
        //4.1构造消息：从终端读取消息
        printf("please input>>>");
        fgets(wbuf,sizeof(wbuf),stdin);
        wbuf[strlen(wbuf)-1]=0;

        //4.2向服务端发送消息
        sendto(sockfd,wbuf,strlen(wbuf),0,(sockaddr*)&sin,sizeof(sin));

        //4.3接受服务端回应的消息
        recvfrom(sockfd,rbuf,sizeof(rbuf),0,NULL,NULL);
        printf("recv from [%s:%d]:%s\n",inet_ntoa(sin.sin_addr),ntohs(sin.sin_port),rbuf);

        //4.4接受quit时结束通信
        if(strcmp(wbuf,"quit")==0)break;
    }
    
    //5.关闭网络通信
    close(sockfd);
    printf("close socket...\n");
    return 0;
}