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
//QList<MainWindow_Radar *> MainWindowNew::main_radar_list;
MainWindowNew::MainWindowNew(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowNew)
{
    ui->setupUi(this);
    // 设置对话框icon
    this->setWindowIcon(QIcon(":/img/fangzhen.png"));
    this->setStyleSheet("padding:0;margin:0");
    this->setAttribute(Qt::WA_DeleteOnClose);
    graphicsScene=new MainWindowNewScene();
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
//    默认 定位到雷达
    ui->toolBox_left->setCurrentIndex(0);
//不设置大小会出现拖动释放后位置跳变
    graphicsScene->setSceneRect(QRectF(0, 0, 5000, 5000));

    connect(graphicsScene,SIGNAL(signal_xy(double,double)),this,SLOT(xy_show(double,double)));
    this->showMaximized();
    this->setWindowTitle(tr("新建工程"));
    //可以设置在视图上使用鼠标拖出橡皮筋框选择图形项
    this->ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
//      接收来之左边item的信号
    connect(this->ui->listWidget_rader,&DragListWidget::itemOperate,this,&MainWindowNew::itemOperateSlot);
//  接收来之scene的item的信号
    connect (graphicsScene,&MainWindowNewScene::itemOperate,[=](Menu_iteamOperation::OperateType operate,QString id){
        this->itemOperateSlot(operate,id);
    });
//    默认属性窗口隐藏
    ui->dockWidget_property->hide();
}

MainWindowNew::~MainWindowNew()
{
    delete ui;
}

void MainWindowNew::closeEvent(QCloseEvent *event)
{
//    for(MainWindow_Radar *m : main_radar_list){
//        m->close();
//    }
//    main_radar_list.clear();

    if(ui->listWidget_rader->closeDragListWidget()){
        event->accept();
    }
    else { 
        event->ignore();
    }
}

void MainWindowNew::resizeEvent(QResizeEvent *)
{
//    rect = ui->graphicsView->geometry();
//    qDebug() << "视图的位置1：" << rect << endl;
//    rect.moveLeft(9);
//    graphicsScene->setSceneRect(rect);
}
//坐标跟踪
void MainWindowNew::xy_show(double x,double y)
{
//    x-=9;
//    y-=9;
    QString str=QString("x=%1,y=%2").arg(QString::number(x)).arg(QString::number(y));
    label_xy->setText(str);
}
//关于我们
void MainWindowNew::on_actionabout_DPSP_triggered()
{
    QMessageBox::about(this, tr("关于此软件 DPSP"),
                             tr("[EXAMPLE:]> The <b>DPSP(Digital-Prototype-Simulation-Platform)</b>"
                                " is an engineering level simulation tool for radar and electronic countermeasures equipment.."));
}
//退出
void MainWindowNew::on_actionexit_triggered()
{
    qApp->closeAllWindows();
}

//左边窗口显示
void MainWindowNew::on_actio_leftDock_triggered()
{
    if(ui->dockWidget_left->isHidden()){
        ui->dockWidget_left->show();
    }else{
        ui->dockWidget_left->hide();
    }
}
//下发操作指令
 void MainWindowNew::itemOperateSlot(Menu_iteamOperation::OperateType operateType, QString id,QString newName){
    qDebug()<<"mainWindowsNew::"<<operateType<<","<<id<<"new name:"<<newName;
    if(operateType==Menu_iteamOperation::del){
        this->graphicsScene->itemOperateSlot(Menu_iteamOperation::del,id);
        ui->listWidget_rader->itemOperateSlot(Menu_iteamOperation::del,id);
    }else if(operateType==Menu_iteamOperation::property){
        //弹出属性窗
       ui->dockWidget_property->show();
       ui->actionProperty->setChecked(true);
        //属性窗口内容显示
        propertyContent(id);
    }
    else if(operateType==Menu_iteamOperation::edit){
       //新建或者提升组件窗口（从left的item来，他在就在，他死就死）
        qDebug()<<"edit来了";
        ui->listWidget_rader->itemOperateSlot(Menu_iteamOperation::edit,id);
    }else if(operateType==Menu_iteamOperation::rename){
        this->graphicsScene->itemOperateSlot(Menu_iteamOperation::rename,id,newName);
        ui->listWidget_rader->itemOperateSlot(Menu_iteamOperation::rename,id,newName);
    }
 }
  //属性窗口内容显示
 void MainWindowNew::propertyContent(QString id){
     //读取XMl写入窗口
    qDebug()<<"交给你了,组件name:"<<id<<"留下了";
     //
 }

 void MainWindowNew::on_actionProperty_triggered(bool checked)
{
    if(checked){
        ui->dockWidget_property->show();
    }else{
        ui->dockWidget_property->hide();
    }
}
