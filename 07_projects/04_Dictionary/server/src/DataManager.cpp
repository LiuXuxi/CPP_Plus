#include"DataManager.hpp"
#include<iostream>
#include<fstream>

DataManager::DataManager(const std::string& user, const std::string& pwd,const std::string& host, int port){
    // 1. 创建 Session
    sess_ = std::make_unique<mysqlx::Session>(host, port, user, pwd);

    //2.如果数据库不存在，创建数据库
    try {
        // 执行创建数据库的 SQL
        sess_->sql("CREATE DATABASE IF NOT EXISTS user_db CHARACTER SET utf8").execute();
        sess_->sql("create database if not exists dict_db character set utf8").execute();
        std::cout << "数据库 user_db 检查/创建完成。" << std::endl;
    } catch (const mysqlx::Error &err) {
        std::cerr << "创建数据库失败: " << err.what() << std::endl;
    }
    
    // 3. 获取不同的数据库 Schema
    user_schema_ = std::make_unique<mysqlx::Schema>(sess_->getSchema("user_db"));
    dict_schema_ = std::make_unique<mysqlx::Schema>(sess_->getSchema("dict_db"));

    //4.初始化数据库
    initalizeDatabase();
}

DataManager::~DataManager(){
    
}

// 初始化数据表
void DataManager::initalizeDatabase(){
    initalizeUsrDB();
    initalizeDictDB();
}

void DataManager::initalizeUsrDB(){
    try {
        // 1. 创建用户表 (usr)
        // 注意：MySQL中 PRIMARY KEY 建议使用 VARCHAR 并指定长度，TEXT 做主键限制较多
        sess_->sql("CREATE TABLE IF NOT EXISTS user_db.usr ("
                   "name VARCHAR(64) PRIMARY KEY, "
                   "passwd INT, "
                   "stage INT DEFAULT 0" // 0表示离线
                   ")").execute();

        // 2. 创建历史记录表 (history)
        sess_->sql("CREATE TABLE IF NOT EXISTS user_db.history ("
                   "name VARCHAR(64), "
                   "word VARCHAR(64), "
                   "mean TEXT, "
                   "time VARCHAR(32)"
                   ")").execute();

        std::cout << "表 usr 和 history 检查/创建成功" << std::endl;
    } catch (const mysqlx::Error &err) {
        std::cerr << "建表失败: " << err.what() << std::endl;
        throw; // 向上抛出，让服务器知道数据库初始化失败
    }
}

void DataManager::initalizeDictDB() {
    try {
        std::unique_lock<std::mutex>lock(dict_mtx_);
        // 1. 建表
        sess_->sql("CREATE TABLE IF NOT EXISTS dict_db.dict ("
                   "word VARCHAR(64) PRIMARY KEY, "
                   "mean TEXT"
                   ")").execute();
        Table dict=dict_schema_->getTable("dict");
        if(dict.count()==0){
            importDict("./dict.txt","dict");
        }
    } catch (const mysqlx::Error &e) {
        std::cerr << "initalize DictDB 失败: " << e.what() << std::endl;
        throw; 
    }
}

void DataManager::importDict(const std::string& filePath,const std::string& tableName) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "无法打开词典文件: " << filePath << std::endl;
        return;
    }
    std::cout<<"打开文件成功"<<std::endl;

    mysqlx::Table dict = dict_schema_->getTable(tableName);
    std::string line;
    int count = 0;

    try {
        // 1. 开启事务
        sess_->startTransaction();
        std::cout << "开始导入数据并开启事务..." << std::endl;

        // 建议使用批量插入对象来进一步提升速度
        auto inserter = dict.insert("word", "mean");

        while (std::getline(file, line)) {
            if (line.empty()) continue;

            auto pos = line.find(' ');
            if (pos == std::string::npos) continue;

            std::string word = line.substr(0, pos);
            std::string mean = line.substr(pos + 1);

            // 清洗 mean 开头的空格
            size_t first = mean.find_first_not_of(' ');
            if (first != std::string::npos) mean = mean.substr(first);

            if (word.empty() || mean.empty()) continue;

            // 将数据放入缓存队列
            inserter.values(word, mean);
            count++;

            // 每 2000 条执行一次 execute，但此时还没 commit 到磁盘
            if (count % 2000 == 0) {
                inserter.execute();
                inserter = dict.insert("word", "mean"); // 重置插入对象
            }
        }

        // 提交剩余的数据到服务器缓冲区
        if (count % 2000 != 0) {
            inserter.execute();
        }

        // 2. 提交事务（真正落盘）
        sess_->commit();
        std::cout << "导入完成，共计: " << count << " 条记录已成功提交。" << std::endl;

    } catch (const mysqlx::Error &err) {
        // 3. 发生错误时回滚事务，保证数据库不被污染
        sess_->rollback();
        std::cerr << "数据导入出错，已执行回滚。错误信息: " << err.what() << std::endl;
        return;
    }
}

// 用户相关的操作
bool DataManager::registerUser(const std::string &name, const std::string &password){

}

bool DataManager::loginUser(const std::string &name, const std::string &password, bool &is_online){

}
bool DataManager::logoutUser(const std::string &name){

}

// 单词表的操作
bool DataManager::querryWord(const std::string &word, std::string &meaning){

}

bool DataManager::recordHistory(const std::string &name, const std::string &word, const std::string &meaning, const std::string &time){

}

bool DataManager::getHistory(const std::string name, std::string &history){

}