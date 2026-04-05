#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

void cleanup(void*arg){
    printf("cleanup:%s\n",(char*)arg);
}

void* thr_fn1(void*argv){
    printf("thread 1 start\n");
    pthread_cleanup_push(cleanup,"thread 1 first handler");
    pthread_cleanup_push(cleanup,"thread 1 second handler");
    printf("thread 1 push complete\n");
    if(argv)return (void*)1;
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    //用return返回，不触发清理函数
    return (void*)1;
}

void* thr_fn2(void*argv){
    printf("thread 2 start\n");
    pthread_cleanup_push(cleanup,"thread 2 first handler");
    pthread_cleanup_push(cleanup,"thread 2 second handler");
    printf("thread 2 push complete\n");
    if(argv)pthread_exit((void*)2);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    //exit结束线程，触发清理函数
    pthread_exit((void*)2);
}

int main(){
    int err;
    pthread_t tid1,tid2;
    void*tret;

    //创建线程1
    err=pthread_create(&tid1,NULL,thr_fn1,(void*)1);
    //成功返回0，失败返回非0
    if(err!=0){
        printf("pthread create failed\n");
        return -1;
    }

    //创建线程2
    err=pthread_create(&tid2,NULL,thr_fn2,(void*)1);
    //成功返回0，失败返回非0
    if(err!=0){
        printf("pthread create failed\n");
        return -1;
    }

    //主线程等待线程1结束，回收资源
    err=pthread_join(tid1,&tret);
    //成功返回0，失败返回非0
    if(err!=0){
        printf("pthread join failed\n");
        return -1;
    }
    printf("thread1 exit code %ld\n",(long)tret);


    //主线程等待线程2结束，回收资源
    err=pthread_join(tid2,&tret);
    //成功返回0，失败返回非0
    if(err!=0){
        printf("pthread join failed\n");
        return -1;
    }
    printf("thread2 exit code %ld\n",(long)tret);

    return 0;
}