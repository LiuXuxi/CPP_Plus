#include <iostream>
#include <math.h>           // 数学相关函数
#include <pthread.h>        // 线程支持库
#include <unistd.h>         // sleep 函数

using namespace std;

// 定义分支线程的线程体函数
void* task(void* arg) {
    while (1) {
        cout << "hello world" << endl;
        sleep(1);           // 休眠 1 秒
    }
    return NULL;            // 虽然不会执行到这里，但规范上需要返回
}

int main(int argc, const char* argv[]) {
    pthread_t tid;          // 定义线程号变量
    // 创建线程
    if (pthread_create(&tid, NULL, task, NULL) != 0) {
        cout << "pthread_create error" << endl;
        return 1;           // 创建失败，提前退出
    }
    // 主线程保持运行（防止主线程退出导致整个进程结束）
    while (1);
    return 0;
}