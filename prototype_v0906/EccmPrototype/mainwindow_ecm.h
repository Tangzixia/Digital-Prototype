#ifndef MAINWINDOW_ECM_H
#define MAINWINDOW_ECM_H

#include<QLabel>
#include <QMainWindow>

#include "GraphicsScenePublic.h"
//盗用
#include "diagramitem.h"
#include <QButtonGroup>
/**
* @projectName   prototype_v0906
* @brief         电子对抗主界面
* @author       Shao
* @date          2019-09-17
*/
namespace Ui {
class MainWindow_ECM;
}

class MainWindow_ECM : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_ECM(QWidget *parent = nullptr);
    //有参的构造方法
    explicit MainWindow_ECM(QString id , QWidget *parent = nullptr);
    ~MainWindow_ECM();

private:
    Ui::MainWindow_ECM *ui;
    QLabel *label_xy;
    //使用DiagramItem的构造方法所必须的
    QMenu *itemMenu;
    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    GraphicsScenePublic *sceneECM;
    void xy_show(double x, double y);
    void createCompBox();
    QWidget *createCellWidget(const QString &text,
                          DiagramItem::DiagramType type);
private slots:
    //实现点击左边算法组件改变右边Graphics状态
    void buttonGroupClicked(int id);
    //右边插入左边算法组件失去焦点
    void itemInserted(DiagramItem *item);
    void itemSelected(QGraphicsItem *item);
    void sceneScaleChanged(const QString &scale);

};

#endif // MAINWINDOW_ECM_H
