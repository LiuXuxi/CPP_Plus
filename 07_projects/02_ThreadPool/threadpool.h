#pragma once
#include<functional>
#include<vector>
#include<thread>
#include<queue>
#include<mutex>
#include<condition_variable>

class threadpool
{
private:
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
    
public:
    //@ThreadSize:线程池中的线程数量
    //功能：初始化指定数量的线程数量的线程池
    threadpool(const int ThreadSize);
    //关闭线程池，回收所有线程
    ~threadpool();
    //@task:指定的任务
    //添加任务到线程池的工作队列
    void addTask(std::function<void()>task);
};

