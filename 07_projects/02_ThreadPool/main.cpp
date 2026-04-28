#include"threadpool.h"
#include<chrono>
#include<iostream>
#include<cstdio>
//#include <syncstream>

int main(){
    //创建一个线程池，拥有三个工作线程
    threadpool mypool(3);
    //多线程执行多个任务
    for(int i=1;i<=20;i++){
        //lambda表达式作为任务
        //i为外部参数
        mypool.addTask([i]{
            //任务内容

            // //1.给cout加锁
            // static std::mutex cout_mutex;
            // {
            //     std::unique_lock<std::mutex>lock(cout_mutex);
            //     std::cout<<"this is the "<<i<<" task,the thread id is "<<std::this_thread::get_id()<<std::endl;
            // }
            
            //2.使用单个printf
            printf("this is the %d task,the thread id is %lu\n",i,std::this_thread::get_id());

            // //3.使用C++20
            // std::osyncstream(std::cout)<<"this is the "<<i<<" task,the thread id is "<<std::this_thread::get_id()<<std::endl;

            //模拟执行任务所需的时间
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        });
    }
    return 0;
}