#ifndef DRAGLISTWIDGET_H
#define DRAGLISTWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QListWidget>
#include <QApplication>
#include "menu_iteamoperation.h"
#include<QDialog>
#include<QMap>
#include "mainwindow_radar.h"
//动态命名弹出窗口
#define SET_RADARNAME(name) radar##name
/**
* @projectName   prototype_v0719
* @brief         整体工程项目左边的拖拽列表：雷达/电子对抗设备，是以list的形式表现的。
* @author        Antrn
* @date          2019-08-12
*/
class DragListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit DragListWidget(QWidget *parent = nullptr);
    void addDragItem(QListWidgetItem*item);
    void addDragItem(const QString &label);

    static QString puzzleMimeType() {
     return QStringLiteral("image/x-module-piece");
    }

signals:
    //没用到
    void repaintWidget();
    void itemOperate(Menu_iteamOperation::OperateType,QString,QString newName="");
protected:
    //开始拖拽
    void startDrag(Qt::DropActions supportedActions);

   //从该部件中拖拽出去的操作.
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    //接受外来拖拽的操作.
//    void dragEnterEvent(QDragEnterEvent *event);
//    void dragMoveEvent(QDragMoveEvent *event);
//    void dropEvent(QDropEvent *event);

private:
    //记录拖拽的起点.
    QPoint m_dragPoint;
    //记录被拖拽的项.
    QListWidgetItem *m_dragItem;
    QListWidgetItem *addRadarButton;
    //0:name 1:itemImgPath
    void listItem_add(QString);
    void add_listItem(QString);
    QMap<QString,QListWidgetItem*> id_item;
    //listWidgetItem计数
    int id_inde=1;
     //是否新建了edit窗口
    QMap<QString,MainWindow_Radar*> newEditWindowList;
    //保存前一个被操作的item信息（名字 index）

    struct forRename
           {
                QString preName;
                int preIndex=-1;
                bool ifEnableChange=true;
           }forRename;
    //今后传值方便
    QString itemType="雷达";
public slots:
//操作item
    void itemOperateSlot(Menu_iteamOperation::OperateType, QString ,QString newName="");
    void renameSlot(QListWidgetItem* );


};

#endif // DRAGLISTWIDGET_H
