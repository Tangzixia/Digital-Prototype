#include "radarscene.h"
#include "arrow.h"
#include <QDebug>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QMetaEnum>
#include "diagramitem.h"
/**
* @projectName   prototype_v0719
* @brief         编辑雷达页面的自定义场景类，上面绘制设计雷达的组件链接。
* @author        Antrn
* @date          2019-08-12
*/
//Q_DECLARE_METATYPE(DiagramItem)
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

    //添加处理指令即xml头部说明和场景的属性
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("components");
    doc.appendChild(root);
    QDomAttr scene_w = doc.createAttribute("width");
    QDomAttr scene_h = doc.createAttribute("height");
    scene_w.setValue(QString::number(width()));
    scene_h.setValue(QString::number(height()));
    root.setAttributeNode(scene_w);
    root.setAttributeNode(scene_h);
    Arrs = doc.createElement("Arrs");
    root.appendChild(Arrs);
    Items = doc.createElement("Items");
    root.appendChild(Items);
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

void RadarScene::modifyXmlItems(QPointF pos, DiagramItem *item)
{

    // 使用下面这两行使得Enum转String怎么都不行
    // QMetaEnum m = QMetaEnum::fromType<DiagramItem::DiagramType>();
    // QDomElement comp = doc.createElement(m.valueToKey(myItemType));
    QDomElement comp;
    switch (item->diagramType()) {
        case DiagramItem::DiagramType::Comp1:
            comp = doc.createElement("comp_1");
            break;
        case DiagramItem::DiagramType::Comp2:
            comp = doc.createElement("comp_2");
            break;
        case DiagramItem::DiagramType::Comp4:
            comp = doc.createElement("comp_4");
            break;
        default:
            break;
    }
    QDomElement color = doc.createElement("color");
    QDomAttr posx = doc.createAttribute("pos_x");
    QDomAttr posy = doc.createAttribute("pos_y");
    QDomAttr id = doc.createAttribute("id");
    posx.setValue(QString::number(pos.x()));
    posy.setValue(QString::number(pos.y()));
    id.setValue(QString::number(item->itemId));
    comp.setAttributeNode(posx);
    comp.setAttributeNode(posy);
    comp.setAttributeNode(id);
    QDomText c = doc.createTextNode(itemColor().name());
    color.appendChild(c);
    comp.appendChild(color);
    Items.appendChild(comp);
}

void RadarScene::updateXmlItemsPos(QPointF pos, DiagramItem *item)
{
    QDomNode node = doc.elementsByTagName("Items").at(0).firstChild();
    QDomElement elem;
    // 遍历items标签找到
    while(!node.isNull()){
        if(node.isElement()){
             elem = node.toElement();
             if(elem.attribute("id").compare(QString::number(item->itemId))==0){
                 elem.setAttribute("pos_x", pos.x());
                 elem.setAttribute("pos_y", pos.y());
//                 qDebug() << doc.toString();
                 return;
             }
        }
        node = node.nextSibling();
    }
    qDebug() << "[warning] 未找到匹配的id=" << item->itemId;
}

void RadarScene::modifyXmlArrows(Arrow *arrow, DiagramItem *startItem, DiagramItem *endItem)
{
    QDomElement arr = doc.createElement("arrow");
    QDomElement color = doc.createElement("color");
    QDomAttr start = doc.createAttribute("start_item_id");
    QDomAttr end = doc.createAttribute("end_item_id");
    QDomAttr id = doc.createAttribute("id");
    start.setValue(QString::number(startItem->itemId));
    end.setValue(QString::number(endItem->itemId));
    id.setValue(QString::number(arrow->itemId));
    arr.setAttributeNode(start);
    arr.setAttributeNode(end);
    arr.setAttributeNode(id);
    QDomText c = doc.createTextNode(myLineColor.name());
    color.appendChild(c);
    arr.appendChild(color);
    Arrs.appendChild(arr);
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
        case InsertItem:{
            item = new DiagramItem(myItemType, myItemMenu);
            item->setBrush(myItemColor);
            item->itemId = generateUniqueid();
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);
            modifyXmlItems(mouseEvent->scenePos(), item);
            break;
        }
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
        //防止点线段
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();
        //删除press时新建的线段
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
            arrow->itemId = generateUniqueid();
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();

            modifyXmlArrows(arrow, startItem, endItem);
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
