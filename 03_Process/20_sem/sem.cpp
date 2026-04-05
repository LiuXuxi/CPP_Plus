#include"sem.h"
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include<stdio.h>
#include<errno.h>

//信号量结构体
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
    struct seminfo *__buf;
} arg;


static int init_sem(int semid,int semno){
    int val=-1;
    printf("please input the initial value of the %d sem:",semno);
    //输入初始化的值
    scanf("%d",&val);
    //吸收回车，避免影响其他程序
    getchar();

    //定义与设置信号量
    union semun us;
    us.val=val;

    if((semctl(semid,semno,SETVAL,us))==-1){
        perror("semctl error");
        return -1;
    }
    return 0;
}

int create_sem(int semcount){
    //1.创建信号量的Key值
    int key=-1;
    if((key=ftok("/",'k'))==-1){
        perror("ftok error");
        return -1;
    }

    //2.创建或打开信号量
    int semid=-1;
    if((semid=semget(key,semcount,IPC_CREAT|IPC_EXCL|0664))==-1){
        if(errno==EEXIST){
            semid=semget(key,semcount,IPC_CREAT|0644);
            return semid;
        }
        return -1;
    }

    //3.初始化信号量
    for(int i=0;i<semcount;i++)init_sem(semid,i);

    //4.返回信号量id
    return semid;
}

int P(int semid,int semno){
    //1.定义信号量操作结构体
    //@semno:信号量
    //@-1:表示申请一个资源
    //@0：表示阻塞申请
    struct sembuf buf;
    buf.sem_num=semno;
    buf.sem_op=-1;
    buf.sem_flg=0;
    
    //2.操作信号量
    if((semop(semid,&buf,1))==-1){
        perror("P error");
        return -1;
    }
    return 0;
}

int V(int semid,int semno){
    //1.定义信号量操作结构体
    //@semno:信号量
    //@-1:表示释放一个资源
    //@0：表示阻塞释放
    struct sembuf buf;
    buf.sem_num=semno;
    buf.sem_op=1;
    buf.sem_flg=0;
    
    //2.操作信号量
    if((semop(semid,&buf,1)==-1)){
        perror("V error");
        return -1;
    }
    return 0;
}

int delete_sem(int semid){
    if((semctl(semid,0,IPC_RMID))==-1){
        perror("delete error");
        return -1;
    }
    return 0;
}