#ifndef LOGIN_WIDGET_H
#define LOGIN_WIDGET_H

#include <QDialog>

namespace Ui {
class login_widget;
}

class login_widget : public QDialog
{
    Q_OBJECT

public:
    explicit login_widget(QWidget *parent = nullptr,int *uid=NULL);
    ~login_widget();

    Ui::login_widget *ui;
    int *uid;
private:
};

#endif // LOGIN_WIDGET_H
