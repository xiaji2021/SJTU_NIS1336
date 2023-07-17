#include "mythread.h"
#include <task_manager.h>
#include <QDateTime>
#include <stdio.h>
#include <QObject>
#include <QDebug>
#include <QMutex>



extern TaskManager * task_manager;


MyThread::MyThread(QObject * parent )
{


}

void MyThread::run()
{
    //子线程的主函数，提醒用户任务是否将要开始
    while (1){
        QDateTime cur_time = QDateTime::currentDateTime();
        for (int i=0;i<task_manager->tasks.size();i++){
            int year,month,day,hour,minute;

            sscanf(task_manager->tasks[i].attention_time.c_str(),"%d:%d:%d:%d:%d",&year,&month,&day,&hour,&minute);
            QDateTime attention_time;
            attention_time.setDate(QDate(year,month,day));
            attention_time.setTime(QTime(hour,minute));
            if (attention_time.date()==cur_time.date()
                    &&attention_time.time().hour()==cur_time.time().hour()
                    &&attention_time.time().minute()==cur_time.time().minute()
                    &&abs(attention_time.time().second()-cur_time.time().second())<=10){
                //对到达提醒时间的任务，释放信号attend(i)
                emit attend(i);
                QThread::sleep(100);
            }
        }
    }

}

