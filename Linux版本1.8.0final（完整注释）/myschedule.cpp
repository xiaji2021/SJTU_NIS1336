

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<vector>
#include<stdlib.h>
#include<cstring> 
#include<iostream>
#include <ctime>
#include <thread>
#include "task_manager.h"
#include "Users.h"
#include "scp.h"

using namespace std;

// 给全局变量退出线程标志赋初值
int exitflag = 0;

TaskManager task_manager;

void showHelp2() {
    printf("addtask taskname start_time [-p priority] [-c category] [-t attention_time] (where time should be yyyy:mo:dd:hh:mi:ss)\n");
    printf("showtask [-date date]/[-month month]/[-category category] (date should be yyyy:mo:dd and month should be yyyy:mo)\n");
    printf("deltask tasknumber\n");
    printf("adduser username password\n");
    printf("deluser uid\n");
    printf("showuser uid\n");
    printf("changepassword uid\n");
    printf("admin_change uid admin_flag (admin_flag: 0 - remove admin privilege, 1 - add admin privilege)\n");
    printf("su_user uid\n");
    printf("help\n");
    printf("exit\n");
}

// 读取用户输入的命令并处理
void *read(void *arg) {
    std::string username;
    std::string password;
    printf("username: ");
    std::cin >> username;
    printf("password: ");
    std::cin >> password;

    // 验证用户名和密码
    int uid = user_check(username.c_str(), password.c_str());
    if (uid < 0) {
        printf("username or password error!\n");
		exitflag = 1;
		pthread_exit(nullptr);
    } else {
        printf("login success! your uid is %d\n", uid);
    }

    // 根据用户的 UID 确定任务文件名
    taskfilename = "user" + std::to_string(uid) + ".txt";

    std::string line;
    std::vector<std::string> argv;
    while (1) {
        std::getline(std::cin, line);
        if (line.empty()) {
            continue;
        }

        // 解析命令行参数
        std::istringstream iss(line);
        std::string token;
        while (iss >> token) {
            argv.push_back(token);
        }

        int argc = argv.size();

        if (argc == 1 && strcasecmp(argv[0].c_str(), "help") == 0) {
            showHelp2();
        } else if (argc == 1 && strcasecmp(argv[0].c_str(), "showtask") == 0) {
          task_manager.showTask("", "", "");  // 显示所有任务
        } else if (argc >= 2 && strcasecmp(argv[0].c_str(), "showtask") == 0) {
           std::string date = "";
           std::string month = "";
           std::string category = "";

          // 解析命令行参数以获取日期、月份和分类
          for (int i = 1; i < argc; i++) {
             if (strcasecmp(argv[i].c_str(), "-date") == 0 && i + 1 < argc) {
             date = argv[i + 1];
             } else if (strcasecmp(argv[i].c_str(), "-month") == 0 && i + 1 < argc) {
             month = argv[i + 1];
             } else if (strcasecmp(argv[i].c_str(), "-category") == 0 && i + 1 < argc) {
             category = argv[i + 1];
             }
            }
            task_manager.showTask(date, month, category);// 显示符合条件的任务
        } else if (argc == 2 && strcasecmp(argv[0].c_str(), "deltask") == 0) {
            int task_id = atoi(argv[1].c_str());
            if (task_id < 0) {
                printf("input error\n");
                 argv.clear();
                continue;
            }
            task_manager.deleteTask(task_id);// 删除指定 ID 的任务
        } else if (strcasecmp(argv[0].c_str(), "addtask") == 0) {
            std::string task_name;
            std::string start_time;
            int priority = 0;
            std::string category = "Others";
            std::string attention_time = " ";

            if (argc < 3) {
                printf("input error\n");
                 argv.clear();
                continue;
            }
            task_name = argv[1];
            start_time = argv[2];

            int flag = 1;
            for (int i = 0; i < argc - 1; i++) {
                if (strcasecmp(argv[i].c_str(), "-p") == 0) {
                    priority = atoi(argv[i + 1].c_str());
                    if (priority < 0) {
                        printf("input error\n");
                        flag = 0;
                        break;
                    }
                }
                if (strcasecmp(argv[i].c_str(), "-c") == 0) {
                    category = argv[i + 1];
                }
                if (strcasecmp(argv[i].c_str(), "-t") == 0) {
                    attention_time = argv[i + 1];
                }
            }
            if (!flag) {
                 argv.clear();
                continue;
            }
            task_manager.addTask(task_name, start_time, priority, category, attention_time);// 添加任务
        } else if (argc == 3 && strcasecmp(argv[0].c_str(), "adduser") == 0) {
            std::string username = argv[1];
            std::string password = argv[2];
            int uid = regist(username.c_str(), password.c_str());
            if (uid < 0) {
                printf("regist failed\n");
            } else {
                printf("regist success: your uid is %d\n", uid);
            }
        } else if (argc == 2 && strcasecmp(argv[0].c_str(), "deluser") == 0) {
            int uid = atoi(argv[1].c_str());
            if (uid < 0) {
                printf("input error\n");
                 argv.clear();
                continue;
            }
            dele_user(uid);// 删除用户
        } else if (argc == 1 && strcasecmp(argv[0].c_str(), "showuser") == 0) {
            show_user(); // 显示用户信息
        } else if (argc == 2 && strcasecmp(argv[0].c_str(), "changepassword") == 0) {
            int uid = atoi(argv[1].c_str());
            if (uid < 0) {
                printf("input error\n");
                argv.clear();
                continue;
            }
            change_passwd(uid);// 修改用户密码
        } else if (argc == 3 && strcasecmp(argv[0].c_str(), "admin_change") == 0) {
            int admin_add_uid = atoi(argv[1].c_str());
            int i = atoi(argv[2].c_str());
            admin_change(admin_add_uid, i);// 添加或移除管理员权限
        } else if (argc == 2 && strcasecmp(argv[0].c_str(), "su_user") == 0) {
            int su_uid = atoi(argv[1].c_str());
            su_user(su_uid,task_manager);// 切换用户身份
        } else if (argc == 1 && strcasecmp(argv[0].c_str(), "exit") == 0) {  
			exitflag = 1;
			pthread_exit(nullptr);// 退出线程
        } else {
            printf("input error\n");
            argv.clear();
            continue;
        }
        argv.clear();
    }
    return NULL;
}

