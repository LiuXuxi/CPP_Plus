#pragma once
#include"msg.pb.h"
#include<string>
#include<map>
#include"arpa/inet.h"
#include<sys/socket.h>
#include<thread>
#include<mutex>
#include<queue>
#include<condition_variable>
#include<functional>

enum class Msg_Type{
    LOGIN=1,
    CHAT=2,
    QUIT=3
};

class server
{
private:
    const int BUF_SIZE=128;

    //套接字文件
    int sockfd;
    //客户端容器
    std::map<int,struct sockaddr_in>clients;
    //保护客户端的互斥锁
    std::mutex client_mutex;

    //存储线程的线程容器
    std::vector<std::thread>workers;
    //存储任务的工作队列
    std::queue<std::function<void()>>tasks;
    //互斥锁：解决线程池的互斥问题
    std::mutex task_mutex;
    //条件变量，用于解决同步问题
    std::condition_variable task_cv;
    //线程池工作状态的标志
    bool isRun;

private:
    //启动线程池
    void startThreadPool(const int ThreadSize);
    //@task:指定的任务
    //添加任务到线程池的工作队列
    void addTask(std::function<void()>task);
    //@client_fd:客户端的文件描述符
    //@cin:客户端的信息结构体
    // 处理客户端连接的函数
    void handleClient(int client_fd, struct sockaddr_in cin);
    //@msg：发送客户端的消息
    //@exclude_fd:不转发消息的客户端
    // 广播消息函数        
    void broadcast(const Msg&msg, int exclude_fd = -1);             
public:
    //@ip:服务器端绑定的ip
    //@port:服务器端绑定的端口号
    //@ThreadSize:服务器端线程池初始化的工作线程数量
    server(const std::string&ip,const int port,const int ThreadSize=4);
    //析构函数  
    ~server();
    //服务器端主线程主流程
    void run();
};

