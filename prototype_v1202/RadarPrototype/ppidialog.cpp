#include "ppidialog.h"
#include "ui_ppidialog.h"

PPIDialog::PPIDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PPIDialog)
{
    ui->setupUi(this);

}

PPIDialog::~PPIDialog()
{
    delete ui;
}


void PPIDialog::on_radioBtnShowOn_clicked(bool checked)
{
    if(checked){
        ui->widget->start();
    }else{
        ui->radioBtnShowOn->setChecked(true);
    }
}

void PPIDialog::on_radioBtnShowOFF_clicked(bool checked)
{
    if(checked){
        ui->widget->stop();
    }else{
        ui->radioBtnShowOFF->setChecked(true);
    }
}
