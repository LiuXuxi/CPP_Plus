#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include <semaphore.h>

//条件信号量，用于解决一个生产者和多个消费者的同步问题
pthread_cond_t cond;

//互斥锁，用于多个消费者的互斥问题
pthread_mutex_t mutex;

void* task1(void *argv){
    //生产总数
    int num=3;
    while(num--){
        //生产冷却
        sleep(1);

        //生产过程
        printf("produce an item...\n");
        //唤醒等待队列的第一个等待线程
        pthread_cond_signal(&cond);

    }
    pthread_exit((void*)1);
}

void* task2(void *argv){
    //给消费者上锁，防止所有消费者同时抢占资源
    pthread_mutex_lock(&mutex);
    //等待消费者线程唤醒
    pthread_cond_wait(&cond,&mutex);
    printf("%x:cost an item...\n",pthread_self());
    pthread_mutex_unlock(&mutex);
    pthread_exit((void*)0);
}

int main(){
    //定义线程号
    pthread_t tid1,tid2,tid3,tid4;

    //初始化互斥锁
    pthread_mutex_init(&mutex,NULL);
    //初始化条件信号量
    pthread_cond_init(&cond,NULL);

    //创建分支线程1
    if((pthread_create(&tid1,NULL,task1,NULL))!=0){
        printf("pthread1 create failed\n");
        return -1;
    }

    //创建分支线程2
    if((pthread_create(&tid2,NULL,task2,NULL))!=0){
        printf("pthread2 create failed\n");
        return -1;
    }

    //创建分支线程3
    if((pthread_create(&tid3,NULL,task2,NULL))!=0){
        printf("pthread3 create failed\n");
        return -1;
    }

    //创建分支线程4
    if((pthread_create(&tid4,NULL,task2,NULL))!=0){
        printf("pthread4 create failed\n");
        return -1;
    }

    //主线程等待分支线程返回
    void*tret;
    if((pthread_join(tid1,&tret))!=0){
        printf("pthread1 join failed\n");
        return -1;
    }
    printf("pthread 1 exit %lu\n",(unsigned long)tret);
    if((pthread_join(tid2,&tret))!=0){
        printf("pthread2 join failed\n");
        return -1;
    }
    printf("pthread 2 exit %lu\n",(unsigned long)tret);
    if((pthread_join(tid3,&tret))!=0){
        printf("pthread3 join failed\n");
        return -1;
    }
    printf("pthread3  exit %lu\n",(unsigned long)tret);
    if((pthread_join(tid4,&tret))!=0){
        printf("pthread4 join failed\n");
        return -1;
    }
    printf("pthread 4 exit %lu\n",(unsigned long)tret);

    //销毁互斥锁
    pthread_mutex_destroy(&mutex);
    //销毁条件信号量
    pthread_cond_destroy(&cond);

    return 0;
}