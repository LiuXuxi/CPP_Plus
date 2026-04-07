#include<iostream>
#include<thread>
#include<chrono>
#include<condition_variable>
#include<mutex>

//创建锁，解决消费者互斥问题
std::mutex mux;

//创建条件变量，解决消费者生产者同步问题
std::condition_variable cv;

int main(){
    //创建一个生产者
    std::thread p([](){
        int num=5;
        while(num--){
            std::cout<<"produce an item"<<std::endl;
            cv.notify_one();
            //通知一个线程消费
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });


    //创建多个消费者
    for(int i=0;i<5;i++){
        std::thread c([](){
            std::unique_lock<std::mutex>Mux(mux);
            cv.wait(Mux);
            std::cout<<"cost an item"<<std::endl;
            Mux.unlock();
        });
        //分离线程
        c.detach();
    }

    //等待回收生产者线程
    p.join();

    std::cout<<"hello thread"<<std::endl;
    return 0;
}