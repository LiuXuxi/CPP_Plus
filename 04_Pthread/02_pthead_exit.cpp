#include<pthread.h>
#include<stdio.h>

void* thr_fn1(void*argv){
    printf("thread1 is returning...\n");
    return ((void*)1);
}

void* thr_fn2(void*argv){
    printf("thread2 is exiting...\n");
    pthread_exit((void*)2);
}

int main(){
    int err;
    pthread_t tid1,tid2;
    //线程退出时的状态属性
    void *tret;

    //创建线程1
    err=pthread_create(&tid1,NULL,thr_fn1,NULL);
    //成功返回0，失败返回非0
    if(err!=0){
        printf("pthread create failed\n");
        return -1;
    }

    //创建线程2
    err=pthread_create(&tid2,NULL,thr_fn2,NULL);
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