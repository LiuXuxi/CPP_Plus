#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

int num=1000;

//定义一个互斥锁，解决互斥问题
pthread_mutex_t mutex;

void* task1(void *argv){
    while(1){
        sleep(1);

        //如果不适用锁，会偶然地发生两种错误
        //1.num打印相同的值：1000->1000
        //2.num打印差距为2：1000->1002
        //这都是由于线程互斥问题引起的，时间片分配随机

        
        //给操作临界区的代码上锁，变为原子操作
        pthread_mutex_lock(&mutex);
        ++num;
        //释放锁
        pthread_mutex_unlock(&mutex);

        printf("num is %d\n",num);
    }
    pthread_exit((void*)1);
}

void* task2(void *argv){
    while(1){
        sleep(1);

        //给操作临界区的代码上锁，变为原子操作
        pthread_mutex_lock(&mutex);
        ++num;
        //释放锁
        pthread_mutex_unlock(&mutex);

        printf("num is %d\n",num);
    }
    pthread_exit((void*)2);
}

int main(){
    //按照默认属性初始化锁
    pthread_mutex_init(&mutex,NULL);

    //定义线程号
    pthread_t tid1,tid2;
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
    
    //销毁锁，锁变为未初始化状态，可以重新被初始化
    pthread_mutex_destroy(&mutex);

    return 0;
}