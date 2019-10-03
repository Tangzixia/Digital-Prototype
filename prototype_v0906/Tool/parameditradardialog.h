#ifndef PARAMEDITRADARDIALOG_H
#define PARAMEDITRADARDIALOG_H

#include "algorithmcomp.h"

#include <QDialog>

namespace Ui {
class ParamEditRadarDialog;
}

class ParamEditRadarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParamEditRadarDialog(QWidget *parent = nullptr);
    ~ParamEditRadarDialog();
    AlgorithmComp ac;
    QMap<QString, QString> mp;

private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

    void on_pushButton_Path_clicked();

    void on_pushButton_Add_clicked();

    void on_pushButton_Del_clicked();

private:
    Ui::ParamEditRadarDialog *ui;
    int row=0;
};

#endif // PARAMEDITRADARDIALOG_H
