#include<iostream>
#include<mutex>
#include<thread>
#include<chrono>

//临界资源
int num=1000;

//创建互斥锁
std::mutex mux;

void task(){
    while(1){
        //上锁
        mux.lock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++num;
        std::cout<<"num is "<<num<<std::endl;
        //解锁
        mux.unlock();
    }
}

int main(){
    //创建多个线程
    for(int i=0;i<10;i++){
        std::thread th(task);

        //分离线程，不用主线程等待回收
        th.detach();
    }

    //主线程不回收分支线程，执行自己任务
    std::this_thread::sleep_for(std::chrono::seconds(20));
    std::cout<<"hello thread"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(20));
    return 0;
}