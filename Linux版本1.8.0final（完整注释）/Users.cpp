//
// Created by 夏季 on 2023/7/3.
//

#include "Users.h"
#include "task_manager.h"
//#include <bcrypt/BCrypt.hpp>

//用户数量
int UID_NUM = 0;
//当前登录用户ID
int UID_CURR = -1;
//用户ID存储文件名
std::string uidFilename = "uid.txt";
//用户数据保存容器
std::map<int ,USER> userinfo;

//用STL中的哈希函数进行哈希
std::string STL_hash(std::string str){
    std::hash<std::string> hasher;
    size_t hashValue = hasher(str);
    std::stringstream ss;
    ss << hashValue;
    std::string hash_str = ss.str();
    return hash_str;
}

//从用户数据存储文件uid.txt中获取文件，返回获取的文件个数
int userfile_read(const std::string &uid_filename) {

    //尝试打开文件，如果打开失败，创建文件
    std::ifstream *file =new std::ifstream(uid_filename);

    if(file->fail())
    {
        std::cout<<"无法打开用户信息文件，尝试创建..."<<std::endl;

        //创建文件
        std::ofstream new_file(uid_filename);
        if(new_file.is_open()) new_file.close();
        delete file;

        //再次以读取方式打开文件，并确认是否创建文件成功
        file = new std::ifstream(uid_filename);
        if(file->fail())  {
            std::cout<<"无法正常创建文件，请检查问题"<< std::endl;
            return -1;
        }
    }

    //从文件中逐步行读取文件，并保存到map中
    std::string line;
    while(std::getline(*file, line))
    {
        //iss默认以空格分隔字符串
        std::istringstream iss(line);
        USER user_tmp;int uid_tmp;

        iss>>uid_tmp;
        iss>>user_tmp.username;
        iss>>user_tmp.password_hash;
        iss>>user_tmp.is_admin;

        userinfo[uid_tmp] = user_tmp;
    }

    return userinfo.size();//返回容器中元素的个数
}

