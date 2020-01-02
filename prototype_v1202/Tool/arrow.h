#ifndef ARROW_H
#define ARROW_H

#include "diagramitem.h"

#include <QPainterPath>
#include <QGraphicsItem>
#include <QObject>
/**
* @projectName   prototype_v0719
* @brief         用于编辑雷达页面的组件元素之间链接的箭头类。
* @author        Antrn
* @date          2019-08-12
*/
class Arrow : public QGraphicsLineItem{
public:
    enum { Type = UserType + 4 };
    QString itemId;
    Arrow(DiagramItem *startItem, DiagramItem *endItem,
      QGraphicsItem *parent = nullptr);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor(const QColor &color) { myColor = color; }
    QString getColor(){return myColor.name();}
    DiagramItem *startItem() const { return myStartItem; }
    DiagramItem *endItem() const { return myEndItem; }
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    DiagramItem *myStartItem;
    DiagramItem *myEndItem;
    QColor myColor;
    //手动画出来的箭头
    QPolygonF arrowHead;
};
#endif // ARROW_H
