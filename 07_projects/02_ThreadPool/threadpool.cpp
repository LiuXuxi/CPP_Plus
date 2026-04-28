#include"threadpool.h"

threadpool::threadpool(const int ThreadSize):isRun(true)
{
    startThreadPool(ThreadSize);
}

threadpool::~threadpool()
{
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
}

void threadpool::addTask(std::function<void()>task){
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

void threadpool::startThreadPool(const int ThreadSize){
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