#include "radarscene.h"
#include "arrow.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
/**
* @projectName   prototype_v0719
* @brief         编辑雷达页面的自定义场景类，上面绘制设计雷达的组件链接。
* @author        Antrn
* @date          2019-08-12
*/
RadarScene::RadarScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    //默认Step
    myItemType = DiagramItem::Comp1;
    line = nullptr;
    textItem = nullptr;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
}

void RadarScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    //只过滤箭头设置颜色
    if (isItemChange(Arrow::Type)) {
        //一次只能设置一个
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}

void RadarScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        //一次只能设置一个
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}

void RadarScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}

void RadarScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}


void RadarScene::setMode(Mode mode)
{
    myMode = mode;
}

void RadarScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}


void RadarScene::editorLostFocus(DiagramTextItem *item)
{
    //获取光标
    QTextCursor cursor = item->textCursor();
    //取消选中
    cursor.clearSelection();
    item->setTextCursor(cursor);
    //如果里面没有内容，就自动删除吧
    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        //计划删除此对象
        item->deleteLater();
    }
}

void RadarScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //左键
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    DiagramItem *item;
    switch (myMode) {
        case InsertItem:
            item = new DiagramItem(myItemType, myItemMenu);
            item->setBrush(myItemColor);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);
            break;

        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;
        case InsertText:
            textItem = new DiagramTextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                    this, SLOT(editorLostFocus(DiagramTextItem*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);

            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
            break;
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void RadarScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    emit signal_xy(mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
    if (myMode == InsertLine && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void RadarScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != nullptr && myMode == InsertLine) {
        //找到线段的开始点p1处叠加的所有Items
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        //已经存在line在最上层
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        //防止重复划线
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();
        //删除过程中出现的线段，改为绘制箭头
        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }
    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool RadarScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}
