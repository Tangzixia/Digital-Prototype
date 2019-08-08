#ifndef MAINWINDOWNEW_H
#define MAINWINDOWNEW_H

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

private:
    Ui::MainWindowNew *ui;
    QRect rect;
};

#endif // MAINWINDOWNEW_H
