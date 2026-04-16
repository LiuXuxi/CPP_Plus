#include"http.h"
#include<iostream>
#include <unistd.h>
#include <sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#define SER_IP "192.168.31.129"
#define BUF_SIZE 1024

//@port:服务器端口
//功能：初始化服务器
int init_server(int port){
    //1.创建套接字
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1){
        perror("socket error");
        return -1;
    }
    std::cout<<"sockfd is "<<sockfd<<std::endl;

    //2.设置端口复用
    int res=1;
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&res,sizeof(res))==-1){
        perror("setsockopt error");
        return -1;
    }

    //3.绑定服务器IP和端口
    struct sockaddr_in sin;
    sin.sin_addr.s_addr=inet_addr(SER_IP);
    sin.sin_family=AF_INET;
    sin.sin_port=htons(port);
    socklen_t len=sizeof(sin);
    if(bind(sockfd,(const sockaddr*)&sin,len)==-1){
        perror("bind error");
        return -1;
    }
    std::cout<<"bind successful"<<std::endl;

    //4.将服务器处于监听状态
    if(listen(sockfd,128)==-1){
        perror("listen error");
        return -1;
    }
    std::cout<<"listen successful"<<std::endl;

    //5.返回套接字
    return sockfd;
}

//@fd:客户端文件描述符
//@buf:请求报文
//@method:返回的请求方法
//@url:返回的统一资源描述符
//@version:返回的http版本号
//功能；从请求报文buf中解析请求头为method、url、version并返回
static int GetRequest(FILE*fp,char*buf,char*method,char*url,char*version){
    //1.获取请求行
    //用于存储请求行
    char request_head[BUF_SIZE];
    //从缓冲区得到请求行
    fgets(request_head,BUF_SIZE,fp);

    //2.解析请求行
    int num=sscanf(buf,"%s %s %s",method,url,version);
    if(num!=3){
        perror("sscanf perror");
        return -1;
    }
    return 0;
}

//@url:统一资源标识符
//@len:url的长度
//return:返回可带参数
//功能：从url中返回可带参数，若存在参数则向后截断只保留前面的请求资源路径并返回参数
//若不存在参数，则url不变并返回NULL
static char*getQuerryFromURL(char*url,int len){
    char*querry=NULL;
    for(int i=0;i<len;i++){
        //说明有参数，此时返回参数并截断url
        if(url[i]=='?'){
            querry=&url[i];
            //参数是？后面的字符串
            ++querry;
            //截断url只剩前面的资源路径
            url[i]='\0';
            break;
        }
    }
    return querry;
}

static void show_404(int fd){
    //组装首部信息
    //打开一个html文档并获取大小
    int file=open("../wwwroot/404.html",O_RDONLY);
    if(file==-1){
        perror("open file 404 error");
        return;
    }
    struct stat st;
    if(fstat(file,&st)==-1){
        perror("fstat 404 error");
        close(file);
        return;
    }

    //发送响应头（包含Content-Length）
    char header[256];
    int hdr_len = snprintf(header, sizeof(header),
        "HTTP/1.1 404 Not Found\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n",
        (size_t)st.st_size);
    send(fd, header, hdr_len, 0);

    //发送文件内容
    off_t offset = 0;
    ssize_t sent = 0;
    size_t to_send = (size_t)st.st_size;
    while(to_send > 0){
        sent = sendfile(fd, file, &offset, to_send);
        if(sent <= 0) break;
        to_send -= sent;
    }
    close(file);
}

//@fd:客户端文件描述符
//@err:错误码
static void echo_error(int fd,int err){
    //对错误码进行多分枝选择
    switch(err)
    {
        case 403:
            break;
        case 404:
            show_404(fd);     //调用展示404界面
            break;
        case 405:
            break;
        case 500:
            break;
        default:
            break; 
    }
}

