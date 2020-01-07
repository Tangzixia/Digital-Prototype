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

//    enum DiagramType { Comp1, Comp2, Comp3, Comp4, Comp5};
//    Q_ENUM(DiagramType)

//    DiagramType diagramType() const { return myDiagramType; }

//    DiagramItem(DiagramType diagramType, QMenu *contextMenu, QGraphicsItem *parent = nullptr);
    DiagramItem(QString iconName, QMenu *contextMenu, QString radar_id, QGraphicsItem *parent = nullptr);
    void removeArrow(Arrow *arrow);
    void removeArrows();
    QPolygonF polygon() const { return myPolygon; }
    void addArrow(Arrow *arrow);
//    QPixmap image() const;
    int type() const override { return Type;}
    // 一开始的方案：使用int唯一id
//    int itemId;
    // 后来使用uuid代替int形式
    QString itemSuuid;
    QString iconName; //组件小图标
    int init_pos_set=0;  // 是否初始化的标志
    QRectF boundingRect();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

    QString getIconName() const;
    void setIconName(const QString &value);

    QString getRadar_id() const;

    void setRadar_id(const QString &value);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void focusInEvent(QFocusEvent *) override;
    void focusOutEvent(QFocusEvent *) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void showItemsProperties(QString sid);

private:
//    DiagramType myDiagramType;
    //自定义绘制多边形
    QPolygonF myPolygon;
    //保存的右键菜单，和菜单栏中的一致
    QMenu *myContextMenu;
    //存储箭头
    QList<Arrow *> arrows;
    // 记住这个组件是属于哪个雷达的
    QString radar_id;
};

#endif // DIAGRAMITEM_H
