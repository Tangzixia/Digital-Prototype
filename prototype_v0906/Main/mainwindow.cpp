#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "realtimecurve.h"
#include <QLabel>
#include <QSettings>
#include <QWhatsThis>
/**
* @projectName   prototype_v0719
* @brief         整个程序的主窗口： 打开现有的工程时。
* @author        Antrn
* @date          2019-08-21
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("打开工程面板");
    showMaximized();
    init();
    // what's this?
    QAction *whatAction = QWhatsThis::createAction(this);
    ui->mainToolBar->addAction(whatAction);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    openGL = new OpenGL();
    openGL->setWindowTitle(tr("Open GL"));
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
