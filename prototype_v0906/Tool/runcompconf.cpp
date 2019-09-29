#include "qudong.h"
#include "runcompconf.h"
#include "ui_runcompconf.h"
#include <QDebug>
#include <QTimer>
RunCompConf::RunCompConf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunCompConf)
{
    ui->setupUi(this);
    connect(ui->btn_start, &QPushButton::clicked, this, &RunCompConf::On_start);
    connect(this,SIGNAL(startRun()),this,SLOT(On_start()));
    connect(this,SIGNAL(rateSignal(float)),this,SLOT(On_rateSignal(float)));
    connect(this,SIGNAL(overRun()),this,SLOT(On_over()));
    connect(ui->btn_pause,SIGNAL(clicked()),this,SLOT(On_pause()));
    connect(ui->btn_stop,SIGNAL(clicked()),this,SLOT(On_over()));

    ui->run_progressBar->setValue(0);
    ui->btn_stop->setEnabled(false);
    ui->btn_pause->setEnabled(false);
}

RunCompConf::~RunCompConf()
{
    delete ui;
}

void RunCompConf::On_start()
{
    qDebug() << "Start";
    ui->btn_stop->setEnabled(true);
    ui->btn_pause->setEnabled(true);
    ui->run_progressBar->setValue(0);

    QTimer::singleShot( 0, this, [=](){
        emit rateSignal(10);}
    );
    // WARNING 执行指令，这个指令记得及时修改
    QuDong::startRun("/home/a913/Qt_project/20190910/prototype_v0906/Projects/test/code/radar_test.cpp", "/home/a913/Qt_project/20190910/prototype_v0906/Projects/test/code/radar_test.out");
    QTimer::singleShot( 1000, this, [=](){
        emit rateSignal(70);}
    );
    QTimer::singleShot( 3000, this, [=](){
        emit rateSignal(100);}
    );
    emit overRun();
}

void RunCompConf::On_pause()
{
    qDebug() << "Pause";
    // TODO 能够将进度暂停
}

void RunCompConf::On_over()
{
    qDebug() << "Done";
    // TODO 能够将进程杀死
}

void RunCompConf::On_rateSignal(float rate)
{
    qDebug() << rate;
    ui->run_progressBar->setValue(int(rate));
}

