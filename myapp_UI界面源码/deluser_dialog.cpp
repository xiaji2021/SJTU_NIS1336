#include "deluser_dialog.h"
#include "ui_deluser_dialog.h"
#include <QMessageBox>

deluser_dialog::deluser_dialog(QWidget *parent,int *UID) :
    QDialog(parent),uid(UID),
    ui(new Ui::deluser_dialog)
{
    ui->setupUi(this);
    connect(ui->btn_confirm,&QPushButton::clicked,this,[=]{
        *uid = ui->lineEdit->text().toInt();
        if (*uid<0){
            //出错处理
            QMessageBox::critical(this,"错误信息","用户确认错误信息");
        }
        else {
            this->close();
        }
        //删除操作在mainwindow.cpp中进行
    });
    connect(ui->btn_cancel,&QPushButton::clicked,this,[=]{
        this->close();
    });
}

deluser_dialog::~deluser_dialog()
{
    delete ui;
}
