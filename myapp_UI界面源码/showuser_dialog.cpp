#include "showuser_dialog.h"
#include "ui_showuser_dialog.h"
#include "Users.h"


showuser_dialog::showuser_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showuser_dialog)
{
    //显示用户列表
    ui->setupUi(this);
    ui->userlist->setColumnCount(3);
    ui->userlist->setHorizontalHeaderLabels(QStringList()<<"uid"<<"username"<<"is admin");
    ui->userlist->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->userlist->setRowCount(userinfo.size());

    userfile_read(uidFilename);
    int i=0;
    for(const auto& pair:userinfo){

        int col=0;
        ui->userlist->setItem(i,col++,new QTableWidgetItem(QString::number(pair.first)));
        ui->userlist->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(pair.second.username)));
        ui->userlist->setItem(i,col++,new QTableWidgetItem(QString::number(pair.second.is_admin)));

        extern int uid;
        if (pair.first==uid) ui->userlist->item(i,0)->setBackgroundColor(QColor(0,255,0));
        if (pair.first==uid) ui->userlist->item(i,1)->setBackgroundColor(QColor(0,255,0));
        if (pair.first==uid) ui->userlist->item(i,2)->setBackgroundColor(QColor(0,255,0));

        i++;
    }


}

showuser_dialog::~showuser_dialog()
{
    delete ui;
}
