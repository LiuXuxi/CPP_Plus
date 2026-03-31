#include <sys/types.h>
#include <sys/stat.h>
#include<stdio.h>

int main(){
    if(mkfifo("./fifo",0644)==-1){
        printf("mkfifo failed");
        return -1;
    }
    printf("fifo create successfully\n");
    return 0;
}