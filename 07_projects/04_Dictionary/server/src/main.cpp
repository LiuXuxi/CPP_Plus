#include<iostream>
#include<string.h>
#include"DataManager.hpp"

int main(int argc, const char* argv[]) {
    // if (argc < 5) {
    //     std::cerr << "Usage: " << argv[0] << " <ip> <port> <user> <pwd>" << std::endl;
    //     return -1;
    // }

    // std::string ip = argv[1];
    // int port = std::stoi(argv[2]); // 建议用 stoi 替代 atoi，更安全

    try {
        // 传入所有解析到的参数
        DataManager manager("root","root");
        
        std::cout << "服务器初始化成功，准备运行..." << std::endl;
        
        // 如果这里没有任何阻塞逻辑（比如 while 循环），程序会立刻结束并析构 manager
    } catch (const std::exception& e) {
        std::cerr << "异常退出: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}