#ifndef MAINWINDOWNEWSCENE_H
#define MAINWINDOWNEWSCENE_H

#include <QGraphicsScene>
#include <QPaintEvent>
#include <QGraphicsItem>
#include "arrowitem.h"


class MainWindowNewScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MainWindowNewScene();
    ~MainWindowNewScene();
    QList<QGraphicsItem*> Item_List;
    QPointF computeFrom(const QRectF& from, const QPointF& to);
    void computeLine(QGraphicsItem*from, QGraphicsItem*to);
    QLineF computeMinLength(QList<QLineF> line_list);
    bool dragFlag;

    QList<QGraphicsItem*> connect_to_List;
    QList<QGraphicsItem*> connect_from_List;
    QList<ArrowItem*> delete_line_List;

signals:
    void signal_xy(double x,double y);
    void rateSignal(float rate);

private:
    QList<QPointF> ItemPoint_List;
    QList<QLineF*> Line_List;
    QList<ArrowItem*> graphicsLineItem_List;

    //临时保存起点终点
    QPointF sourcePoint;
    QPointF destPoint;

    QGraphicsItem *line_begin_item;

    QGraphicsItem *drag_item;


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
