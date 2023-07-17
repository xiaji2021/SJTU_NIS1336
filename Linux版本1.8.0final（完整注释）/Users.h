//
// Created by 夏季 on 2023/7/3.
//

#ifndef NIS1336_USERS_H
#define NIS1336_USERS_H

#include "iostream"
#include <map>
#include "sstream"
#include "fstream"
#include "istream"
#include "string"
#include <functional>

class TaskManager;  // 前向声明

extern TaskManager task_manager;

class USER{
private:

    std::string password_hash;//不保存密码明文
    int UID;

    //用于访问private的友元函数
    friend int userfile_read(const std::string &);
    friend int userfile_update(const std::string &);
    friend int regist(const std::string& name, const std::string& passwd);
    friend int user_check(const std::string& name, const std::string& passwd);
    friend void change_passwd(int uid_change);
    friend bool dele_user(int uid_dele);

public:
    std::string username;
    bool is_admin;
};


extern std::map<int ,USER> userinfo;
extern int UID_NUM ;
extern int UID_CURR ;
extern std::string uidFilename;

void admin_change(int admin_add_uid,int i);
void show_user();
void su_user(int su_uid,TaskManager& task_manager);


#endif //NIS1336_USERS_H
