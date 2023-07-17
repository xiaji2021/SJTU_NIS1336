#include "addtask_widget.h"
#include "ui_addtask_widget.h"
#include "task_manager.h"
#include <QDebug>
#include <string>
#include <cstring>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>

using namespace std;

addtask_widget::addtask_widget(QWidget *parent,TaskManager* parent_task_manager) :
    QDialog(parent),
    ui(new Ui::addtask_widget)
{
    //设置缺省值
    task_manager=parent_task_manager;
    ui->setupUi(this);
    ui->lineEdit_taskname->setText("task");
    ui->dateTimeEdit_start_time->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_attention_time->setDateTime(QDateTime::currentDateTime());
    ui->spinBox_priority->setValue(0);
    ui->comboBox_category->addItem("learning");
    ui->comboBox_category->addItem("living");
    ui->comboBox_category->addItem("amusement");
    ui->comboBox_category->addItem("others");

    connect(ui->btn_confirm,&QPushButton::clicked,[=]{
        QString taskname = ui->lineEdit_taskname->text();
        QString start_time = ui->dateTimeEdit_start_time->text();
        QString priority = QString::number(ui->spinBox_priority->value());
        QString category = ui->comboBox_category->currentText();
        QString attention_time = ui->dateTimeEdit_attention_time->text();


        QDateTime st = ui->dateTimeEdit_start_time->dateTime();
        QString st_string = QString::number(st.date().year())+":"+QString::number(st.date().month())+":"
                +QString::number(st.date().day())+":"+QString::number(st.time().hour())+":"
                +QString::number(st.time().minute());

        QDateTime at = ui->dateTimeEdit_attention_time->dateTime();
        QString at_string = QString::number(at.date().year())+":"+QString::number(at.date().month())+":"
                +QString::number(at.date().day())+":"+QString::number(at.time().hour())+":"
                +QString::number(at.time().minute());


        if (taskname==""||at<QDateTime::currentDateTime()||st<at){
            //出错处理
            QMessageBox::critical(this,"error","add task error!");
            return;
        }
        //添加任务
        task_manager->addTask(taskname.toStdString(),st_string.toStdString(),
                              atoi(priority.toStdString().c_str()),
                              category.toStdString(),at_string.toStdString());
        this->close();
    });
    connect(ui->btn_cancel,&QPushButton::clicked,[=]{
        this->close();
    });
}

addtask_widget::~addtask_widget()
{
    delete ui;
}




