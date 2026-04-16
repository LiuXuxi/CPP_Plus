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

int main(){
    //1.创建套接字
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1){
        perror("socket error");
        return -1;
    }
    printf("sockfd is %d\n",sockfd);

    //2.绑定IP和PORT
    //2.1创建并初始化地址信息结构体
    struct sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_port=htons(SER_PORT);
    sin.sin_addr.s_addr=inet_addr(SER_IP);
    
    if((bind(sockfd,(const sockaddr*)&sin,sizeof(sin)))==-1){
        perror("bind error");
        return -1;
    }
    printf("bind succeed\n");

    //3.启动监听状态
    if((listen(sockfd,128))==-1){
        perror("listen error");
        return -1;
    }
    printf("listen succeed\n");

    //4.接受一个客户端
    //记录客户端通信信息的结构体
    struct sockaddr_in cin;
    socklen_t len=sizeof(cin);
    
    while(1){
        int newfd=accept(sockfd,(struct sockaddr*)&cin,&len);
        if(newfd==-1){
            perror("accept error");
            return -1;
        }
        printf("[%s:%d]:accept succeed\n",inet_ntoa(cin.sin_addr),ntohs(cin.sin_port));

        //5.接受并处理客户端发送的消息
        char rbuf[128];
        char callback[128]="*_*";
        while(1){
            //清空容器
            memset(rbuf,0,sizeof(rbuf));
            //阻塞接受
            recv(newfd,rbuf,sizeof(rbuf),0);
            printf("recv from [%s:%d]:%s\n",inet_ntoa(cin.sin_addr),ntohs(cin.sin_port),rbuf);
            //给客户端回复消息
            if(send(newfd,callback,strlen(callback),0)==-1){
                perror("send error");
                return -1;
            }

            if(strcmp(rbuf,"quit")==0)break;
        }

        //关闭服务器文件描述符
        close(newfd);
    }

    //6.关闭网络通信
    close(sockfd);
    printf("close socket...\n");
    return 0;
}
