#include"client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include"arpa/inet.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

#define ERR_LOG(msg)                                                      \
    do                                                                    \
    {                                                                     \
        perror(msg);                                                      \
        std::cout << __LINE__ << "  " << __func__ << "  " << __FILE__ << std::endl; \
    } while (0)

client::client(const std::string&ip,const int port,const std::string& name):name(name)
{
    //1.创建套接字
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        ERR_LOG("socket error");
        close(sockfd);
        return;
    }
    
    //2.连接服务器端
    struct sockaddr_in sin;
    sin.sin_addr.s_addr=inet_addr(ip.c_str());
    sin.sin_family=AF_INET;
    sin.sin_port=htons(port);
    socklen_t len=sizeof(sin);
    if(connect(sockfd,(const sockaddr*)&sin,len)<0){
        ERR_LOG("connect error");
        close(sockfd);
        return;
    }

    //3.发送登录消息
    sendMsg(Msg_Type::LOGIN);
}

client::~client()
{
    sendMsg(Msg_Type::QUIT);
    if(sockfd>0)close(sockfd);
}

void client::run()
{
    // 1. 创建 epoll 实例
    int epfd = epoll_create(1);
    if (epfd == -1) {
        ERR_LOG("epoll_create error");
        return;
    }

    struct epoll_event ev, events[2];

    // 2. 将标准输入 (stdin) 加入 epoll 监听 —— 用于读取用户输入
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO; 
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

    // 3. 将服务器套接字 (sockfd) 加入 epoll 监听 —— 用于接收消息
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    std::string text;
    running=true;
    while (running)
    {
        // 4. 等待事件发生（阻塞）
        int n = epoll_wait(epfd, events, 2, -1);
        if (n < 0) {
            if (errno == EINTR) continue;
            ERR_LOG("epoll_wait error");
            break;
        }

        for (int i = 0; i < n; i++)
        {
            int fd = events[i].data.fd;

            // 情况 A：标准输入有数据（用户打了字）
            if (fd == STDIN_FILENO)
            {
                if (!std::getline(std::cin, text)) break;

                if (text == "quit") {
                    running=false;
                    break;
                }
                // 发送消息
                sendMsg(Msg_Type::CHAT, text);
            }
            // 情况 B：套接字有数据（服务器发来了消息）
            else if (fd == sockfd)
            {
                recvMsg();
            }
        }
    }

    close(epfd);
}

void client::recvMsg(){
    char buf[BUF_SIZE];
    memset(buf,0,BUF_SIZE);
    int recv_len=recv(sockfd,buf,BUF_SIZE,0);
    //std::cout<<"recv_len is "<<recv_len<<std::endl;
    if(recv_len<=0){
        ERR_LOG("recv error");
        running=false;
        return;
    }
    Msg msg;
    msg.ParseFromArray(buf,recv_len);
    std::cout<<msg.name()<<": "<<msg.data()<<std::endl;
}

void client::sendMsg(Msg_Type type,const std::string&data){
    Msg msg;
    msg.set_data(data.c_str());
    msg.set_type((int)type);
    msg.set_name(name.c_str());

    //序列化
    std::string output;
    msg.SerializeToString(&output);

    if(send(sockfd,output.c_str(),output.size(),0)<0){
        ERR_LOG("send error");
        running=false;
        return;
    }
}