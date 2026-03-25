#include <iostream>
#include <fstream>   // 文件流
#include <string>    // 字符串类
#include <vector>
#include <limits>    // 用于清理缓冲区

using namespace std;

// 使用常量定义文件路径，方便修改
const string DATA_FILE = "../userData.txt";

// 注册功能
int signUp() {
    // ios::app 表示追加模式 (Append)
    ofstream ofs(DATA_FILE, ios::app);
    
    if (!ofs.is_open()) {
        cerr << "错误：无法打开或创建数据库文件！" << endl;
        return -1;
    }

    string userID, pwd;
    cout << "请输入注册的账号：";
    cin >> userID;
    cout << "请输入注册的密码：";
    cin >> pwd;

    // 直接写入，中间用空格分隔
    ofs << userID << " " << pwd << endl;
    
    cout << "\n[系统] 账号 " << userID << " 注册成功！" << endl;
    
    ofs.close();
    return 0;
}

// 登录功能
int signIn() {
    // 默认是输入模式 (Input)
    ifstream ifs(DATA_FILE);
    
    if (!ifs.is_open()) {
        cout << "错误：数据库文件不存在，请先注册账号。" << endl;
        return -1;
    }

    string inputID, inputPWD;
    cout << "请输入登录的账号：";
    cin >> inputID;
    cout << "请输入登录的密码：";
    cin >> inputPWD;

    string fileID, filePWD;
    bool isFound = false;

    // 循环读取文件中的每一对账号密码
    while (ifs >> fileID >> filePWD) {
        if (inputID == fileID && inputPWD == filePWD) {
            isFound = true;
            break;
        }
    }

    if (isFound) {
        cout << "\n[系统] 登录成功！欢迎回来，" << inputID << "。" << endl;
    } else {
        cout << "\n[系统] 登录失败：账号或密码错误。" << endl;
    }

    ifs.close();
    return isFound ? 0 : -1;
}

// 辅助函数：按任意键继续
void pause() {
    cout << "\n按回车键返回菜单...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除缓冲区残余换行
    cin.get(); // 等待用户按下回车
}

void menu() {
    while (true) {
        system("clear"); // 如果需要每次刷新菜单，可以取消注释
        cout << "========= 欢迎系统 =========" << endl;
        cout << "1. 注册账号 (Sign Up)" << endl;
        cout << "2. 登录系统 (Sign In)" << endl;
        cout << "0. 退出程序" << endl;
        cout << "============================" << endl;
        cout << "请选择操作：";

        int opt;
        if (!(cin >> opt)) { // 防止用户输入非数字导致死循环
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "非法输入，请输入数字。" << endl;
            continue;
        }

        switch (opt) {
            case 1:
                signUp();
                pause();
                break;
            case 2:
                signIn();
                pause();
                break;
            case 0:
                cout << "谢谢使用，再见！" << endl;
                return;
            default:
                cout << "错误：没有这个功能，请重新选择。" << endl;
                pause();
        }
    }
}

int main() {
    menu();
    return 0;
}