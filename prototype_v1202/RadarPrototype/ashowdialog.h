#ifndef ASHOWDIALOG_H
#define ASHOWDIALOG_H

#include <QDialog>

namespace Ui {
class AshowDialog;
}

class AshowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AshowDialog(QWidget *parent = nullptr);
    ~AshowDialog();

private:
    Ui::AshowDialog *ui;
};

#endif // ASHOWDIALOG_H