// 提醒线程函数
void *attention(void *arg) {
    while (true) {
        // 检查任务的注意时间
        task_manager.checkAttentionTime();

        // 等待一秒钟 // 等待一秒钟
    }
    return nullptr;
}

int run() {
    pthread_t thread_id1;
    pthread_t thread_id2;

    if (pthread_create(&thread_id1, NULL, read, NULL)) {
        printf("read creation failed\n");
        return -1;
    }
    if (pthread_create(&thread_id2, NULL, attention, NULL)) {
        printf("attention creation failed\n");
        return -1;
    }

    pthread_join(thread_id2, NULL);
    pthread_join(thread_id1, NULL);

    return 0;
}


void showHelp() {
    printf("$myschedule username password addtask taskname start_time [-p priority] [-c category] [-t attention_time] (where time should be yyyy:mo:dd:hh:mi:ss)\n");
    printf("$myschedule username password showtask [-date date]/[-month month]/[-category category] (date should be yyyy:mo:dd and month should be yyyy:mo)\n");
    printf("$myschedule username password deltask tasknumber\n");
    printf("$myschedule adduser username password\n");
    printf("$myschedule deluser uid\n");
    printf("$myschedule showuser uid\n");
    printf("$myschedule changepassowrd uid\n");
    printf("$myschedule -run\n");
    printf("$myschedule -h\n");
}


