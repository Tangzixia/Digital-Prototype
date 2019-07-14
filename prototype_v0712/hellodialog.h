#include "mainwindow.h"
#ifndef HELLODIALOG_H
#define HELLODIALOG_H



#include <QDialog>

namespace Ui {
class HelloDialog;
}

class HelloDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelloDialog(QWidget *parent = nullptr);
    ~HelloDialog();

private slots:

    void on_pushButton_open_clicked();

    void on_pushButton_new_clicked();

private:
    Ui::HelloDialog *ui;
    MainWindow *main;
};
#endif // HELLODIALOG_H
