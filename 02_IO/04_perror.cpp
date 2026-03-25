#include "stdio.h"

int main(){
    FILE*fp=NULL;
    fp=fopen("./file1.txt","r");
    if(fp==NULL){
        perror("fopen failed:");
    }
    else {
        printf("fopen succeed by mode r\n");
        fclose(fp);
    }
    fp=fopen("./file2.txt","w");
    if(fp==NULL){
        perror("fopen failed:");
    }
    else{
        printf("fopen succeed by mode w\n");
        fclose(fp);
    }
    return 0;
}