//
// Created by 夏季 on 2023/7/3.
//

#include "Users.h"
#include "task_manager.h"
#include <QString>
//#include </opt/homebrew/Cellar/openssl@3/3.1.0/include/openssl/sha.h>




//#include <bcrypt.h>
//#include <bcrypt/BCrypt.hpp>



std::string STL_hash(std::string str){
    std::hash<std::string> hasher;
    size_t hashValue = hasher(str);
    std::stringstream ss;
    ss << hashValue;
    std::string hash_str = ss.str();
    return hash_str;
}




std::map<int ,USER> userinfo;

int UID_NUM = 0;
int UID_CURR = -1;

std::string uidFilename = "uid.txt";


int userfile_read(const std::string &uid_filename) {
    std::ifstream *file =new std::ifstream(uid_filename);
    if(file->fail()) {
        std::ofstream new_file(uid_filename);
        if(new_file.is_open()) new_file.close();
        delete file;
        file = new std::ifstream(uid_filename);
        if(file->fail())  {
            return -1;
        }
    }
        std::string line;
        while(std::getline(*file, line)){//line格式：UID username passwd_hash
            std::istringstream iss(line);
            USER user_tmp;int uid_tmp;
            iss>>uid_tmp;iss>>user_tmp.username;iss>>user_tmp.password_hash;iss>>user_tmp.is_admin;
            userinfo[uid_tmp] = user_tmp;
            //std::cout<<uid_tmp<<user_tmp.username<<user_tmp.password_hash<<std::endl;

    }
    return userinfo.size();//返回容器中元素的个数
}

int userfile_update(const std::string &uid_filename){
    std::ofstream file(uid_filename);   //写入需要使用ofstream

    for(const auto& pair : userinfo){
         file << pair.first << " ";
         file << pair.second.username <<" ";
         file << pair.second.password_hash<<" ";
         file << pair.second.is_admin;
         file << "\n";
    }
    return userinfo.size();
}



int regist(const std::string& name, const std::string& passwd) {
    int new_uid = 0;
    if(userfile_read(uidFilename)==0)  {
        new_uid = 0;
    }
    else {
        for (const auto &pair: userinfo) {
            if (pair.second.username == name) {
                return -1;//失败
            }
            //确认UID的开始位置

            auto key_max = userinfo.rbegin();
            new_uid = key_max->first + 1;
        }
    }
    UID_NUM++;

    std::string userfilename="user"+std::to_string(new_uid)+".txt";
	std::ofstream userfile(userfilename);
	if (!userfile.is_open()){
		printf("create userfile failure\n");
		return -1;
	}

        std::string hash_tmp = STL_hash(passwd);
        USER user_tmp;
        user_tmp.password_hash = hash_tmp;
        user_tmp.username = name;
        if(new_uid == 0) user_tmp.is_admin = 1;
        else user_tmp.is_admin = 0;
        userinfo[new_uid] = user_tmp;
        userfile_update(uidFilename);


        extern int uid;
        taskfilename="user"+QString::number(uid).toStdString()+".txt";
        std::ofstream fout(userfilename);
        fout<<"0\n";
        fout.close();
        return new_uid;




}

int user_check(const std::string& name, const std::string& passwd) {
    userfile_read(uidFilename);
    std::string hash_tmp = STL_hash(passwd);

    int Match = 0;
    for (const auto &pair: userinfo) {
        if (pair.second.username == name) {Match++;
            if (pair.second.password_hash ==hash_tmp) {
                UID_CURR = pair.first;
                return pair.first;
            }

            else {
                return -1;
            }
        }
    }
    if(Match == 0) {
        return -1;
    }

}

void change_passwd(int uid_change,std::string curr_passwd,std::string new_passwd,std::string new_passwd_repeat)
{
        if(new_passwd == new_passwd_repeat){
            userinfo[uid_change].password_hash =STL_hash(new_passwd);
            userfile_update(uidFilename);
            return ;
        }
        return;

}



bool dele_user(int uid_dele){
    userfile_read(uidFilename);
    extern int uid;
    UID_CURR = uid;
    if(userinfo.count(uid_dele) == 0) {
        return 0;
    }
    if(userinfo[UID_CURR].is_admin==0) {
        return 0;
    }
    //root可以删除非自己的管理员，管理员可以删除普通用户
    else if(UID_CURR==uid_dele){
        return 0;
    }
    else if(UID_CURR!=0&&userinfo[uid_dele].is_admin==1){

        return 0;
    }
    else {
        userinfo.erase(uid_dele);
        std::string userfilename="user"+std::to_string(uid_dele)+".txt";
        std::remove(userfilename.c_str());
        userfile_update(uidFilename);
        UID_NUM--;
        
        return 1;
    }

}


void show_user(){
    userfile_read(uidFilename);
    for(const auto& pair:userinfo){
        if(pair.first == UID_CURR) std::cout<<"*";else std::cout<<" ";
        if(pair.second.is_admin == 1) std::cout<<"$";
        else std::cout<<" ";
        std::cout<<pair.first<<" "<< pair.second.username<<std::endl;
    }

}

void admin_change(int admin_add_uid,int i){//1代表添加admin权限，0代表取消admin权限
    userfile_read(uidFilename);
    if(UID_CURR!=0){
    }
    if(userinfo.count(admin_add_uid) == 0) {
        return;
    }

    //写太多if了，换换心情
    switch (i){
        case 1:
            userinfo[admin_add_uid].is_admin = true;
            break;
        case 0:
            userinfo[admin_add_uid].is_admin = false;
            break;
        default:
            break;

    }


    userfile_update(uidFilename);
    return;

}


