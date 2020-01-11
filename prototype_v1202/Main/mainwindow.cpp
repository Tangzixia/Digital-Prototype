#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "realtimecurve.h"
#include "draglistwidget.h"
#include "mainwindownew.h"
#include <QLabel>
#include <QSettings>
#include <QWhatsThis>
#include <QDebug>
/**
* @projectName   prototype_v0719
* @brief         整个程序的主窗口： 打开现有的工程时。
* @author        Antrn
* @date          2019-08-21
*/
MainWindow::MainWindow(QString title, QString dpsp_path, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    w_title(title),
    p_dpsp_path(dpsp_path)
{
    ui->setupUi(this);
    setWindowTitle(w_title);
    initOpenGL();
    // 第一次尝试 what's this?
    QAction *whatAction = QWhatsThis::createAction(this);
    ui->mainToolBar->addAction(whatAction);

    //  接收来之左边item的信号
    connect(this->ui->listWidget_rader,&DragListWidget::itemOperate, this, &MainWindow::itemOperateSlot);

    // 读取工程文件
    if(loadProject(p_dpsp_path)){
        qDebug() << "加载成功！" << info.toStdMap() << comps.toStdMap();
        foreach(const QString &id, comps.keys()){
            // 获取设备的name
            QString newName = comps.value(id);
            while(ui->listWidget_rader->getId_item().contains(newName)){
                qDebug() << "名称"<< newName << "已经存在，换一个";
                // TODO 更改名字
            }
            // 从堆中新建对象
            QListWidgetItem *item = new QListWidgetItem;
            ui->listWidget_rader->insert2id_item(newName,item);
            QString path;
            if(id.startsWith("radar"))
                path=":/img/radar.png";
            else if(id.startsWith("eccm"))
                path=":/img/ECM.png";
            else if(id.startsWith("target"))
                path=":/img/object.png";
            item->setIcon(QIcon(path));
            item->setText(tr(newName.toUtf8().data()));

            //这里的用户角色存储用户数据（和拖入场景有关）
            item->setData(Qt::UserRole, QPixmap(path));
            item->setData(Qt::UserRole+1, newName);
            item->setData(Qt::UserRole+2, ui->listWidget_rader->getItemType());
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
            ui->listWidget_rader->addDragItem(item);
        }
        showMaximized();
    }else{
        qDebug() << "加载失败！";
        qApp->closeAllWindows();
    }
    graphicsScene = new MainWindowNewScene;
    // 将窗口中的场景设置为自定义的场景
    ui->graphicsView->setScene(graphicsScene);
    // 设置鼠标跟踪开启
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setAcceptDrops(true);
    //不设置大小会出现拖动释放后位置跳变
    graphicsScene->setSceneRect(QRectF(0, 0, 5000, 5000));
    // 接收来之scene的item的信号
    connect (graphicsScene,&MainWindowNewScene::itemOperate,[=](Menu_iteamOperation::OperateType operate,QString id){
        this->itemOperateSlot(operate,id);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initOpenGL()
{
    openGL = new OpenGL();
    openGL->setWindowTitle(tr("OpenGL"));
}

/**
 * @brief 加载项目工程文件，读取
 * @param pro_path 项目文件.dpsp路径
 * @return 是否读取成功
 */
bool MainWindow::loadProject(QString pro_path)
{
#if 0
    // 读取文本文件方式
    QFile pro(pro_path);
    if(pro.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&pro);
        // 设置编码格式 不然中文乱码
        in.setCodec("UTF-8");
        QString line = in.readLine();
        while (!line.isNull()) {
//          qDebug() << line;
          line = in.readLine();
        }
        return true;
    }else{
        return false;
    }
#endif
    // 传入的是引用，相当于地址，读取完后会发生改变
    return Utils::readProjectXml(pro_path, info, comps);
}

void MainWindow::on_actionOpenGL_triggered()
{
    openGL->exec();
}
//读取设置
void MainWindow::readSettings()
{
    QSettings setting("DPSP_MainOpen", "Digit-pro");
    QPoint pos = setting.value("pos", QPoint(200, 200)).toPoint();
    QSize size = setting.value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
}

//写入设置
void MainWindow::writeSettings()
{
    QSettings setting("DPSP_MainOpen", "Digit-pro");
    setting.setValue("pos", pos());
    setting.setValue("size", size());
}
//关闭主窗口时的动作，保存应用设置
void MainWindow::closeEvent(QCloseEvent *event)
{
    // TODO 先关闭其他子窗口

    // TODO 保存设置
    writeSettings();
    event->accept();
}

/**
 * @brief MainWindow::itemOperateSlot
 * @param newName
 */
void MainWindow::itemOperateSlot(Menu_iteamOperation::OperateType operateType, QString id, QString newName)
{
    qDebug()<<"mainWindowsNew::"<<operateType<<","<<id<<"new name:"<<newName;
    if(operateType==Menu_iteamOperation::del){
        this->graphicsScene->itemOperateSlot(Menu_iteamOperation::del,id);
        ui->listWidget_rader->itemOperateSlot(Menu_iteamOperation::del,id);
    }else if(operateType==Menu_iteamOperation::property){
        qDebug() << "属性窗口先不管";
    }
    else if(operateType==Menu_iteamOperation::edit){
       // 新建或者提升组件窗口（从left的item来，他在就在，他死就死）
        qDebug()<<"edit来了";
        ui->listWidget_rader->itemOperateSlot(Menu_iteamOperation::edit,id);
    }else if(operateType==Menu_iteamOperation::rename){
        this->graphicsScene->itemOperateSlot(Menu_iteamOperation::rename,id,newName);
        ui->listWidget_rader->itemOperateSlot(Menu_iteamOperation::rename,id,newName);
    }else if(operateType==Menu_iteamOperation::ppi){
        this->graphicsScene->itemOperateSlot(Menu_iteamOperation::ppi,id,newName);
        ui->listWidget_rader->itemOperateSlot(Menu_iteamOperation::ppi,id,newName);
    }else if(operateType==Menu_iteamOperation::ashow){
            this->graphicsScene->itemOperateSlot(Menu_iteamOperation::ashow,id,newName);
            ui->listWidget_rader->itemOperateSlot(Menu_iteamOperation::ashow,id,newName);
        }
}

void MainWindow::on_actionexit_triggered()
{
    // 软件退出
    qApp->closeAllWindows();
}

void MainWindow::on_actionCurve_triggered()
{
    RealTimeCurve *rtc = new RealTimeCurve;
    rtc->show();
}

void MainWindow::on_actionToolsBar_triggered()
{
    //显示工具栏
    if(ui->mainToolBar->isHidden()){
        ui->mainToolBar->show();
    }else{
        ui->mainToolBar->hide();
    }
}

void MainWindow::on_actionEquip_triggered()
{
    //显示装备栏
    if(ui->dockWidget_left->isVisible()){
        ui->dockWidget_left->close();
    }else{
        ui->dockWidget_left->show();
    }
}
