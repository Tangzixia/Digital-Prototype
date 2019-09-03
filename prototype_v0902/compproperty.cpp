#include "compproperty.h"
#include "ui_compproperty.h"

CompProperty::CompProperty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompProperty)
{
    ui->setupUi(this);
}

CompProperty::~CompProperty()
{
    delete ui;
}
