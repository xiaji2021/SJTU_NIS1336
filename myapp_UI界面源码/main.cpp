#include "mainwindow.h"
#include "login_widget.h"
#include "mythread.h"
#include "Users.h"
#include <QThread>
#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <string>
#include <cstring>
#include <QObject>


int uid=-1;

MainWindow * w_main;
TaskManager * task_manager;


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    //默认注册root用户
    regist("root","root");


    //首先弹出登录页面
    login_widget w_login(NULL,&uid);
    w_login.setWindowTitle("login");
    w_login.exec();


    //登陆成功后弹出主页面
    w_main = new MainWindow(NULL);
    task_manager = & w_main->task_manager;
    w_main->setBaseSize(1000,800);
    if (uid>=0){
        w_main->update_task();
        w_main->show();
    }

    return a.exec();
}
