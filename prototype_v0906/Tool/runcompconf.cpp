#include "runcompconf.h"
#include "ui_runcompconf.h"

RunCompConf::RunCompConf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunCompConf)
{
    ui->setupUi(this);
}

RunCompConf::~RunCompConf()
{
    delete ui;
}
