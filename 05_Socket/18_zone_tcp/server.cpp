#include<stdio.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <sys/un.h>

int main(){
    //1.创建套接字
    int sockfd=socket(AF_UNIX,SOCK_STREAM,0);
    if(sockfd==-1){
        perror("socket error");
        return -1;
    }
    printf("sockfd is %d\n",sockfd);

    struct sockaddr_un sin;
    sin.sun_family=AF_UNIX;
    strcpy(sin.sun_path,"./unix"); //用于与客户端通信的套接字文件

    //检查该套接字文件在不在
    if(access("./unix",F_OK)==0){
        //如果存在，则进行删除
        if(unlink("./unix")==-1){
            perror("unlink error");
            return -1;
        }
    }
    
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
    struct sockaddr_un cin;
    socklen_t len=sizeof(cin);
    int newfd=accept(sockfd,(struct sockaddr*)&cin,&len);
    if(newfd==-1){
        perror("accept error");
        return -1;
    }
    printf("[%s]:accept succeed\n",cin.sun_path);

    //5.接受并处理客户端发送的消息
    char rbuf[128];
    char callback[128]="*_*";
    while(1){
        //清空容器
        memset(rbuf,0,sizeof(rbuf));
        //阻塞接受
        recv(newfd,rbuf,sizeof(rbuf),0);
        printf("recv:%s\n",rbuf);
        //给客户端回复消息
        if(send(newfd,callback,strlen(callback),0)==-1){
            perror("send error");
            return -1;
        }

        if(strcmp(rbuf,"quit")==0)break;
    }

    //6.关闭网络通信
    close(sockfd);
    printf("close socket...\n");
    return 0;
}