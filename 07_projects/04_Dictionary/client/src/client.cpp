#include"client.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include"arpa/inet.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <iomanip>

// 定义颜色转义码
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define BLUE    "\033[34m"

#define ERR_LOG(msg)                                                      \
    do                                                                    \
    {                                                                     \
        perror(msg);                                                      \
        std::cout << __LINE__ << "  " << __func__ << "  " << __FILE__ << std::endl; \
    } while (0)

client::client(const std::string&ip,const int port)
{
    //1.创建套接字
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        ERR_LOG("socket error");
        close(sockfd);
        return;
    }
    
    //2.连接服务器端
    struct sockaddr_in sin;
    sin.sin_addr.s_addr=inet_addr(ip.c_str());
    sin.sin_family=AF_INET;
    sin.sin_port=htons(port);
    socklen_t len=sizeof(sin);
    if(connect(sockfd,(const sockaddr*)&sin,len)<0){
        ERR_LOG("connect error");
        close(sockfd);
        return;
    }
}

client::~client()
{
    Logout();
    if(sockfd>0)close(sockfd);
}

void client::run()
{
    int choice;
    while(1){
        MenuView();
        // 1. 尝试读取输入
        if (!(std::cin >> choice)) {
            // 2. 如果输入不是数字（读取失败）
            std::cout << "\033[31m输入无效！请输入数字。\033[0m" << std::endl;

            std::cin.clear(); // 清除错误状态标志位
            // 跳过缓冲区中所有非法字符，直到遇到换行符
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            system("pause"); // 让用户看清错误提示
            continue;
        }

        // 3. 输入是数字，开始逻辑判断
        switch (choice) {
            case 1:
            {
                Login();
                // 执行登录逻辑
                break;
            }
            case 2:
            {
                Register();
                // 执行注册逻辑
                break;
            }
            case 3:
            {
                Logout();
                return; // 退出程序
            }
            default:
            {
                // 数字超出了 1-3 的范围
                std::cout << "\033[31m无效的选择，请输入 1, 2 或 3。\033[0m" << std::endl;
                system("pause");
                break;
            }
        }
        std::cin.get();
        std::cin.clear();
    }
}

void client::MenuView() {
    // 清屏（Windows使用 "cls", Linux/Unix使用 "clear"）
    // 为了跨平台，这里打印一些空行或尝试系统调用
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << MAGENTA << BOLD;
    std::cout << "================================================" << std::endl;
    std::cout << "        __  __ ______ _   _ _    _          " << std::endl;
    std::cout << "       |  \\/  |  ____| \\ | | |  | |         " << std::endl;
    std::cout << "       | \\  / | |__  |  \\| | |  | |         " << std::endl;
    std::cout << "       | |\\/| |  __| | . ` | |  | |         " << std::endl;
    std::cout << "       | |  | | |____| |\\  | |__| |         " << std::endl;
    std::cout << "       |_|  |_|______|_| \\_|\\____/          " << std::endl;
    std::cout << "================================================" << RESET << std::endl;

    std::cout << std::endl;
    std::cout << CYAN << "      欢迎访问系统，请选择您要执行的操作：" << RESET << std::endl;
    std::cout << std::endl;

    // 绘制功能选项卡
    std::cout << YELLOW << "    ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓" << RESET << std::endl;
    std::cout << YELLOW << "    ┃" << GREEN << "  [1]  用户登录 (Login)               " << YELLOW << "┃" << RESET << std::endl;
    std::cout << YELLOW << "    ┃" << GREEN << "  [2]  新用户注册 (Register)           " << YELLOW << "┃" << RESET << std::endl;
    std::cout << YELLOW << "    ┃" << GREEN << "  [3]  退出系统 (Exit)               " << YELLOW << "┃" << RESET << std::endl;
    std::cout << YELLOW << "    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛" << RESET << std::endl;

    std::cout << std::endl;
    std::cout << BOLD << BLUE << "  请输入指令 (1-3) > " << RESET;
}

