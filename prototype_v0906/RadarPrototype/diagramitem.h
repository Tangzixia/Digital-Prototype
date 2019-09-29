#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H
#include <QMetaEnum>
#include <QDateTime>
#include <QGraphicsItem>
#include <QObject>
class Arrow;

/**
* @projectName   prototype_v0719
* @brief         这里就是悬浮框中雷达内部组件的图表类。
* @author        Antrn
* @date          2019-08-12
*/

class DiagramItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 15 };

    enum DiagramType { Comp1, Comp2, Comp3, Comp4, Comp5};
    Q_ENUM(DiagramType)
    DiagramType diagramType() const { return myDiagramType; }

    DiagramItem(DiagramType diagramType, QMenu *contextMenu, QGraphicsItem *parent = nullptr);

    void removeArrow(Arrow *arrow);
    void removeArrows();
    QPolygonF polygon() const { return myPolygon; }
    void addArrow(Arrow *arrow);
    QPixmap image() const;
    int type() const override { return Type;}
    int itemId;
    QString iconName;
    int init_pos_set=0;
    QRectF boundingRect();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

protected:
    // 鼠标事件，会导致一些奇怪的bug，一起运动
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
//    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
//    void KeyPressEvent(QKeyEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void focusInEvent(QFocusEvent *) override;
    void focusOutEvent(QFocusEvent *) override;

signals:


private:
    DiagramType myDiagramType;
    //自定义绘制多边形
    QPolygonF myPolygon;
    //保存的右键菜单，和菜单栏中的一致
    QMenu *myContextMenu;
    //存储箭头
    QList<Arrow *> arrows;

};

#endif // DIAGRAMITEM_H
