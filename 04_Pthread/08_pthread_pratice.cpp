#include<stdio.h>
#include<pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

struct File{
    char* scr;
    char*dest;
    int start;
    int len;
};

int get_file_len(const char*src,const char*dest){
    int sfd,dfd;
    //打开源文件，得到长度
    if((sfd=open(src,O_RDONLY))==-1){
        perror("open scr error");
        return -1;
    }
    //创建或打开目标文件
    if((dfd=open(dest,O_RDWR|O_CREAT|O_TRUNC,0644))==-1){
        perror("open dest error");
        return -1;
    }

    //得到长度
    int len=lseek(sfd,0,SEEK_END);
    
    //关闭文件
    close(sfd);
    close(dfd);
    return len;
}

void* task(void*argv){
    //1.将参数进行强制转换
    struct File*file=(struct File*)argv;

    //2.打开文件
    int sfd,dfd;
    //打开源文件，得到长度
    if((sfd=open(file->scr,O_RDONLY))==-1){
        perror("open scr error");
        return (void*)-1;
    }
    //创建或打开目标文件
    if((dfd=open(file->dest,O_RDWR))==-1){
        perror("open dest error");
        return (void*)-1;
    }

    //3.偏移光标
    lseek(sfd,file->start,SEEK_SET);
    lseek(dfd,file->start,SEEK_SET);
    
    //4.开始执行拷贝任务
    //记录每次读取的数据
    int ret=0;
    //记录拷贝的总个数
    int count=0;
    //拷贝数据的容器
    char buf[128]="";
    while(1){
        ret=read(sfd,buf,sizeof(buf));
        count+=ret;
        //当最后一次拷贝超出长度时需要进行截取
        if(count>=file->len){
            write(dfd,buf,ret-(count-file->len));
            break;
        }
        //未超出长度时正常拷贝
        write(dfd,buf,ret);
    }

    //5.关闭文件
    close(sfd);
    close(dfd);

    //6.线程退出
    pthread_exit(NULL);
}

int main(){
    pthread_t tid1,tid2;

    char* src="../test1.txt";
    char*dest="../test2.txt";

    //得到文件大小
    int total=get_file_len(src,dest);

    //创建并初始化任务参数
    struct File file1,file2;
    file1.scr=file2.scr=src;
    file1.dest=file2.dest=dest;
    file1.start=0,file1.len=total/2;
    file2.start=total/2,file2.len=total-total/2;

    //1.创建两个线程，线程开始执行任务
    if((pthread_create(&tid1,NULL,task,(void*)&file1))==-1){
        printf("pthread1 create error...\n");
        return -1;
    }
    if((pthread_create(&tid2,NULL,task,(void*)&file2))==-1){
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

    printf("finish copy...\n");
    return 0;

}