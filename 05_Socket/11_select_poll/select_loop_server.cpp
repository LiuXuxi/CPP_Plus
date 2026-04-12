#include<stdio.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <sys/select.h>
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

    //创建文件描述符集合，用于select
    //tempfd:用于文件描述符集合处理，防止readfd被改变
    fd_set readfd,tempfd;
    //加入stdin，用于输入复用
    FD_SET(0,&readfd);
    //加入sockfd，用于连接客户端复用
    FD_SET(sockfd,&readfd);
    //记录最大的文件描述符
    int maxfd=sockfd;

    //最新接受的客户端
    int newfd=-1;

    //定义一个地址信息结构体数组来存储客户端对应的地址信息
    struct sockaddr_in cins[1024];

    
    while(1){
        //只处理tempfd，而保留readfd的数据，用于循环处理保证数据完整
        tempfd=readfd;

        //等待文件描述符被响应，这里不设置等待时间，也就是一直阻塞
        select(maxfd+1,&tempfd,NULL,NULL,NULL);

        //当客户端响应时，处理客户端
        if(FD_ISSET(sockfd,&tempfd)){
            newfd=accept(sockfd,(struct sockaddr*)&cin,&len);
            if(newfd==-1){
                perror("accept error");
                return -1;
            }
            printf("[%s:%d]:accept succeed\n",inet_ntoa(cin.sin_addr),ntohs(cin.sin_port));

            //将新客户端的文件描述符加入到集合中
            FD_SET(newfd,&readfd);

            //将新客户端的信息结构体加入到数组中
            cins[newfd]=cin;

            //更新maxfd
            if(newfd>maxfd)maxfd=newfd;
            
        }

        //当输入响应时，处理输入数据
        if(FD_ISSET(0,&tempfd)){
            char buf[128]="";
            fgets(buf,sizeof(buf),stdin);
            buf[strlen(buf)-1]=0;
            printf("input:%s\n",buf);
            for(int i=4;i<=maxfd;i++){
                if((send(i,buf,strlen(buf),0))==-1){
                    perror("send error");
                    return -1;
                }
            }
        }

        for(int i=4;i<=maxfd;i++){
            if(FD_ISSET(i,&tempfd)){
                //5.接受并处理客户端发送的消息
                char rbuf[128];
                char callback[128]="*_*";
                //清空容器
                memset(rbuf,0,sizeof(rbuf));
                //阻塞接受
                if((recv(i,rbuf,sizeof(rbuf),0))==-1){
                    perror("recv error");
                    return -1;
                }
                printf("recv from [%s:%d]:%s\n",inet_ntoa(cins[i].sin_addr),ntohs(cins[i].sin_port),rbuf);
                //给客户端回复消息
                if(send(i,callback,strlen(callback),0)==-1){
                    perror("send error");
                    return -1;
                }

                if(strcmp(rbuf,"quit")==0){
                    //客户端下线，关闭相应的文件描述符
                    close(i);
                    //将文件描述符从集合中去除，防止影响下一次连接客户端
                    FD_CLR(i,&readfd);
                    //更新最大文件描述符
                    for(int j=maxfd;j>=4;j--){
                        if(FD_ISSET(j,&readfd)){
                            maxfd=j;
                            break;
                        }
                    }
                    printf("close from [%s:%d]:%s\n",inet_ntoa(cins[i].sin_addr),ntohs(cins[i].sin_port),rbuf);
                    //结束此次循环
                    continue;
                }
            
            }
        }
        
    }

    //6.关闭网络通信
    close(sockfd);
    printf("close socket...\n");
    return 0;
}