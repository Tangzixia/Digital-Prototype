#include "mainwindownew.h"
#include "ui_mainwindownew.h"
#include "draglistwidget.h"

#include <QtDebug>
#include <QListWidgetItem>


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

    //this->setAttribute(Qt::WA_DeleteOnClose);
    graphicsScene=new MainWindowNewScene;
    // xy坐标标签
    label_xy=new QLabel(this);
    // 状态栏左边加入xy坐标
    ui->statusBar->addWidget(label_xy);
//    设置雷达组件类表初始状态隐藏
    ui->RadarListWidget->setVisible(false);

//    雷达组件列表按钮绑定事件
    connect(ui->RadarListButton,&QPushButton::clicked,this,[=](){
        bool flag = this->ui->RadarListWidget->isHidden();
        if(flag){
            this->ui->RadarListWidget->setVisible(true);
        }
        else{
            this->ui->RadarListWidget->setVisible(false);
        }
    });

//    当listwidget additem的时候通知父组件重绘（并没有卵用）
    connect(ui->RadarListWidget,&DragListWidget::repaintWidget,this,[=](){
//        this->ui->scrollAreaWidgetContents->repaint();
        this->repaint();
    });

    // 将主窗口中的场景设置为自定义的场景
    ui->graphicsView->setScene(graphicsScene);
    ui->graphicsView->setStyleSheet("border:0px;padding:0");
//     设置鼠标跟踪开启
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setAcceptDrops(true);

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
    rect = ui->graphicsView->geometry();
    qDebug() << "视图的位置1：" << rect << endl;
    rect.moveLeft(9);
    graphicsScene->setSceneRect(rect);
}


void MainWindowNew::xy_show(double x,double y)
{
    x-=9;
    y-=9;
    QString str=QString("x=%1,y=%2").arg(QString::number(x)).arg(QString::number(y));
    label_xy->setText(str);
}



