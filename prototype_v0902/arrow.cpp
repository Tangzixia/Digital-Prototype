#include "arrow.h"
#include <math.h>
#include <QPen>
#include <QPainter>

const qreal Pi = 3.14;

/**
* @projectName   prototype_v0719
* @brief         用于编辑雷达页面的组件元素之间链接的箭头类。
* @author        Antrn
* @date          2019-08-12
*/

/*
* TODO 选中箭头，按键盘delete可以删除箭头或者item组件
*/

Arrow::Arrow(DiagramItem *startItem, DiagramItem *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    myStartItem = startItem;
    myEndItem = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::black;
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void Arrow::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    //碰撞检测，不画箭头
    if (myStartItem->collidesWithItem(myEndItem))
        return;

    QPen myPen = pen();
    //myColor 成员变量，记录颜色
    myPen.setColor(myColor);
    qreal arrowSize = 20;
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(myPen);
    painter->setBrush(myColor);

    //传入在视图中的两个坐标为起始和终点，分别为中点
    QLineF centerLine(myStartItem->pos(), myEndItem->pos());
    QPolygonF endPolygon = myEndItem->polygon();
    QPointF p1 = endPolygon.first() + myEndItem->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    //从第二个点开始遍历
    for (int i = 1; i < endPolygon.count(); ++i) {
        p2 = endPolygon.at(i) + myEndItem->pos();
        polyLine = QLineF(p1, p2);
        //判断两个item的连线和此条边界线是否相交，记录相交类型
        QLineF::IntersectType intersectType =
            polyLine.intersect(centerLine, &intersectPoint);
        //在线段内直接相交
        if (intersectType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, myStartItem->pos()));


    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

    QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                    cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                    cos(angle + Pi - Pi / 3) * arrowSize);

    arrowHead.clear();
    arrowHead << line().p1() << arrowP1 << arrowP2;

    painter->drawLine(line());
    painter->drawPolygon(arrowHead);
    //箭头被选中
    if (isSelected()) {
        painter->setPen(QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();
        //往右平移4
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        //再往左平移8，这样两边各为4
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}
