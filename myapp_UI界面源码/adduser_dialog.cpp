#include "adduser_dialog.h"
#include "ui_adduser_dialog.h"
#include "Users.h"
#include <QMessageBox>

adduser_dialog::adduser_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adduser_dialog)
{
    ui->setupUi(this);
    connect(ui->btn_confirm,&QPushButton::clicked,this,[=]{
        QString username = ui->lineEdit_username->text();
        QString password = ui->lineEdit_password->text();
        int uid_add = regist(username.toStdString(),password.toStdString());
        if (uid_add<0){
            //出错处理
            QMessageBox::critical(this,"注册失败","注册失败！");
        }
        else{
            QMessageBox::information(this,"注册成功","你的uid为"+QString::number(uid_add));
        }
        close();
    });


    connect(ui->btn_cancel,&QPushButton::clicked,this,[=]{
        this->close();
    });
}

adduser_dialog::~adduser_dialog()
{
    delete ui;
}
