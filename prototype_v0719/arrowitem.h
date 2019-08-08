#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPen>

class ArrowItem : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    const static float LINE_WIDTH;
    const static float ARROW_SIZE;

    ArrowItem();
    ~ArrowItem();
    void setData(const QPointF& from,const QPointF& to);
    void setLineItem(QGraphicsItem*begin,QGraphicsItem*end);
    QGraphicsItem*getBeginItem();
    QGraphicsItem*getEndItem();

    QPainterPath shape()const;//重写shape函数
    QRectF boundingRect()const;
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* style, QWidget* widget);

public slots:
    void delete_item();


protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    bool mValid;
    QPointF mFrom;
    QPointF mTo;

    QPointF mP1;
    QPointF mP2;

    QGraphicsItem*line_begin_item;
    QGraphicsItem*line_end_item;


    int ellipse_r = 0;
    QPen pen;
    QBrush brush;
};

#endif // ARROWITEM_H