//向uid.txt中写入用户数据
int userfile_update(const std::string &uid_filename){

    std::ofstream file(uid_filename);

    //遍历写入
    for(const auto& pair : userinfo){ //C++11
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

    //无用户时，默认创建新root用户
    if(userfile_read(uidFilename)==0)  {
        std::cout<<"你是root用户，请牢记账户密码"<<std::endl;
        new_uid = 0;
    }

    //注册普通用户及错误处理
    else
    {
        for (const auto &pair: userinfo) {

            //处理用户名存在的情况
            if (pair.second.username == name) {
                std::cout << "用户名已存在" << std::endl;
                return -1;//失败
            }

            //确认UID的开始位置，默认新用户uid为当前uid最大的用户后一位
            auto key_max = userinfo.rbegin();
            new_uid = key_max->first + 1;
        }
    }

    //用户过多处理，默认不能增加超过100次
    if (new_uid==100) {std::printf("too much users\n");return -1;}

    UID_NUM++;

    //注册完成后用户task文件创建处理
    std::string userfilename="user"+std::to_string(new_uid)+".txt";
	std::ofstream userfile(userfilename);
    //错误处理
	if (!userfile.is_open()){
		printf("create userfile failure\n");
		return -1;
	}

    //uid.txt文件中增加一行
    std::string hash_tmp = STL_hash(passwd);
    USER user_tmp;
    user_tmp.password_hash = hash_tmp;
    user_tmp.username = name;

    //root用户默认赋予管理权限
    if(new_uid == 0) user_tmp.is_admin = 1;
    else user_tmp.is_admin = 0;

    userinfo[new_uid] = user_tmp;
    userfile_update(uidFilename);

    return new_uid;

}

int user_check(const std::string& name, const std::string& passwd) {

    //读取用户文件
    userfile_read(uidFilename);
    //std::string hash_tmp = BCrypt::generateHash(passwd);
    std::string hash_tmp = STL_hash(passwd);
    int Match = 0;

    for (const auto &pair: userinfo)
    {
        //找到对应用户名
        if (pair.second.username == name)
        {
            Match++;

            //确认密码是否正确
            if (/*BCrypt::validatePassword(passwd, pair.second.password_hash)*/pair.second.password_hash ==hash_tmp)
            {
                UID_CURR = pair.first;
                return pair.first;
            }

            //错误处理
            else
            {
                std::cout<<"密码错误！"<<std::endl;
                return -1;
            }
        }
    }

    if(Match == 0) {
        std::cout<<"无效的用户名"<<std::endl;
        return -1;
    }

}

void change_passwd(int uid_change)
{
    //检查uid的有效性
    if(userinfo.count(uid_change) == 0){
        std::cout<<"无效的用户!"<<std::endl;
        return;
    }

    //检查是否有权限
    if(userinfo[UID_CURR].is_admin==0){
        std::cout<<"无权限！"<<std::endl;
        return ;
    }

    //如果修改管理员用户，需要root权限，或者是修改自己
    else if (UID_CURR!=0&&userinfo[uid_change].is_admin==1&&uid_change!=UID_CURR) {

        std::cout << "只有root用户可以修改管理员用户密码！" << std::endl;
        return;
    }

    else
    {
        std::string curr_passwd;
        std::string new_passwd;
        std::string new_passwd_repeat;

        std::cout<<"请输入当前密码:";
        std::cin>>curr_passwd;

        // if(BCrypt::generateHash(curr_passwd) != userinfo[uid_change])
        if(STL_hash(curr_passwd) != userinfo[uid_change].password_hash){
            std::cout<<"密码错误！"<<std::endl;
            return;
        }

        //最多允许三次输入错误
        for(int i=0;i<3;i++){
            std::cout<<"请输入新密码:";
            std::cin>>new_passwd;
            std::cout<<"请再次输入新密码:";
            std::cin>>new_passwd_repeat;
            std::scanf("\n");

            //两次输入密码相同则接受
            if(new_passwd == new_passwd_repeat){
            userinfo[uid_change].password_hash =STL_hash(new_passwd);
            userfile_update(uidFilename);
            return ;
            }

            else std::cout<<"两次输入的密码不一致，请重新输入"<<std::endl;
        }

        return;
    }

}



bool dele_user(int uid_dele){

    userfile_read(uidFilename);

    if(userinfo.count(uid_dele) == 0) {
        std::cout<<"无效的用户id!"<<std::endl;
        return 0;
    }

    if(userinfo[UID_CURR].is_admin==0) {
        std::cout<<"无权限！"<<std::endl;
        return 0;
    }

    //root可以删除非自己的管理员，管理员可以删除普通用户
    else if(UID_CURR==uid_dele){
        std::cout<<"无法删除自身账户，请咨询root管理员！"<<std::endl;
        return 0;
    }

    else if(UID_CURR!=0&&userinfo[uid_dele].is_admin==1){
        std::cout<<"管理员账户需要root权限删除"<<std::endl;

        return 0;
    }

    //进行用户删除操作
    else {
        //map中删除用户信息
        userinfo.erase(uid_dele);
        //txt中删除用户信息
        std::string userfilename="user"+std::to_string(uid_dele)+".txt";
	    std::remove(userfilename.c_str());
        //重新读入用户文件
	    userfile_update(uidFilename);

        UID_NUM--;

        return 1;
    }

}


void show_user(){

    userfile_read(uidFilename);
    std::cout<<"  "<<"UID"<<" "<<"USERNAME"<<std::endl;

    //遍历输出
    for(const auto& pair:userinfo){

        //当前用户带*
        if(pair.first == UID_CURR) std::cout<<"*";else std::cout<<" ";

        //管理员用户带$
        if(pair.second.is_admin == 1) std::cout<<"$";
        else std::cout<<" ";

        //输出其他字段信息
        std::cout<<pair.first<<" "<< pair.second.username<<std::endl;
    }

    std::cout<<"注：*代表当前用户,$代表有管理权限的用户"<<std::endl;

}

void admin_change(int admin_add_uid,int i){
    //1代表添加admin权限，0代表取消admin权限

    userfile_read(uidFilename);

    //root用户才有权限进行此操作
    if(UID_CURR!=0){
        std::cout<<"非root用户，无法执行此操作"<<std::endl;return;
    }

    //检查id号是否存在
    if(userinfo.count(admin_add_uid) == 0) {
        std::cout<<"无效的用户id!"<<std::endl;
        return;
    }

    //输出当前用户权限
    if(userinfo[admin_add_uid].is_admin == 0) std::cout<<"当前权限：普通用户"<<std::endl;
    else std::cout<<"当前权限：管理员用户"<<std::endl;

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

    //输出修改后权限
    if(userinfo[admin_add_uid].is_admin == 0) std::cout<<"修改后权限：普通用户"<<std::endl;
    else std::cout<<"修改后权限：管理员用户"<<std::endl;

    //更新用户信息文件
    userfile_update(uidFilename);

    return;

}

void su_user(int su_uid, TaskManager& task_manager){

    userfile_read(uidFilename);

    if(userinfo.count(su_uid) == 0) {
        std::cout<<"无效的用户id!"<<std::endl;
        return;
    }

    if(su_uid == UID_CURR)  {
        std::cout<<"已是当前用户状态，无需切换"<<std::endl;
        return;
    }

    //处理用户切换
    std::string curr_passwd;
    std::cout<<"请输入需要切换的用户密码:";
    std::cin>>curr_passwd;

    if(user_check(userinfo[su_uid].username,curr_passwd) == su_uid)
    {
        std::cout<<"切换成功！"<<std::endl;
        UID_CURR = su_uid;

        // 重新创建 TaskManager 对象并读取任务文件
        taskfilename = "user"+std::to_string(su_uid)+".txt";
        TaskManager new_task_manager;
        new_task_manager.readFromFile(taskfilename);

        // 更新全局的task_manager
        task_manager = new_task_manager;

        return;
    }
    else
    {
        std::cout<<"密码错误！"<<std::endl;

        return;
    }

}