int main(int argc,char* argv[])
{
    // SSH会话连接到服务器
    ssh_session sshSession = connect2server();

    // 从服务器下载文件
    scp_download(sshSession);

	char username[20];
	char password[20];

    // 检查命令行参数以确定要执行的操作 
	if (argc==2&&strcasecmp(argv[1],"run")==0){
        // 运行任务管理器
		run();
	}
	else if (argc==2&&strcasecmp(argv[1],"-h")==0){
		showHelp();
	}
    else if (argc >= 4 && strcasecmp(argv[3], "showtask") == 0) {
       // 显示任务
       strcpy(username, argv[1]);
       strcpy(password, argv[2]);
       int uid = user_check(username, password);
       if (uid < 0) {
        printf("username or password error!\n");
        return -1;
       }
       taskfilename = "user" + to_string(uid) + ".txt";

       std::string date = "";   // 初始化为默认值
       std::string month = "";  // 初始化为默认值
       std::string category = ""; // 初始化为默认值

      // 检查命令行参数是否提供了日期、月份或分类
       for (int i = 4; i < argc; i++) {
         if (strcasecmp(argv[i], "-date") == 0 && i + 1 < argc) {
            date = argv[i + 1];
         } else if (strcasecmp(argv[i], "-month") == 0 && i + 1 < argc) {
            month = argv[i + 1];
         } else if (strcasecmp(argv[i], "-category") == 0 && i + 1 < argc) {
            category = argv[i + 1];
         }
        }
       task_manager.showTask(date, month, category);
    }
	else if (argc==5&&strcasecmp(argv[3],"deltask")==0){
		// 删除任务
		strcpy(username,argv[1]);
		strcpy(password,argv[2]);
		int uid=user_check(username,password);
		if (uid<0){
			printf("username or password error!\n");
			return -1;
		}
		taskfilename="user"+to_string(uid)+".txt";
		FILE *fp; 

		
		int task_id = atoi(argv[4]);
		if (task_id<0) {
			printf("input error!\n");
			return -1;
		}
		task_manager.deleteTask(task_id);
	} 
	else if (argc>3&&strcasecmp(argv[3],"addtask")==0){
		// 添加任务
		if (argc<6) {printf("input error!\n");return -1;}
		strcpy(username,argv[1]);
		strcpy(password,argv[2]);
		int uid=user_check(username,password);
		if (uid<0){
			printf("username or password error!\n");
			return -1;
		}
		taskfilename="user"+to_string(uid)+".txt";

		char task_name[100]=" ";
		char start_time[100]=" ";
		int priority=0;
		char category[100]="Others";
		char attention_time[100]=" ";
	
		strcpy(task_name,argv[4]);
		strcpy(start_time,argv[5]);
		
		int ret=getopt(argc,argv,"p:c:t:");
		while(ret!=-1){
			switch(ret){
				case 'p':
					priority=atoi(optarg);
					if (priority<0){printf("input error");return -1;}
					break; 
				case 'c':
					strcpy(category,optarg);
					break;	
				case 't':
					strcpy(attention_time,optarg);
			}
			ret=getopt(argc,argv,"p:c:t:");
		}
		task_manager.addTask(task_name,start_time,priority,category,attention_time);
	}
	else if (argc==4&&strcasecmp(argv[1],"adduser")==0){
		// 添加用户
		char username[20];
		char password[20];
		strcpy(username,argv[2]);
		strcpy(password,argv[3]);
		int uid = regist(username,password);
		if (uid<0) printf("regist failed\n");
		else printf("regist success: your uid is %d\n",uid);
	}
	else if (argc==3&&strcasecmp(argv[1],"deluser")==0){
		// 删除用户
		int uid=atoi(argv[2]);
		if (uid<0) {printf("input error\n");return -1;}
		UID_CURR=0;
		dele_user(uid);
	}
	else if (argc==2&&strcasecmp(argv[1],"showuser")==0){
		// 显示用户
		UID_CURR=0;
		show_user();
	} 
	else if (argc==3&&strcasecmp(argv[1],"changepassword")==0){
		// 修改密码
		int uid=atoi(argv[2]);
		if (uid<0) {printf("input error\n");return -1;}
		change_passwd(uid);
	}
	else {printf("input error!\n");return -1;}
	
    // 上传任务文件到服务器
    scp_upload(sshSession);

    // 释放SSH会话资源
    free_ssh_session(sshSession);
    
    return 0;
}
