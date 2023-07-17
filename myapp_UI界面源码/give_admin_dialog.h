#ifndef GIVE_ADMIN_DIALOG_H
#define GIVE_ADMIN_DIALOG_H

#include <QDialog>

namespace Ui {
class give_admin_dialog;
}

class give_admin_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit give_admin_dialog(QWidget *parent = nullptr);
    ~give_admin_dialog();

private:
    Ui::give_admin_dialog *ui;
};

#endif // GIVE_ADMIN_DIALOG_H