void client::Register(){
    //1.阻塞获取用户名和密码
    std::string name,pwd;
    std::cout<<"请输入用户名：";
    while(!(std::cin>>name));
    // 跳过缓冲区中所有非法字符，直到遇到换行符
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout<<"请输入密码:";
    while(!(std::cin>>pwd));
    // 跳过缓冲区中所有非法字符，直到遇到换行符
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //2.构建请求注册消息
    Msg msg;
    msg.set_name(name);
    msg.set_type(REGISTER);
    msg.set_data(pwd);

    //3.序列化消息
    std::string output;
    msg.SerializeToString(&output);

    //4.发送消息
    if(send(sockfd,output.c_str(),output.size(),0)<0){
        ERR_LOG("send error");
    }

    //5.接受消息，判断是否注册成功
    char buf[BUF_SIZE]="";
    int recv_len=0;
    recv_len=recv(sockfd,buf,BUF_SIZE,0);
    //表示客户端下线，删除客户端并结束工作线程
    if(recv_len<=0){
        std::cerr<<"recv error"<<std::endl;
        return;
    }

    //6.反序列化消息
    msg.ParseFromArray(buf,recv_len);
    std::string response=msg.data();
    if(response=="OK")std::cout<<"注册成功"<<std::endl;
    else if(response=="EXISTS")std::cout<<"用户已经注册过了"<<std::endl;
}

