#include "changepassword_dialog.h"
#include "ui_changepassword_dialog.h"
#include "Users.h"

changepassword_dialog::changepassword_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changepassword_dialog)
{
    ui->setupUi(this);
    connect(ui->btn_confirm,&QPushButton::clicked,this,[=]{
        QString username = ui->lineEdit_username->text();
        QString current_password = ui->lineEdit_current_password->text();
        QString new_password = ui->lineEdit_new_password->text();
        QString confirm_new_password = ui->lineEdit_confirm_new_password->text();
        int uid=user_check(username.toStdString(),current_password.toStdString());
        //更改密码
        change_passwd(uid,current_password.toStdString(),new_password.toStdString(),confirm_new_password.toStdString());
        this->close();
    });
    connect(ui->btn_cancel,&QPushButton::clicked,this,[=]{
        this->close();
    });

}

changepassword_dialog::~changepassword_dialog()
{
    delete ui;
}
