#include "ashowdialog.h"
#include "ui_ashowdialog.h"

AshowDialog::AshowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AshowDialog)
{
    ui->setupUi(this);
}

AshowDialog::~AshowDialog()
{
    delete ui;
}
