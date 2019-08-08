#include "mainwindow_radar.h"
#include "radarscene.h"
#include "ui_mainwindow_radar.h"

#include <QCloseEvent>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>

MainWindow_Radar::MainWindow_Radar(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_Radar)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    showMaximized();
    ui->actionCom_list->setCheckable(true);
    ui->actionCom_list->setChecked(true);

    // 新建场景
    RadarScene *graphicsScene = new RadarScene;
    // xy坐标标签
    label_xy=new QLabel(this);
    connect(graphicsScene,SIGNAL(signal_xy(double,double)),this,SLOT(xy_show(double,double)));
    // 将主窗口中的场景设置为自定义的场景
    ui->graphicsView->setScene(graphicsScene);
    // 设置鼠标跟踪开启
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setAcceptDrops(true);

}

MainWindow_Radar::~MainWindow_Radar()
{
    delete ui;
}

void MainWindow_Radar::closeEvent(QCloseEvent *event)
{
    int ret1 = QMessageBox::question(this, tr("确认"), tr("确定退出雷达编辑?"), QMessageBox::Yes, QMessageBox::No);
    if(ret1 == QMessageBox::Yes){
        qDebug() << tr("close radar window");
        //TODO 保存雷达配置
        event->accept();
    }else{
        event->ignore();
    }

}

void MainWindow_Radar::on_actionCom_list_triggered()
{
    //显示工具箱（文本编辑器）
    if(ui->dockCompList->isVisible()){
        ui->dockCompList->close();
    }else{
        ui->dockCompList->show();
    }
}

void MainWindow_Radar::xy_show(double x, double y)
{
    x-=9;
    y-=9;
    //(9,9,xx,xx)要减去9
    //qDebug() << ui->graphicsView->geometry();
    //qDebug() << this->graphicsScene->width() << " ; " << this->graphicsScene->height() ;
    QString str=QString("x=%1,y=%2").arg(QString::number(x)).arg(QString::number(y));
    label_xy->setText(str);
}
