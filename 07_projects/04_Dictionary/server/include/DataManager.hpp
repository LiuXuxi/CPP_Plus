#pragma once
#include<string>
#include<memory>
#include<mysql-cppconn/mysqlx/xdevapi.h>
#include<mutex>
using namespace mysqlx;

class DataManager
{
private:
    // 使用指针避开“无默认构造函数”的问题
    std::unique_ptr<mysqlx::Session> sess_;
    std::unique_ptr<mysqlx::Schema> user_schema_;
    std::unique_ptr<mysqlx::Schema> dict_schema_;

    // 细粒度锁：保护非线程安全的数据库对象
    std::mutex user_mtx_;
    std::mutex dict_mtx_;

    // 初始化数据表
    void initalizeDatabase();
    void initalizeUsrDB();
    void initalizeDictDB();
    void importDict(const std::string& filePath,const std::string& tableName);

public:
    DataManager(const std::string& user, const std::string& pwd,const std::string& host="localhost", int port=33060);
    ~DataManager();

    // 用户相关的操作
    bool registerUser(const std::string &name, const std::string &password);               // 用户注册操作
    bool loginUser(const std::string &name, const std::string &password, bool &is_online); // 用户登录操作
    bool logoutUser(const std::string &name);                                         // 用户退出操作

    // 单词表的操作
    bool querryWord(const std::string &word, std::string &meaning);                                                  // 查询单词
    bool recordHistory(const std::string &name, const std::string &word, const std::string &meaning, const std::string &time); // 历史记录
    bool getHistory(const std::string name, std::string &history);
};

