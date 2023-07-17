#include "login_widget.h"
#include "ui_login_widget.h"
#include <QMessageBox>
#include "Users.h"


login_widget::login_widget(QWidget *parent,int *UID) :
    QDialog(parent),uid(UID),
    ui(new Ui::login_widget)
{
    *uid=-1;
    ui->setupUi(this);
    connect(ui->btn_confirm,&QPushButton::clicked,this,[=]{
        QString username = ui->lineEdit_username->text();
        QString password = ui->lineEdit_password->text();
        //获取用户uid
        *uid=user_check(username.toStdString(),password.toStdString());
        if (*uid<0){
            //出错处理
            QMessageBox::critical(this,"错误","用户信息错误！");
        }
        else {
            this->close();
        }
    });
    connect(ui->btn_cancel,&QPushButton::clicked,this,[=]{
        this->close();
    });
}

login_widget::~login_widget()
{
    delete ui;
}
