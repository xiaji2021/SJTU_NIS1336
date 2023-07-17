#ifndef CHANGEPASSWORD_DIALOG_H
#define CHANGEPASSWORD_DIALOG_H

#include <QDialog>

namespace Ui {
class changepassword_dialog;
}

class changepassword_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit changepassword_dialog(QWidget *parent = nullptr);
    ~changepassword_dialog();

private:
    Ui::changepassword_dialog *ui;
};

#endif // CHANGEPASSWORD_DIALOG_H
