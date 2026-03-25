#include "stdio.h"
#include<errno.h>
#include<string.h>

int main(){
    FILE*fp=NULL;
    fp=fopen("./file1.txt","r");
    if(fp==NULL){
        printf("fopen errno:%d,fopen errmsg:%s\n",errno,strerror(errno));
    }
    else {
        printf("fopen succeed by mode r\n");
        fclose(fp);
    }
    fp=fopen("./file2.txt","w");
    if(fp==NULL){
        printf("fopen errno:%d,fopen errmsg:%s",errno,strerror(errno));
    }
    else{
        printf("fopen succeed by mode w\n");
        fclose(fp);
    }
    return 0;
}