//@fd:客户端文件描述符
//@methon:返回的请求方法
//@path:请求资源路径
//@querry:客户端传入的参数
static void handle_request(int fd,char* method,char* path,char* querry){
    
}
    

//@fd:客户端文件描述符
//@path:请求资源路径
//@s:路径长度
//功能：响应普通页面的函数
static int echo_www(int fd,const char*path,size_t s){
    //以只读形式打开资源
    int file=open(path,O_RDONLY);
    if(file==-1){
        perror("open file error");
        return -1;
    }

    //组装响应头
    char header[256];
    int hdr_len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n",
        s);
    if(send(fd, header, hdr_len, 0) == -1){
        close(file);
        return -1;
    }

    off_t offset = 0;
    ssize_t sent = 0;
    size_t to_send = s;
    while(to_send > 0){
        sent = sendfile(fd, file, &offset, to_send);
        if(sent <= 0) break;
        to_send -= sent;
    }
    if(to_send != 0){
        //返回错误界面
        echo_error(fd,500);
        close(file);
        return -1;
    }
    close(file);
    return 0;
}

int cli_handler(int fd){
    //1.创建接受客户端信息的缓冲区
    char rbuf[BUF_SIZE];

    //2.接受客户端消息
    if((recv(fd,rbuf,sizeof(rbuf),0))==-1){
        perror("recv error");
        return -1;
    }

#if 1
    //查看客户端消息
    std::cout<<"======================"<<std::endl;
    std::cout<<rbuf<<std::endl;
    std::cout<<"======================"<<std::endl;
#endif

    //3.解析http请求

    //将内核缓冲区的内容放到用户态缓冲区中
    FILE*fp=fdopen(fd,"r");

    char method[20];        //方法
    char url[BUF_SIZE];     //统一资源标记符号
    int len=0;              //url的长度
    char version[20];       //http版本号
    char*querry=NULL;        //url中的可带参数
    GetRequest(fp,rbuf,method,url,version);
    len=strlen(url);
    //在url中得到参数，并截断url
    querry=getQuerryFromURL(url,len);

    //4.响应http请求
    //4.1判断请求方法是GET请求还是POST请求
    //如果都不是，则method解析错误
    if(strcasecmp(method, "GET")!=0 && strcasecmp(method,"POST")!=0)
    {
        //说明该请求方法既不是GET请求也不是POST请求
        printf("method error\n");
        echo_error(fd, 405);      //向客户端回复一个错误页面
        close(fd);           //关闭套接字
        return -1;
    }
    //标记是否要进行手动处理界面（动态处理还是静态处理），1为真，0为假
    //默认静态处理
    int need_handle=0;
    //如果是POST就是动态处理
    if(strcasecmp(method,"POST")==0)need_handle=1;
    //如果是GET并且带了参数也是动态处理
    if(strcasecmp(method,"GET")==0&&querry!=NULL)need_handle=1;

    //4.2固定(确认）请求资源路径
    char path[BUF_SIZE]="";
    sprintf(path,"../wwwroot%s",url);
    //如果请求的没有的地址没有任何携带资源，那么默认返回index.html
    if(path[strlen(path)-1] == '/')
    {
        strcat(path, "index.html");
    }

    //4.3判断当前服务器是否有该path
    //定义文件状态类型的结构体
    struct stat st;
    //如果指定的文件存在，则会把该文件的信息放入st结构体中
    //如果不存在，函数返回-1
    if((stat(path,&st))==-1){
        //说明文件路径资源不存在
        //向客户端返回一个错误界面
        echo_error(fd,404);
        close(fd);
        return -1;
    }

    //4.4开始构建响应报文
    //动态处理，需要执行子程序返回结果
    if(need_handle){
        handle_request(fd,method,path,querry);
    }
    //静态处理
    else{
        //返回默认请求的界面
        echo_www(fd,path,st.st_size);
    }

    //5.结束
    fclose(fp);

    return 0;
}
