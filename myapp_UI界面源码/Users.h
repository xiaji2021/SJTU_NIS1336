//
// Created by 夏季 on 2023/7/3.
//

#ifndef NIS1336_USERS_H
#define NIS1336_USERS_H

#include "iostream"
#include "sstream"
#include "fstream"
#include "istream"
#include "string"
#include <map>
#include <functional>




class USER{
private:
    std::string password_hash;//不保存密码明文
    int UID;
    friend int userfile_read(const std::string &);
    friend int userfile_update(const std::string &);
    friend int regist(const std::string& name, const std::string& passwd);
    friend int user_check(const std::string& name, const std::string& passwd);
    friend
    void change_passwd(int uid_change,std::string curr_passwd,std::string new_passwd,std::string new_passwd_repeat);
    friend bool dele_user(int uid_dele);

public:
    std::string username;
    bool is_admin =1 ;
};


extern std::map<int ,USER> userinfo;

extern int UID_NUM ;
extern int UID_CURR ;

extern std::string uidFilename;

void admin_change(int admin_add_uid,int i);
void show_user();
void su_user(int su_uid);

int userfile_read(const std::string &);
int userfile_update(const std::string &);
int regist(const std::string& name, const std::string& passwd);
int user_check(const std::string& name, const std::string& passwd);

void change_passwd(int uid_change,std::string curr_passwd,std::string new_passwd,std::string new_passwd_repeat);
bool dele_user(int uid_dele);







//int userfile_read(const std::string &);



#endif //NIS1336_USERS_H
