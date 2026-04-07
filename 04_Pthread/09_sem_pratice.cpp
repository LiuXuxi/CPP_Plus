#include<stdio.h>
#include <semaphore.h>
#include<pthread.h>
#include<unistd.h>

//创建无名信号量,分别解决同步问题
sem_t sems[3];

//创建参数结构体
struct Argv{
    //tid=1/2/3，用于区分信号量
    int tid;
    //任务参数
    char c;
};

void* task(void*_argv){
    //强制转换得到任务参数
    struct Argv* argv=(struct Argv*)_argv;

    //执行任务
    while(1){

        //如果sleep放在这，所有线程一起等待，一次性打印ABC
        //sleep(1);
        
        //申请资源
        sem_wait(&sems[argv->tid]);

        //如果sleep放在这，只有执行任务的线程挂起，一次一次打印A B C
        sleep(1);

        //执行任务体
        printf("%c",argv->c);
        //刷新缓冲区
        fflush(stdout);

        //释放资源
        sem_post(&sems[(argv->tid+1)%3]);
    }

    //线程退出
    pthread_exit(NULL);

}

int main(){
    pthread_t tid1,tid2,tid3;

    //创建并初始化线程的任务参数
    struct Argv argv1={0,'A'},argv2={1,'B'},argv3={2,'C'};

    //初始化信号量
    sem_init(&sems[0],0,1);
    sem_init(&sems[1],0,0);
    sem_init(&sems[2],0,0);

    //1.创建3个线程，线程开始执行任务
    if((pthread_create(&tid1,NULL,task,(void*)&argv1))==-1){
        printf("pthread1 create error...\n");
        return -1;
    }
    if((pthread_create(&tid2,NULL,task,(void*)&argv2))==-1){
        printf("pthread2 create error...\n");
        return -1;
    }
    if((pthread_create(&tid3,NULL,task,(void*)&argv3))==-1){
        printf("pthread2 create error...\n");
        return -1;
    }

    //2.主线程等待回收分支线程的资源
    if((pthread_join(tid1,NULL))==-1){
        printf("pthread join 1 error...\n");
        return -1;
    }
    if((pthread_join(tid2,NULL))==-1){
        printf("pthread join 2 error...\n");
        return -1;
    }
    if((pthread_join(tid3,NULL))==-1){
        printf("pthread join 3 error...\n");
        return -1;
    }

    //销毁信号量
    sem_destroy(&sems[0]);
    sem_destroy(&sems[1]);
    sem_destroy(&sems[2]);
    return 0;

}