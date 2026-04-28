#include<msg.pb.h>
#include<string>

enum class Msg_Type{
    LOGIN=1,
    CHAT=2,
    QUIT=3
};

class client
{
private:
    const int BUF_SIZE=128;
    int sockfd;
    std::string name;
    bool running;

private:
    void recvMsg();
    void sendMsg(Msg_Type type,const std::string&data="");
    
public:
    client(const std::string&ip,const int port,const std::string& name);
    ~client();
    void run();
};