void client::Login(){
    //1.阻塞获取用户名和密码
    std::string name,pwd;
    std::cout<<"请输入用户名：";
    while(!(std::cin>>name));
    // 跳过缓冲区中所有非法字符，直到遇到换行符
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout<<"请输入密码:";
    while(!(std::cin>>pwd));
    // 跳过缓冲区中所有非法字符，直到遇到换行符
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //2.构建请求登录消息
    Msg msg;
    msg.set_name(name);
    msg.set_type(LOGIN);
    msg.set_data(pwd);

    //3.发送请求登录消息
    std::string output;
    msg.SerializeToString(&output);
    if(send(sockfd,output.c_str(),output.size(),0)<0){
        std::cerr<<"send error"<<std::endl;
        return;
    }

    //4.接受响应消息
    char buf[BUF_SIZE]="";
    int recv_len=0;
    recv_len=recv(sockfd,buf,BUF_SIZE,0);
    if(recv_len<=0){
        std::cerr<<"recv error"<<std::endl;
        return;
    }

    //5.反序列化消息
    msg.ParseFromArray(buf,recv_len);
    std::string response=msg.data();
    if(response == "OK"){
        this->name = name;
        is_logged = true;
        
        while (true) {
            // 1. 显示菜单
            gameView();

            int choice;
            
            // 2. 输入验证循环
            while (true) {
                std::cout << "请输入选项 (1-3): "; // 提示用户输入
                
                std::cin >> choice;

                // 检查输入流是否出错（例如用户输入了 'a' 而不是数字）
                if (std::cin.fail()) {
                    std::cin.clear(); // 清除错误标志
                    // 丢弃这一行输入的所有剩余字符（包括那个非法的字母和回车）
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "❌ 无效输入，请输入数字！" << std::endl;
                } else {
                    // 输入是数字，清空该行可能残留的字符（虽然对于int输入通常不需要，但为了保险）
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break; // 退出输入循环，执行下面的 switch
                }
            }

            // 3. 处理逻辑
            if (choice == 1) {
                Querry();
            } else if (choice == 2) {
                History();
            } else if (choice == 3) {
                Logout();
                break; // 退出登录循环
            } else {
                // 处理输入了数字但不在 1-3 范围内的情况
                std::cout << "❌ 选项不存在，请重新输入。" << std::endl;
            }
            std::cin.get();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    else if(response=="EXISTS"){
        std::cout<<"用户已在别处登录"<<std::endl;
    }else if(response=="FAIL"){
        std::cout<<"用户名和密码错误"<<std::endl;
    }
}

void client::Querry(){
    //1.阻塞获取需要查询的单词
    std::string word;
    std::cout<<"请输入查询的单词：";
    while(!(std::cin>>word));
    // 跳过缓冲区中所有非法字符，直到遇到换行符
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    //2.构建请求查询消息
    Msg msg;
    msg.set_name(this->name);
    msg.set_type(QUERY);
    msg.set_data(word);

    //3.发送请求查询消息
    std::string output;
    msg.SerializeToString(&output);
    if(send(sockfd,output.c_str(),output.size(),0)<0){
        std::cerr<<"send error"<<std::endl;
        return;
    }

    //4.接受响应消息
    char buf[BUF_SIZE]="";
    int recv_len=0;
    recv_len=recv(sockfd,buf,BUF_SIZE,0);
    if(recv_len<=0){
        std::cerr<<"recv error"<<std::endl;
        return;
    }

    //5.反序列化消息
    msg.ParseFromArray(buf,recv_len);
    std::string mean=msg.data();
    std::cout<<mean<<std::endl;
}

void client::History(){
    //2.构建请求查询消息
    Msg msg;
    msg.set_name(name);
    msg.set_type(HISTORY);

    //3.发送请求历史消息
    std::string output;
    msg.SerializeToString(&output);
    if(send(sockfd,output.c_str(),output.size(),0)<0){
        std::cerr<<"send error"<<std::endl;
        return;
    }

    //4.接受历史消息
    char buf[BUF_SIZE]="";
    int recv_len=0;
    recv_len=recv(sockfd,buf,BUF_SIZE,0);
    if(recv_len<=0){
        std::cerr<<"recv error"<<std::endl;
        return;
    }

    //5.反序列化消息
    msg.ParseFromArray(buf,recv_len);
    std::string history=msg.data();
    std::cout<<history<<std::endl;
}

void client::gameView(){
        // 清屏
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    // 顶部状态栏
    std::cout << CYAN << BOLD;
    std::cout << std::string(50, '-') << std::endl; // 分隔线
    std::cout << std::endl;

    // 中心标题装饰
    std::cout << MAGENTA << BOLD;
    std::cout << "          ==================================" << std::endl;
    std::cout << "          |   WELCOME TO USER DASHBOARD    |" << std::endl;
    std::cout << "          ==================================" << RESET << std::endl;

    std::cout << std::endl;
    std::cout << CYAN << "      您已进入个人工作台，请选择功能：" << RESET << std::endl;
    std::cout << std::endl;

    // 绘制功能选项卡 - 增加了简单的 ASCII 图标以增加区分度
    std::cout << YELLOW << "    ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓" << RESET << std::endl;
    std::cout << YELLOW << "    ┃" << GREEN << "  [1]  🔍 查询单词 (Search)            " << YELLOW << "┃" << RESET << std::endl;
    std::cout << YELLOW << "    ┃" << GREEN << "  [2]  📜 查看历史 (History)           " << YELLOW << "┃" << RESET << std::endl;
    std::cout << YELLOW << "    ┃" << GREEN << "  [3]  🚪 返回上一级 (Back)            " << YELLOW << "┃" << RESET << std::endl;
    std::cout << YELLOW << "    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛" << RESET << std::endl;

    std::cout << std::endl;
    std::cout << BOLD << BLUE << "  请输入指令 (1-3) > " << RESET;
}

void client::Logout(){
    //1.构建信息
    Msg msg;
    msg.set_name(name);
    msg.set_type(QUIT);

    //2.序列化消息
    std::string output;
    msg.SerializeToString(&output);

    //3.发送消息
    if(send(sockfd,output.c_str(),output.size(),0)<0){
        ERR_LOG("send error");
        return;
    }
}