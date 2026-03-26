#include<iostream>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string>
using namespace std;

int main(){
    int fd=-1;
    //打开图片文件
    if((fd=open("../Roxy.bmp",O_RDWR))==-1){
        perror("open error");
        return -1;
    }

    //查看图片大小
    //bmp文件前两个字节表示文件类型，3-6字节表示文件大小
    //可以通过把光标移到文件最后验证是否正确
    cout<<"文件大小为："<<lseek(fd,0,SEEK_END)<<endl;

    //先向右偏移两个字节，忽略文件类型
    lseek(fd,2,SEEK_SET);
    //在光标位置处读取个四字节，得到文件大小
    int size=0;
    read(fd,&size,4);
    cout<<"文件大小为："<<size<<endl;

    //移动光标到54字节，跳过文件头和信息头
    //54字节之后才是颜色字段
    //如果没跳过，会修改文件头和信息头，这很严重
    lseek(fd,54,SEEK_SET);

    //定义一像素的颜色，颜色规律是蓝绿红
    unsigned char color[3]{0,0,255};
    for(int i=0;i<500;i++){
        for(int j=0;j<500;j++){
            write(fd,color,sizeof(color));
        }
    }

    //关闭图片文件
    close(fd);


    return 0;
}