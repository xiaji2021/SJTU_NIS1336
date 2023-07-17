#include "give_admin_dialog.h"
#include "ui_give_admin_dialog.h"
#include "Users.h"
#include <QMessageBox>

give_admin_dialog::give_admin_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::give_admin_dialog)
{
    ui->setupUi(this);
    connect(ui->btn_confirm,&QPushButton::clicked,this,[=]{
        QString current_password = ui->lineEdit_root_password->text();
        QString uid_give_admin = ui->lineEdit_give_admin->text();
        int uid_root=user_check("root",current_password.toStdString());
        if (uid_root!=0){
            //出错处理
            QMessageBox::critical(this,"登陆错误","root登陆错误！");
        }
        //赋予用户管理员权限
        admin_change(uid_give_admin.toInt(),1);

        this->close();
    });
    connect(ui->btn_cancel,&QPushButton::clicked,this,[=]{
        this->close();
    });

}

give_admin_dialog::~give_admin_dialog()
{
    delete ui;
}
