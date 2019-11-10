#ifndef PPIDIALOG_H
#define PPIDIALOG_H

#include <QDialog>

namespace Ui {
class PPIDialog;
}

class PPIDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PPIDialog(QWidget *parent = nullptr);
    ~PPIDialog();

private slots:


    void on_radioBtnShowOn_clicked(bool checked);

    void on_radioBtnShowOFF_clicked(bool checked);

private:
    Ui::PPIDialog *ui;
};

#endif // PPIDIALOG_H
