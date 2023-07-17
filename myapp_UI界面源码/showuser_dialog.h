#ifndef SHOWUSER_DIALOG_H
#define SHOWUSER_DIALOG_H

#include <QDialog>

namespace Ui {
class showuser_dialog;
}

class showuser_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit showuser_dialog(QWidget *parent = nullptr);
    ~showuser_dialog();

private:
    Ui::showuser_dialog *ui;
};

#endif // SHOWUSER_DIALOG_H
