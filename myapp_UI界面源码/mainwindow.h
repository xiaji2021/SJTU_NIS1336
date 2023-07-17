#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include "task_manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    TaskManager task_manager;

    MainWindow(QWidget *parent = nullptr);
    void update_task();
    ~MainWindow();
public slots:
    void get_attention(int taskID);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
