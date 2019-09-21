#ifndef MAINWINDOWNEW_H
#define MAINWINDOWNEW_H
/**
* @projectName   prototype_v0719
* @brief         新建工程时的主窗口。
* @author        Antrn
* @date          2019-08-12
*/
#include "mainwindownewscene.h"

#include <QMainWindow>
#include <QLabel>
namespace Ui {
class MainWindowNew;
}

class MainWindowNew : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowNew(QWidget *parent = nullptr);
    ~MainWindowNew();
    QLabel *label_xy;
    MainWindowNewScene *graphicsScene;

protected:
    void closeEvent(QCloseEvent *event);
    //设置监听窗口大小，使得视图和场景每次窗口缩放都左上角对应
    void resizeEvent(QResizeEvent *event);

private slots:
    void xy_show(double x,double y);

    void on_actionabout_DPSP_triggered();

    void on_actionexit_triggered();

    void on_actio_leftDock_triggered();

private:
    Ui::MainWindowNew *ui;
    QRect rect;
};

#endif // MAINWINDOWNEW_H
