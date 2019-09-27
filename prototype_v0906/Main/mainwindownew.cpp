#include "mainwindownew.h"
#include "ui_mainwindownew.h"
#include "draglistwidget.h"

#include <QtDebug>
#include <QListWidgetItem>
#include <QMessageBox>


/**
* @projectName   prototype_v0719
* @brief         新建工程时的主窗口。
* @author        Antrn
* @date          2019-08-12
*/
MainWindowNew::MainWindowNew(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowNew)
{
    ui->setupUi(this);
    this->setStyleSheet("padding:0;margin:0");
    //this->setAttribute(Qt::WA_DeleteOnClose);
    graphicsScene=new MainWindowNewScene;
    // xy坐标标签
    label_xy=new QLabel(this);
    // 状态栏左边加入xy坐标
    ui->statusBar->addWidget(label_xy);
    // 将主窗口中的场景设置为自定义的场景
    ui->graphicsView->setScene(graphicsScene);
    ui->graphicsView->setStyleSheet("margin:0");
//     设置鼠标跟踪开启
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setAcceptDrops(true);
//不设置大小会出现拖动释放后位置跳变
    graphicsScene->setSceneRect(QRectF(0, 0, 5000, 5000));

    connect(graphicsScene,SIGNAL(signal_xy(double,double)),this,SLOT(xy_show(double,double)));
    this->showMaximized();
    this->setWindowTitle(tr("新建工程"));
    //可以设置在视图上使用鼠标拖出橡皮筋框选择图形项
    this->ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
}

MainWindowNew::~MainWindowNew()
{
    delete ui;
}

void MainWindowNew::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindowNew::resizeEvent(QResizeEvent *)
{
//    rect = ui->graphicsView->geometry();
//    qDebug() << "视图的位置1：" << rect << endl;
//    rect.moveLeft(9);
//    graphicsScene->setSceneRect(rect);
}


void MainWindowNew::xy_show(double x,double y)
{
//    x-=9;
//    y-=9;
    QString str=QString("x=%1,y=%2").arg(QString::number(x)).arg(QString::number(y));
    label_xy->setText(str);
}

void MainWindowNew::on_actionabout_DPSP_triggered()
{
    QMessageBox::about(this, tr("关于此软件 DPSP"),
                             tr("[EXAMPLE:]> The <b>DPSP(Digital-Prototype-Simulation-Platform)</b>"
                                " is an engineering level simulation tool for radar and electronic countermeasures equipment.."));
}

void MainWindowNew::on_actionexit_triggered()
{
    qApp->closeAllWindows();
}

void MainWindowNew::on_actio_leftDock_triggered()
{
    if(ui->dockWidget_left->isHidden()){
        ui->dockWidget_left->show();
    }else{
        ui->dockWidget_left->hide();
    }
}
