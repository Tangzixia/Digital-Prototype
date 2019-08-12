#include "diagramitem.h"
#include "arrow.h"
#include<QGraphicsScene>
#include <QPainter>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QKeyEvent>
#include <QGraphicsBlurEffect>
/**
* @projectName   prototype_v0719
* @brief         这里就是悬浮框中雷达内部组件的图表类。
* @author        Antrn
* @date          2019-08-12
*/
DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu,
             QGraphicsItem *parent): QGraphicsPolygonItem(parent)
{
    myDiagramType = diagramType;
    myContextMenu = contextMenu;

    QPainterPath path;
    switch (myDiagramType) {
        //立起的正方形
        case StartEnd:
            path.moveTo(200, 50);
            path.arcTo(150, 0, 50, 50, 0, 90);
            path.arcTo(50, 0, 50, 50, 90, 90);
            path.arcTo(50, 50, 50, 50, 180, 90);
            path.arcTo(150, 50, 50, 50, 270, 90);
            path.lineTo(200, 25);
            myPolygon = path.toFillPolygon();
            break;
        //正方形
        case Conditional:
            myPolygon << QPointF(-100, 0) << QPointF(0, 100)
                      << QPointF(100, 0) << QPointF(0, -100)
                      << QPointF(-100, 0);
            break;
        //菱形1
        case Step:
            myPolygon << QPointF(-100, -100) << QPointF(100, -100)
                      << QPointF(100, 100) << QPointF(-100, 100)
                      << QPointF(-100, -100);
            break;
        //默认 菱形2
        default:
            myPolygon << QPointF(-120, -80) << QPointF(-70, 80)
                      << QPointF(120, 80) << QPointF(70, -80)
                      << QPointF(-120, -80);
            break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
    //坐标变化
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
void DiagramItem::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void DiagramItem::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void DiagramItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}

QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);
    painter.setRenderHint(QPainter::Antialiasing, true);
    return pixmap;
}

//void DiagramItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
//{
//    setCursor(Qt::ArrowCursor);
//}

//void DiagramItem::mousePressEvent(QGraphicsSceneMouseEvent *)
//{
//    setFocus();
    //TODO 当移动的时候改变光标
//    setCursor(Qt::ClosedHandCursor);
//}

//void DiagramItem::KeyPressEvent(QKeyEvent *event)
//{
//    if(event->key() == Qt::Key_Down){
//        moveBy(0, 10);
          //？为什么不是想象中的移动图形项，而是scene
//    }
//    switch (event->key()) {
//        case Qt::Key_1:{
//            QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect;
//            blurEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
//            blurEffect->setBlurRadius(8);
//            setGraphicsEffect(blurEffect);
//            break;
//        }
//        case Qt::Key_5:{
//            graphicsEffect()->setEnabled(false);
//            break;
//        }
//    }
//}

//void DiagramItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
//{
//    setCursor(Qt::OpenHandCursor);
//    setToolTip("component");
//}

//右键菜单，保持一致
void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    //检测位置发生变化
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
    }

    return value;
}
