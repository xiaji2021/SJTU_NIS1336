#ifndef ADDTASK_WIDGET_H
#define ADDTASK_WIDGET_H

#include <QWidget>
#include <QDialog>
#include"task_manager.h"
namespace Ui {
class addtask_widget;
}

class addtask_widget : public QDialog
{
    Q_OBJECT

public:
    explicit addtask_widget(QWidget *parent = nullptr,TaskManager * parent_task_manager = NULL);
    ~addtask_widget();
    Ui::addtask_widget *ui;
    TaskManager * task_manager;
private:
};

#endif // ADDTASK_WIDGET_H
