#include<iostream>
#include<fstream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include<vector>
#include<string>
using namespace std;

int main(){
    string src{"../File.txt"};
    string dest{"../File2.txt"};

    //打开需要读取的文件
    ifstream ifd(src,ios::in|ios::binary);
    if(!ifd.is_open()){
        cerr<<"open failed"<<endl;
        return -1;
    }

    //移动读光标的位置
    ifd.seekg(0,ios::end);

    //读取文件大小
    long long size=ifd.tellg();

    cout<<"scr size is "<<size<<endl;

    ifd.close();

    //创建子进程，两个进程准备一起拷贝文件
    pid_t pid=fork();
    if(pid==-1){
        cerr<<"fork faile"<<endl;
        return -1;
    }

    /*
    为什么要在条件内容再重新打开文件？
    防止光标冲突，光标位置和文件描述符绑定，也就是跟流对象绑定
    如果使用if之前的
    */

    //子进程拷贝后一半内容
    else if(pid==0){
        //用in防止清空文件，out会自带trunc
        ofstream fd1(dest,ios::in|ios::out|ios::binary);
        ifstream fd2(src,ios::in|ios::binary);

        vector<char>s(size/2+1);     //用来接受从被拷贝文件读取的内容

        //把光标移到中间位置的一半+1
        fd2.seekg(size/2,ios::beg);
        //读取后半内容
        fd2.read(s.data(),size/2);
        //移动写光标
        fd1.seekp(size/2+1,ios::beg);
        //拷贝后半内容
        fd1.write(s.data(),s.size());
        cout<<"拷贝后半内容完成"<<endl;
    }
    //父进程拷贝前一半
    else{
        //父进程先清空文件
        ofstream fd1(dest,ios::trunc|ios::out|ios::binary);
        ifstream fd2(src,ios::in|ios::binary);

        vector<char>s(size/2+1);     //用来接受从被拷贝文件读取的内容

        //把光标移到中间位置的一半+1
        fd2.seekg(0,ios::beg);
        //读取后半内容
        fd2.read(s.data(),size/2);
        //移动写光标
        fd1.seekp(0,ios::beg);
        //拷贝后半内容
        fd1.write(s.data(),s.size());
        cout<<"拷贝前半内容完成"<<endl;
        wait(NULL);
        cout<<"拷贝完成"<<endl;
        
        //验证拷贝大小是否正确
        fd2.seekg(0,ios::end);
        cout<<"dest size is "<<fd2.tellg()<<endl;

    }
    return 0;
}