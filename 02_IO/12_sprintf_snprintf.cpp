#include<stdio.h>

int main(){
    // //sprintf,将多种不同的数据类型转换为字符串，存放到字符数组中
    // char buf[10];
    // sprintf(buf,"%s %d %lf","xuedu","18","1.80");
    // printf("%s",buf);

    //当字符数组的大小小于字符串的大小时会出现段错误，所以引入了snprintf
    char buf[10];
    //当字符串长度大于数组长度，则返回字符串长度，表示如果空间足够，完整的字符串应该有多长，而不是实际写入数组的长度
    int len=snprintf(buf,sizeof(buf),"%s %d %lf","xuedu",18,1.80);
    printf("%s\n",buf);
    printf("%d\n",len);
    return 0;
}