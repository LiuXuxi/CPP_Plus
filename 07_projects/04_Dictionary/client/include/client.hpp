#include"Msg.pb.h"
#include<string>

class client
{
private:
    const int BUF_SIZE=128;
    int sockfd;
    std::string name;
    std::string pwd;
    bool is_logged;
    bool running;
private:
    void MenuView();
    void Register();
    void Login();
    void Querry();
    void History();
    void Logout();
    void gameView();

public:
    client(const std::string&ip,const int port);
    ~client();
    void run();
};

