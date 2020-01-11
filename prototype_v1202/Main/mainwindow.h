#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/**
* @projectName   prototype_v0719
* @brief         整个程序的主窗口：打开现有的dpsp工程时。
* @author        Antrn
* @date          2019-08-12
*/
#include "mainwindownewscene.h"
#include "menu_iteamoperation.h"
#include "opengl.h"
#include <QCloseEvent>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString title, QString dpsp_path, QWidget *parent = nullptr);
    ~MainWindow();
    void initOpenGL();
    // 加载工程文件
    bool loadProject(QString pro_path);
    // 关闭窗口事件
    void closeEvent(QCloseEvent *event);

private slots:

    //向子类发送itemoperate，从mainwindownew粘贴过来的
    void itemOperateSlot(Menu_iteamOperation::OperateType, QString id, QString newName="");

    void on_actionOpenGL_triggered();
    void on_actionexit_triggered();
    void on_actionCurve_triggered();
    void on_actionToolsBar_triggered();
    void on_actionEquip_triggered();
private:
    Ui::MainWindow *ui;
    //为了在itemOperateSlot中可以向graphicsScene中的控件传递信息
    MainWindowNewScene *graphicsScene;
    OpenGL *openGL;
    QString w_title;
    QString p_dpsp_path;
    // 工程的配置文件信息
    QMap<QString, QString> info;
    // 工程中包含的其他元素，将其id,name保存
    QMap<QString, QString> comps;
    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_H
