#include "graphicsscenepublic.h"
#include "arrow.h"
#include <QDebug>
GraphicsScenePublic::GraphicsScenePublic(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = DiagramItem::Comp1;
    line = nullptr;
}
//重写鼠标移动事件方法，获取XY坐标
void GraphicsScenePublic::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
    emit signal_xy(mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
    if (myMode == InsertLine && line != nullptr) {
       // QLineF newLine(line->line().p1(), mouseEvent->scenePos());
       // line->setLine(newLine);
    }else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
        update();
    }

}

void GraphicsScenePublic::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//    if (line != nullptr && myMode == InsertLine) {
//        //找到线段的开始点p1处叠加的所有Items
//        QList<QGraphicsItem *> startItems = items(line->line().p1());
//        //已经存在line在最上层
//        if (startItems.count() && startItems.first() == line)
//            startItems.removeFirst();
//        QList<QGraphicsItem *> endItems = items(line->line().p2());
//        //防止点线段
//        if (endItems.count() && endItems.first() == line)
//            endItems.removeFirst();
//        //删除press时新建的线段
//        removeItem(line);
//        delete line;

//        if (startItems.count() > 0 && endItems.count() > 0 &&
//            startItems.first()->type() == DiagramItem::Type &&
//            endItems.first()->type() == DiagramItem::Type &&
//            startItems.first() != endItems.first()) {
//            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
//            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
//            Arrow *arrow = new Arrow(startItem, endItem);
////            arrow->setColor(myLineColor);
////            arrow->itemId = generateUniqueid();
//            startItem->addArrow(arrow);
//            endItem->addArrow(arrow);
//            arrow->setZValue(-1000.0);
//            addItem(arrow);
//            arrow->updatePosition();

////            modifyXmlArrows(arrow, startItem, endItem);
//        }
//    }
//    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphicsScenePublic::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    //左键
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    DiagramItem *item;
    switch (myMode) {
        case InsertItem:{
        qDebug() << "ecm--InsertItem";
            item = new DiagramItem(myItemType, myItemMenu);
//            item->(myItemColor);
//            item->setBrush(myItemColor);
            //作用是？？
            item->itemId = generateUniqueid();
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);
//            modifyXmlItems(mouseEvent->scenePos(), item);
            break;
        }
        case InsertLine:
         qDebug() << "ecm--Insert";
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
//            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;
        case InsertText:
                qDebug() << "ecm--InsertText";
//            textItem = new DiagramTextItem();
//            textItem->setFont(myFont);
//            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
//            textItem->setZValue(1000.0);
//            connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
//                    this, SLOT(editorLostFocus(DiagramTextItem*)));
//            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
//                    this, SIGNAL(itemSelected(QGraphicsItem*)));
//            addItem(textItem);
//            textItem->setDefaultTextColor(myTextColor);

//            textItem->setPos(mouseEvent->scenePos());
//            emit textInserted(textItem);
           break;
        default:
            ;
    }
     qDebug() << "mousePressEvent";
    QGraphicsScene::mousePressEvent(mouseEvent);
    qDebug() << "mousePressEvent after";
}

void GraphicsScenePublic::setMode(Mode mode)
{
    myMode = mode;
}

void GraphicsScenePublic::setItemType(DiagramItem::DiagramType itemType){
    myItemType=itemType;
}
