#include "compproperty.h"
#include "ui_compproperty.h"

#include <QDir>
#include <QFileDialog>

CompProperty::CompProperty(QString name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompProperty)
{
    ui->setupUi(this);
    ui->label_path->setText(path_);
    ui->label->setText(name);
}

CompProperty::~CompProperty()
{
    delete ui;
}

/**
 * @brief 用户选择路径
 */
void CompProperty::on_pushButton_clicked()
{
    QString dirpath = "/home/a913/Qt_project/20191130/prototype_v0906/Projects/test/code/";
//    QString path = QFileDialog::getExistingDirectory(this, "选择文件夹", QString(dirpath), QFileDialog::ShowDirsOnly);
    QString path = QFileDialog::getOpenFileName(this, tr("选择文件夹"),
                                                    QString(dirpath),
                                                    tr("text (*.txt *.xml)"));
    path_ = path;
    ui->label_path->setText(path);
    ui->pushButton->setText("OK");
}
