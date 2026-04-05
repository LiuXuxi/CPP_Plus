#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

struct foo{
    int a,b,c,d;
};

void printfoo(const char*s,const struct foo*fp){
    printf("%s",s);
    printf(" structure at 0x%lx\n",(unsigned long)fp);
    printf(" foo.a=%d\n",fp->a);
    printf(" foo.b=%d\n",fp->b);
    printf(" foo.c=%d\n",fp->c);
    printf(" foo.d=%d\n",fp->d);
}

void* thr_fn1(void* argv){
    struct foo Foo={1,2,3,4};
    printfoo("thead1:\n",&Foo);
    pthread_exit((void*)&Foo);
}

void* thr_fn2(void* argv){
    printf("thread2 :ID is %lu\n",(unsigned long)pthread_self);
    pthread_exit((void*)0);
}

int main(){
    int err;
    pthread_t tid1,tid2;
    //线程间传递的参数
    struct foo *fp;

    //创建线程1
    err=pthread_create(&tid1,NULL,thr_fn1,NULL);
    //成功返回0，失败返回非0
    if(err!=0){
        printf("pthread create failed\n");
        return -1;
    }

    //主线程等待线程1结束，回收资源
    err=pthread_join(tid1,(void**)&fp);
    //成功返回0，失败返回非0
    if(err!=0){
        printf("pthread join failed\n");
        return -1;
    }

    sleep(1);
    printf("parent starting second thread...\n");
    
    //创建线程2
    err=pthread_create(&tid2,NULL,thr_fn2,NULL);
    //成功返回0，失败返回非0
    if(err!=0){
        printf("pthread create failed\n");
        return -1;
    }

    //主线程等待线程1结束，回收资源
    err=pthread_join(tid1,NULL);
    //成功返回0，失败返回非0
    if(err!=0){
        printf("pthread join failed\n");
        return -1;
    }
    
    sleep(1);
    printfoo("parent:\n",fp);
    return 0;
}