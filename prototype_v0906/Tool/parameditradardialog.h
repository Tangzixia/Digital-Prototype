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
    explicit ParamEditRadarDialog(AlgorithmComp ac, QWidget *parent = nullptr);
    ~ParamEditRadarDialog();
    // 保存用户填写的信息
    AlgorithmComp ac;
    // 保存信息
    QMap<QString, QString> mp;

private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

    void on_pushButton_Path_clicked();

    void on_pushButton_Add_clicked();

    void on_pushButton_Del_clicked();

private:
    Ui::ParamEditRadarDialog *ui;
    // 当前参数有几行
    int row=0;
    // 保存编辑文件的名字，不用。改为直接传入AlgorithmComp对象
//    QString fname;
};

#endif // PARAMEDITRADARDIALOG_H
