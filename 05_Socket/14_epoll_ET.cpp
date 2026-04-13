#include<stdio.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <sys/epoll.h>
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

    //创建epoll容器（检测集合）
    int epfd=epoll_create1(EPOLL_CLOEXEC);
    if(epfd==-1){
        perror("epoll create error");
        return -1;
    }

    //把sfd放入容器中
    struct epoll_event ev;
    ev.events=EPOLLIN;
    ev.data.fd=sockfd;
    if((epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev))==-1){
        perror("epoll ctl error");
        return -1;
    }

    //创建接受返回的事件集合
    struct epoll_event evs[1024];
    int size=sizeof(evs)/sizeof(evs[0]);
    
    while(1){
        //阻塞等待事件触发
        int num=epoll_wait(epfd,evs,size,-1);
        if(num==-1){
            perror("epoll wait error");
            return -1;
        }
        printf("num is %d",num);

        //遍历检测触发的事件
        for(int i=0;i<num;i++){
            //当前触发的事件文件描述符
            int fd=evs[i].data.fd;
            //说明连接了新客户端
            if(fd==sockfd){
                //处理新客户端
                int newfd=accept(sockfd,(struct sockaddr*)&cin,&len);
                if(newfd==-1){
                    perror("accept error");
                    return -1;
                }
                printf("[%s:%d]:accept succeed\n",inet_ntoa(cin.sin_addr),ntohs(cin.sin_port));

                //将新客户端加入检测集合中
                struct epoll_event newev;
                newev.events=EPOLLIN|EPOLLET;
                newev.data.fd=newfd;
                if((epoll_ctl(epfd,EPOLL_CTL_ADD,newfd,&newev))==-1){
                    perror("newfd add epoll error");
                    return -1;
                }
            }
            //说明旧客户端发送了消息
            else{
                //5.接受并处理客户端发送的消息
                char rbuf[5];
                char callback[128]="*_*";
                //清空容器
                memset(rbuf,0,sizeof(rbuf));
                //阻塞接受
                recv(fd,rbuf,sizeof(rbuf)-1,0);
                printf("recv from [%s:%d]:%s\n",inet_ntoa(cin.sin_addr),ntohs(cin.sin_port),rbuf);
                //给客户端回复消息
                if(send(fd,callback,strlen(callback),0)==-1){
                    perror("send error");
                    return -1;
                }

                if(strcmp(rbuf,"quit")==0){
                    
                    //从检测集合中删除客户端
                    if((epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL))==-1){
                        perror("epoll del fd error");
                        return -1;
                    }
                    //关闭连接该客户端的文件描述符
                    close(fd);
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