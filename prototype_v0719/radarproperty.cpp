#include "radarproperty.h"
#include "ui_radarproperty.h"

#include <QDialog>

RadarProperty::RadarProperty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RadarProperty)
{
    ui->setupUi(this);
}

RadarProperty::~RadarProperty()
{
    delete ui;
}
