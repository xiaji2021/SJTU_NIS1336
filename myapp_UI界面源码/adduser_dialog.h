#ifndef ADDUSER_DIALOG_H
#define ADDUSER_DIALOG_H

#include <QDialog>

namespace Ui {
class adduser_dialog;
}

class adduser_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit adduser_dialog(QWidget *parent = nullptr);
    ~adduser_dialog();

private:
    Ui::adduser_dialog *ui;
};

#endif // ADDUSER_DIALOG_H
