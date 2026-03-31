#include <sys/types.h>
#include <sys/stat.h>
#include<stdio.h>

int main(){
    if(mkfifo("./fifo1",0644)==-1){
        printf("mkfifo failed");
        return -1;
    }
    printf("fifo1 create successfully\n");

    if(mkfifo("./fifo2",0644)==-1){
        printf("mkfifo failed");
        return -1;
    }
    printf("fifo2 create successfully\n");

    return 0;
}