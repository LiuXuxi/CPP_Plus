#include"server.h"
#include <unistd.h>
#include <netinet/in.h>

// 用户打印错误信息的宏
#define ERR_LOG(msg)                                                      \
    do                                                                    \
    {                                                                     \
        perror(msg);                                                      \
        std::cout << __LINE__ << "  " << __func__ << "  " << __FILE__ << std::endl; \
    } while (0)

server::server(const std::string&ip,const int port,const int ThreadSize):isRun(true){
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    //1.创建套接字
    if(sockfd<0){
        ERR_LOG("socket error");
        return;
    }

    //2.设置端口快速复用
    int res=1;
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&res,sizeof(res))<0){
        ERR_LOG("setsockopt error");
        close(sockfd);
        return;
    }

    //3.绑定ip和端口号
    struct sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr=inet_addr(ip.c_str());
    sin.sin_port=htons(port);
    socklen_t len=sizeof(sin);
    if(bind(sockfd,(const sockaddr*)&sin,len)<0){
        ERR_LOG("bind error");
        close(sockfd);
        return;
    }

    //4.启动监听状态
    if(listen(sockfd,128)<0){
        ERR_LOG("listen error");
        return;
    }

    //5.初始化线程池
    startThreadPool(ThreadSize);
}

server::~server(){
    //避免死锁：防止当工作线程在执行任务就被通知
    {
        //主线程抢锁，防止工作线程影响
        std::unique_lock<std::mutex> lock(task_mutex);
        //关闭线程池
        isRun=false;
        //当局部变量离开作用域时调用析构函数自动释放锁
    }
    //主线程唤醒所有工作线程
    task_cv.notify_all();
    //主线程等待回收所有工作线程
    for(auto&worker:workers)worker.join();
    close(sockfd);
}

void server::run(){
    //创建接受客户端的信息结构体
    struct sockaddr_in cli;
    socklen_t len=sizeof(cli);

    //循环接受客户端
    while(1){
        int fd=accept(sockfd,(sockaddr*)&cli,&len);
        if(fd<0){
            ERR_LOG("accept error");
            break;
        }
        //添加客户端任务到任务队列中
        addTask([this,fd,cli]{
            handleClient(fd,cli);
        });
    }
}

void server::addTask(std::function<void()>task){
    {
        //主线程抢锁，防止工作线程影响
        std::unique_lock<std::mutex> lock(task_mutex);
        //将任务加入到工作队列中，等待有空间的工作线程执行任务
        tasks.emplace(task);
        //当局部变量离开作用域时调用析构函数自动释放锁
    }
    //主线程通知一个线程执行任务
    task_cv.notify_one();
}

void server::startThreadPool(const int ThreadSize){
    //创建并初始化指定数量的线程
    for(int i=0;i<ThreadSize;i++){
        //emplace_back：直接创建一个线程添加到线程容器中
        //lambda表达式作为线程的任务
        workers.emplace_back([this]{
            while(1){
                //创建一个任务，工作线程可能同时执行各自任务，所以执行任务时不占用锁
                //但是从工作队列中拿取任务时操作临界资源需要抢占锁
                std::function<void()>task;
                {
                    //抢占锁
                    std::unique_lock<std::mutex> lock(task_mutex);
                    //等待被主线程唤醒：当线程池停止或有任务可取时返回
                    task_cv.wait(lock,[this]{
                        return isRun==false || !tasks.empty();
                    });
                    //当线程池停止且工作队列为空时，线程退出
                    if(isRun==false && tasks.empty()) return;
                    //线程池正常时，工作线程从工作队列中取出任务
                    task=std::move(tasks.front());
                    tasks.pop();
                }
                //拿到任务后开始执行
                task();
            }
        });
    }
}

void server::handleClient(int client_fd, struct sockaddr_in cin){
    //创建接受消息的容器
    char buf[BUF_SIZE]="";
    Msg msg;
    std::string input;
    int recv_len=0;
    
    while(1){
        memset(buf,0,BUF_SIZE);
        recv_len=recv(client_fd,buf,BUF_SIZE,0);
        //std::cout<<"recv_len is "<<recv_len<<std::endl;
        //表示客户端下线，删除客户端并结束工作线程
        if(recv_len<=0){
            //删除客户端
            for(auto&[fd,cli]:clients){
                if(fd==client_fd){
                    clients.erase(fd);
                    break;
                }
            }
            close(client_fd);
            break;
        }
        //成功收到客户端消息，反序列化消息，并判断消息类型
        msg.ParseFromArray(buf,recv_len);
        //std::cout<<msg.type()<<" "<<msg.name()<<" "<<msg.data()<<std::endl;

        switch (msg.type())
        {
        case (int)Msg_Type::LOGIN:
            //保护客户端容器（临界资源）
            {
                std::unique_lock<std::mutex>lock(client_mutex);
                //把新连接的客户端放入到服务器端的客户端容器中
                clients.emplace(client_fd,cin);
                //组装消息并发送给所有其他客户端
                sprintf(buf,"---------%s:login succeed---------",msg.name().c_str());
                msg.set_data(buf);
                //转发所有客户端该用户登录成功，包括自己
                //std::cout<<msg.type()<<" "<<msg.name()<<" "<<msg.data()<<std::endl;
                broadcast(msg);
            }
            break;
        
        case (int)Msg_Type::CHAT:
            {
                std::unique_lock<std::mutex>lock(client_mutex);
                //转发消息给所有客户端，但不包括自己
                broadcast(msg,client_fd);
            }
            break;
        
        case (int)Msg_Type::QUIT:
            //删除客户端
            for(auto&[fd,cli]:clients){
                if(fd==client_fd){
                    clients.erase(fd);
                    break;
                }
            }
            close(client_fd);
            break;
        
        default:
            ERR_LOG("recv invalid msg");
            close(client_fd);
            return;
        }
    }
}

void server::broadcast(const Msg&msg, int exclude_fd){
    //序列化消息
    std::string output;
    msg.SerializePartialToString(&output);

    for(auto&[fd,cli]:clients){
        if(fd!=exclude_fd){
            if(send(fd,output.c_str(),output.size(),0)<0){
                ERR_LOG("send error");
                break;
            }
        }
    }
}