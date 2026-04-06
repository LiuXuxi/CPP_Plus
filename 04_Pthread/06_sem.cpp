#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include <semaphore.h>

//定义一个信号量，解决同步问题，常用于生产者消费者问题
sem_t sem;

void* task1(void *argv){
    //总共要生产的数量
    int num=5;
    while(num--){
        //生产冷却时间
        sleep(1);
        //生产者生产物品
        printf("produce an item\n");
        //给信号量增加一个值，相当于V操作
        sem_post(&sem);
    }
    pthread_exit((void*)1);
}

void* task2(void *argv){
    //总共要消费的数量
    int num=5;
    while(num--){
        //消费冷却时间
        //sleep(1);
        //给信号量减少一个值，相当于P操作
        sem_wait(&sem);
        //消费者消费物品
        printf("cost an item\n");
    }
    pthread_exit((void*)2);
}

int main(){
    //定义线程号
    pthread_t tid1,tid2;

    //初始化信号量，第一个0表示线程间同步，第二个0表示初始为0
    sem_init(&sem,0,0);

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
    
    //关闭信号量
    sem_destroy(&sem);

    return 0;
}