#ifndef MYTHREAD_H
#define MYTHREAD_H
#include<QThread>
#include <QObject>

class MyThread:public QThread{
    Q_OBJECT

public:
    explicit MyThread(QObject *parent = nullptr);
    void run() ;

signals:
    void isDone();
    void attend(int);
};

#endif // MYTHREAD_H
