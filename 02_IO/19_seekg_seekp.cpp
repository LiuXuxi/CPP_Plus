#include<fstream>
#include<iostream>
using namespace std;

int main(){
    //双向输入输出流，可同时输入输出
    //打开文件并覆盖
    fstream fd("../stream.txt",ios::in|ios::out|ios::trunc);
    if(!fd.is_open()){
        cerr<<"open failed"<<endl;
        return -1;
    }

    //输出内容
    fd<<"hello world"<<endl;

    //定位输出光标到开头
    fd.seekp(0,ios::beg);

    //继续内容
    fd<<"hello xuedu"<<endl;

    //定位输入光标到结尾
    fd.seekg(0,ios::end);
    
    //读取文件大小
    cout<<"文件大小为："<<fd.tellg()<<endl;

    fd.close();
    return 0;
}