#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addtask_widget.h"
#include "give_admin_dialog.h"
#include "task_manager.h"
#include "login_widget.h"
#include "deluser_dialog.h"
#include "showuser_dialog.h"
#include "Users.h"
#include "adduser_dialog.h"
#include "changepassword_dialog.h"
#include "mythread.h"
#include <QDialog>
#include <string>
#include <QMenuBar>
#include <QMenu>
#include <QDebug>
#include <QMessageBox>
#include <QStatusBar>
#include <QLabel>
using namespace std;

extern int uid;
MyThread * mythread;
void MainWindow::update_task()
{
    //将任务信息从全局变量task_manager中显示到tasktable中
    extern std::string taskfilename;
    taskfilename="user"+QString::number(uid).toStdString()+".txt";
    task_manager.readFromFile(taskfilename);
    ui->tasktable->setRowCount(task_manager.taskIdCounter);
    for (int i=0;i<task_manager.taskIdCounter;i++){
        int col=0;
        ui->tasktable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(to_string(task_manager.tasks[i].taskId))));
        ui->tasktable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(task_manager.tasks[i].task_name)));
        ui->tasktable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(task_manager.tasks[i].start_time)));
        ui->tasktable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(to_string(task_manager.tasks[i].priority))));
        ui->tasktable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(task_manager.tasks[i].category)));
        ui->tasktable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(task_manager.tasks[i].attention_time)));
    }


}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tasktable->setColumnCount(6);
    ui->tasktable->setHorizontalHeaderLabels(QStringList()<<"taskID"<<"taskname"<<"start time"<<"priority"<<"category"<<"attention time");
    update_task();
    ui->tasktable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //创建并运行子线程
    mythread = new MyThread(this);
    //子线程与主线程通过信号与槽进行通信
    void (MyThread::*signal)(int) = &MyThread::attend;
    connect(mythread,signal,this,&MainWindow::get_attention);
    mythread->start();


    //提示用户当前uid的提示标签
    ui->label_status->setText("your current uid is "+QString::number(uid));


    connect(ui->btn_add,&QPushButton::clicked,this,[=]{
        //添加任务
        addtask_widget *dlg = new addtask_widget(this,&task_manager);
        dlg->setFixedSize(800,500);
        dlg->exec();
        update_task();
    });
    connect(ui->btn_del,&QPushButton::clicked,this,[=]{
        //删除任务
        if (ui->tasktable->selectedItems().size()>0){
            int row_id=ui->tasktable->selectedItems().first()->row();
            int uid_del = ui->tasktable->item(row_id,0)->text().toInt();
            task_manager.deleteTask(uid_del);
            update_task();
        }
    });

    connect(ui->action_add_user,&QAction::triggered,this,[=]{
        //添加用户
        adduser_dialog w_add(this);
        w_add.setWindowTitle("add user");
        w_add.exec();
    });
    connect(ui->action_change_user,&QAction::triggered,this,[=]{
        //更换用户
        int uid_change;
        login_widget w_add(this,&uid_change);
        w_add.setWindowTitle("change user");
        w_add.exec();
        if (uid_change>=0){
            uid=uid_change;
            update_task();
            ui->label_status->setText("your current uid is "+QString::number(uid));
        }
    });
    connect(ui->action_del_user,&QAction::triggered,this,[=]{
        //删除用户
        int uid_delete;
        deluser_dialog w_del(this,&uid_delete);
        w_del.setWindowTitle("del user");
        w_del.exec();
        dele_user(uid_delete);
    });
    connect(ui->action_show_user,&QAction::triggered,this,[=]{
        //展示用户列表
        showuser_dialog w_show(this);
        w_show.exec();
    });
    connect(ui->action_change_password,&QAction::triggered,this,[=]{
        //更改密码
        changepassword_dialog w_change(this);
        w_change.exec();
    });

    connect(ui->action_sort_by_taskID,&QAction::triggered,this,[=]{
        //根据taskID排序
        ui->tasktable->sortItems(0);
    });
    connect(ui->action_sort_by_start_time,&QAction::triggered,this,[=]{
        //根据开始时间排序
        ui->tasktable->sortItems(2);
    });
    connect(ui->action_sort_by_priority,&QAction::triggered,this,[=]{
        //根据优先级排序
        ui->tasktable->sortItems(3);
    });

    connect(ui->action_sort_by_category,&QAction::triggered,this,[=]{
        //根据种类排序
        ui->tasktable->sortItems(4);
    });
    connect(ui->action_sort_by_attention_time,&QAction::triggered,this,[=]{
        //根据提醒时间排序
        ui->tasktable->sortItems(5);
    });

    connect(ui->action_give_admin,&QAction::triggered,this,[=]{
        //赋予用户管理员权限
        give_admin_dialog w_give;
        w_give.exec();
    });


}

MainWindow::~MainWindow()
{
    //在主线程结束时退出子线程
    mythread->quit();
    delete ui;
}


void MainWindow::get_attention(int taskID){
    QMessageBox::information(NULL,"attention",
                             "task "+QString::fromStdString(task_manager.tasks[taskID].task_name)
                             +" is about to start!");
}
