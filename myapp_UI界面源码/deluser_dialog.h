#ifndef DELUSER_DIALOG_H
#define DELUSER_DIALOG_H

#include <QDialog>

namespace Ui {
class deluser_dialog;
}

class deluser_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit deluser_dialog(QWidget *parent = nullptr,int * uid=NULL);
    ~deluser_dialog();

private:
    int * uid;
    Ui::deluser_dialog *ui;
};

#endif // DELUSER_DIALOG_H
