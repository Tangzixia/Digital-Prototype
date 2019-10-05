#include "algorithmcomp.h"
#include "parameditradardialog.h"
#include "ui_parameditradardialog.h"
#include <QDebug>
#include <QFileDialog>
#include <QHeaderView>
#include <QTableWidget>
ParamEditRadarDialog::ParamEditRadarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamEditRadarDialog)
{
    ui->setupUi(this);
    // 必须要设置有多少行，在ui中设置也行
//    ui->tableWidget_Param->setRowCount(20);
//    ui->tableWidget_Param->setColumnCount(3);
    ui->tableWidget_Param->setShowGrid(true);
    QTableWidgetItem *columnHeaderItem0 = ui->tableWidget_Param->horizontalHeaderItem(0); //获得水平方向表头的第一个Item对象
    columnHeaderItem0->setBackgroundColor(QColor(0,60,10)); //设置单元格背景颜色
    columnHeaderItem0->setTextColor(QColor(200,111,30)); //设置文字颜色
    ui->tableWidget_Param->setFont(QFont("Helvetica")); //设置字体
    //去掉水平滚动条
//    ui->tableWidget_Param->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //垂直滚动条按项移动
    ui->tableWidget_Param->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    //去掉自动滚动
//    ui->tableWidget_Param->setAutoScroll(false);
    ui->tableWidget_Param->horizontalHeader()->setStretchLastSection(true);
    // 选中单个单元格
    ui->tableWidget_Param->setSelectionBehavior(QAbstractItemView::SelectItems);
//    ui->tableWidget_Param->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui->tableWidget_Param->setSelectionMode(QAbstractItemView::MultiSelection);
    // 正常情况下是单选，按下Ctrl键后，可以多选
    ui->tableWidget_Param->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // 双击可修改
    ui->tableWidget_Param->setEditTriggers(QAbstractItemView::DoubleClicked);
}

ParamEditRadarDialog::~ParamEditRadarDialog()
{
    delete ui;
}

void ParamEditRadarDialog::on_pushButton_OK_clicked()
{
    ac.setDesc(ui->textEdit->toPlainText());
    mp.insert("ID", ui->lineEdit_ID->text());
    mp.insert("Name", ui->lineEdit_Name->text());
    mp.insert("Time", ui->dateTimeEdit->text());
    mp.insert("Path", ui->pushButton_Path->text());
    ac.setInfo(mp);
    QMap<QString, QMap<QString, QString> >pmap;
    QMap<QString, QString> m;
    qDebug() << "参数总行数row: " << row;
    for (int i=0;i<row;i++) {
        // WARNING 这里填写的内容一定不能为纯数字！！！，xue的教训，会直接造成写入成功，读取失败!
        QString  name = ui->tableWidget_Param->item(i, 0)->text();
        QString desc = ui->tableWidget_Param->item(i, 1)->text();
        QString value = ui->tableWidget_Param->item(i, 2)->text();
        qDebug() << name <<": " << desc <<"; " << value;
        m.clear();
        m.insert("describe", desc);
        m.insert("value", value);
        pmap.insert(name, m);
    }
    qDebug() << "参数列表的大小: " << pmap.size();
    ac.clearParam();
    ac.setParam(pmap);
    accept();
}

void ParamEditRadarDialog::on_pushButton_Cancel_clicked()
{
    close();
}

void ParamEditRadarDialog::on_pushButton_Path_clicked()
{
    QString dirpath = QDir::currentPath();
    QString path = QFileDialog::getExistingDirectory(this, "选择文件夹", QString(dirpath), QFileDialog::ShowDirsOnly);
    ui->pushButton_Path->setText(path);
}

void ParamEditRadarDialog::on_pushButton_Add_clicked()
{
    int r = ui->tableWidget_Param->rowCount();//获取表格中当前总行数
    ui->tableWidget_Param->setRowCount(r+1);//添加一行
    row = r;
    QTableWidgetItem *itemName, *itemDesc, *itemValue;
    itemName = new QTableWidgetItem;
    itemDesc = new QTableWidgetItem;
    itemValue = new QTableWidgetItem;
    itemName->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter); //居中
    itemDesc->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    itemValue->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    QString txt = QString("%1").arg("参数名");
    itemName->setText(txt);
    ui->tableWidget_Param->setItem(row, 0, itemName);
    txt = QString("%1").arg("参数描述");
    itemDesc->setText(txt);
    ui->tableWidget_Param->setItem(row, 1, itemDesc);
    txt = QString("%1").arg("参数值");
    itemValue->setText(txt);
    ui->tableWidget_Param->setItem(row, 2, itemValue);
    row++;
}

void ParamEditRadarDialog::on_pushButton_Del_clicked()
{
    QList<QTableWidgetItem*> items = ui->tableWidget_Param->selectedItems();
    int curRow;
    for (int i=0;i<items.count();i++) {
        curRow = ui->tableWidget_Param->row(items.at(i));//获取选中的行
        ui->tableWidget_Param->removeRow(curRow);
    }
    row-=items.count();
}
