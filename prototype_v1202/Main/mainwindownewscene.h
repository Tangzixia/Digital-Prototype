#ifndef MAINWINDOWNEWSCENE_H
#define MAINWINDOWNEWSCENE_H
/**
* @projectName   prototype_v0719
* @brief         新建工程的主场景，用户配置雷达或者电子对抗，用户整体仿真。
* @author        Antrn
* @date          2019-08-12
*/
#include <QGraphicsScene>
#include <QPaintEvent>
#include <QGraphicsItem>
#include "radaritem.h"
#include "arrowitem.h"
//为了可以识别枚举值 OperateType
#include "menu_iteamoperation.h"

class MainWindowNewScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MainWindowNewScene();
    QList<QGraphicsItem*> Item_List;
    QPointF computeFrom(const QRectF& from, const QPointF& to);
    void computeLine(QGraphicsItem*from, QGraphicsItem*to);
    QLineF computeMinLength(QList<QLineF> line_list);
    bool dragFlag;

    QList<QGraphicsItem*> connect_to_List;
    QList<QGraphicsItem*> connect_from_List;
    QList<ArrowItem*> delete_line_List;
    QList<ArrowItem*> graphicsLineItem_List;

signals:
    void signal_xy(double x,double y);
    void rateSignal(float rate);

    //接收来至于子类的item操作(操作 ，itemId)
    void itemOperate(Menu_iteamOperation::OperateType,QString);
private:
    //临时保存起点终点
    QPointF sourcePoint;
    QPointF destPoint;
    QGraphicsItem *line_begin_item;
    QGraphicsItem *drag_item;
    ///保存拖入场景的雷达item（如果定义了电子对抗，目标环境，雷达的一个公共的，父组件就好了）
    QMap<QString,RadarItem*> id_gItem;
public slots:
    void itemOperateSlot(Menu_iteamOperation::OperateType,QString,QString newName="");
private slots:
    void LinePaint();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    //void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void paintEvent(QPaintEvent *);
};

#endif // MAINWINDOWNEWSCENE_H
