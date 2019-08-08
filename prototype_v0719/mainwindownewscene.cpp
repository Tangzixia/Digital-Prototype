#include "arrowitem.h"
#include "draglistwidget.h"
#include "mainwindownewscene.h"
#include "radaritem.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>

MainWindowNewScene::MainWindowNewScene()
{
//    将begin_item设为空指针
    line_begin_item = nullptr;
    dragFlag = false;
    // 清空列表
    Item_List.clear();
    Line_List.clear();
    graphicsLineItem_List.clear();
}

MainWindowNewScene::~MainWindowNewScene()
{

}

//由两个Radaritem计算起始点和终点
void MainWindowNewScene::computeLine(QGraphicsItem*fromItem, QGraphicsItem*toItem){
    QRectF *from = new QRectF;
    from->setX(fromItem->pos().x());
    from->setY(fromItem->pos().y());
    from->setWidth(fromItem->boundingRect().width());
    from->setHeight(fromItem->boundingRect().height());

    QRectF *to = new QRectF;
    to->setX(toItem->pos().x());
    to->setY(toItem->pos().y());
    to->setWidth(toItem->boundingRect().width());
    to->setHeight(toItem->boundingRect().height());

    QPointF*from_center = new QPointF(from->center());
    QPointF*from_top = new QPointF(from_center->x(),from->top());
    QPointF*from_right = new QPointF(from->right(),from_center->y());
    QPointF*from_bottom = new QPointF(from_center->x(),from->bottom());
    QPointF*from_left = new QPointF(from->left(),from_center->y());

    QPointF*to_center = new QPointF(to->center());
    QPointF*to_top = new QPointF(to_center->x(),to->top());
    QPointF*to_right = new QPointF(to->right(),to_center->y());
    QPointF*to_bottom = new QPointF(to_center->x(),to->bottom());
    QPointF*to_left = new QPointF(to->left(),to_center->y());

    QList<QLineF*> compare_line_List = QList<QLineF*>();
    QLineF*minline;

    //    当to在from右上角时
    if(to->left()>from->right()&&to->bottom()<from->top()){
        QLineF*line1 = new QLineF(*from_top, *to_left);
        QLineF*line2 = new QLineF(*from_top, *to_bottom);
        QLineF*line3 = new QLineF(*from_right, *to_left);
        QLineF*line4 = new QLineF(*from_right, *to_bottom);
        compare_line_List.append(line1);
        compare_line_List.append(line2);
        compare_line_List.append(line3);
        compare_line_List.append(line4);
        minline = computeMinLength(compare_line_List);
        sourcePoint = minline->p1();
        destPoint = minline->p2();
        return;
    }
    //    当to在from右下角时
    if(to->left()>from->right()&&to->top()>from->bottom()){
        QLineF*line1 = new QLineF(*from_right, *to_left);
        QLineF*line2 = new QLineF(*from_right, *to_top);
        QLineF*line3 = new QLineF(*from_bottom, *to_left);
        QLineF*line4 = new QLineF(*from_bottom, *to_top);
        compare_line_List.append(line1);
        compare_line_List.append(line2);
        compare_line_List.append(line3);
        compare_line_List.append(line4);
        minline = computeMinLength(compare_line_List);
        sourcePoint = minline->p1();
        destPoint = minline->p2();
        return;
    }
    //    当to在from左上角时
    if(to->right()<from->left()&&to->bottom()<from->top()){
        QLineF*line1 = new QLineF(*from_top, *to_right);
        QLineF*line2 = new QLineF(*from_top, *to_bottom);
        QLineF*line3 = new QLineF(*from_left, *to_right);
        QLineF*line4 = new QLineF(*from_left, *to_bottom);
        compare_line_List.append(line1);
        compare_line_List.append(line2);
        compare_line_List.append(line3);
        compare_line_List.append(line4);
        minline = computeMinLength(compare_line_List);
        sourcePoint = minline->p1();
        destPoint = minline->p2();
        return;
    }
    //    当to在from左下角时
    if(to->right()<from->left()&&to->top()>from->bottom()){
        QLineF*line1 = new QLineF(*from_left, *to_right);
        QLineF*line2 = new QLineF(*from_left, *to_top);
        QLineF*line3 = new QLineF(*from_bottom, *to_right);
        QLineF*line4 = new QLineF(*from_bottom, *to_top);
        compare_line_List.append(line1);
        compare_line_List.append(line2);
        compare_line_List.append(line3);
        compare_line_List.append(line4);
        minline = computeMinLength(compare_line_List);
        sourcePoint = minline->p1();
        destPoint = minline->p2();
        return;
    }
    //    当to在from上方时
    if(to->left()<=from->right()&&to->right()>=from->left()&&to->bottom()<from->top()){
        minline = new QLineF(*from_top, *to_bottom);
        sourcePoint = minline->p1();
        destPoint = minline->p2();
        return;
    }
    //    当to在from下方时
    if(to->left()<=from->right()&&to->right()>=from->left()&&to->top()>from->bottom()){
        minline = new QLineF(*from_bottom, *to_top);
        sourcePoint = minline->p1();
        destPoint = minline->p2();
        return;
    }
    //    当to在from左面时
    if(to->bottom()>=from->top()&&to->top()<=from->bottom()&&to->right()<from->left()){
        minline = new QLineF(*from_left, *to_right);
        sourcePoint = minline->p1();
        destPoint = minline->p2();
        return;
    }
    //    当to在from右面时
    if(to->bottom()>=from->top()&&to->top()<=from->bottom()&&to->left()>from->right()){
        minline = new QLineF(*from_right, *to_left);
        sourcePoint = minline->p1();
        destPoint = minline->p2();
        return;
    }

}

//循环找到最短路径
QLineF* MainWindowNewScene::computeMinLength(QList<QLineF*> line_list){
    QLineF*min = line_list.first();
    for(int i = 1;i<line_list.length();i++){
        if(line_list.value(i)->length()<min->length()){
            min = line_list.value(i);
        }
    }
    return min;
}


void MainWindowNewScene::LinePaint()
{
    qDebug() << "重绘画板";

}

void MainWindowNewScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "鼠标点击位置：" << event->scenePos();
    dragFlag = true;
    //确保按住左键移动.
    if (event->buttons() == Qt::LeftButton){
//        先判断是否有begin_item存在，如果不存在
        if(line_begin_item==nullptr){
//            点击处如果有item
            if(this->itemAt(event->scenePos(), QTransform())){
//                更新begin_item
                line_begin_item = this->itemAt(event->scenePos(), QTransform());
                //        获取拖拽对象，此处是为拖拽做初始化准备，与连线无关，在mousemove事件做进一步处理
                drag_item = this->itemAt(event->scenePos(), QTransform());
                connect_to_List.clear();
                connect_from_List.clear();
                delete_line_List.clear();
                //        遍历线数组
                for(int i = 0;i<graphicsLineItem_List.length();i++){
                    ArrowItem*linetemp = graphicsLineItem_List.at(i);
                    if(linetemp->getEndItem()==drag_item){
                        connect_from_List.append(linetemp->getBeginItem());
                        delete_line_List.append(linetemp);
                    }
                    if(linetemp->getBeginItem()==drag_item){
                        connect_to_List.append(linetemp->getEndItem());
                        delete_line_List.append(linetemp);
                    }

                }
            }
        }
//        如果begin_item存在
        else{
//          第二次点击处如果有item
            if(this->itemAt(event->scenePos(), QTransform())){
                QGraphicsItem *line_end_item = this->itemAt(event->scenePos(), QTransform());
//                如果两次点击了同一item
                if(line_begin_item == line_end_item){
//                    把begin_item设回空指针,退出函数
                    line_begin_item = nullptr;

                }
                else {
//                    如果两次点击了不同item,开始连线
                    computeLine(line_begin_item,line_end_item);
                    ArrowItem *arrow = new ArrowItem;
                    arrow->setLineItem(line_begin_item,line_end_item);
                    arrow->setData(sourcePoint, destPoint);
                    arrow->setFlag(QGraphicsItem::ItemIsMovable, true);
                    this->addItem(arrow);
                    this->graphicsLineItem_List.append(arrow);
                    update();
//                    连线后把begin_item设回空指针
                    line_begin_item = nullptr;

                }
            }
//          第二次点击处如果没item，放弃连线，并把begin_item设回空指针
            else{
                line_begin_item = nullptr;
            }
        }

    }
    QGraphicsScene::mousePressEvent(event);
}

void MainWindowNewScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    dragFlag = false;
    QGraphicsScene::mouseReleaseEvent(event);
}

void MainWindowNewScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "点击位置: " << event->pos();
//    若为拖拽
    if(dragFlag == true){
        line_begin_item = nullptr;

        for(int i = 0;i<delete_line_List.length();i++){
            this->removeItem(delete_line_List.at(i));
            graphicsLineItem_List.removeOne(delete_line_List.at(i));
        }
        delete_line_List.clear();
        for (int i = 0;i<connect_to_List.length();i++) {
            computeLine(drag_item,connect_to_List.at(i));
            ArrowItem *arrow = new ArrowItem;
            arrow->setLineItem(drag_item,connect_to_List.at(i));
            arrow->setData(sourcePoint, destPoint);
            arrow->setFlag(QGraphicsItem::ItemIsMovable, true);
            this->addItem(arrow);
            this->graphicsLineItem_List.append(arrow);
            delete_line_List.append(arrow);

        }
        for (int i = 0;i<connect_from_List.length();i++) {
            computeLine(connect_from_List.at(i),drag_item);
            ArrowItem *arrow = new ArrowItem;
            arrow->setLineItem(connect_from_List.at(i),drag_item);
            arrow->setData(sourcePoint, destPoint);
            arrow->setFlag(QGraphicsItem::ItemIsMovable, true);
            this->addItem(arrow);
            this->graphicsLineItem_List.append(arrow);
            delete_line_List.append(arrow);
        }

        update();
    }
    emit signal_xy(event->scenePos().x(), event->scenePos().y());
    QGraphicsScene::mouseMoveEvent(event);
}

void MainWindowNewScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(DragListWidget::puzzleMimeType()))
        event->accept();
    else
        event->ignore();
    qDebug() << "托进入" <<endl;
}

void MainWindowNewScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(DragListWidget::puzzleMimeType())){
        event->acceptProposedAction();

        QByteArray comData = event->mimeData()->data(DragListWidget::puzzleMimeType());
        QDataStream dataStream(&comData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QString str;
        dataStream >> pixmap >> str;
        qDebug() << pixmap << " ; " << str;
        if(str == "leida"){
            RadarItem *radar=new RadarItem;
//            // 设置初始位置
//            radar->setPos(event->scenePos());
//            qDebug()<<"放下位置: " <<event->scenePos();
            // 设置初始位置
            radar->setPos(event->scenePos().x()-35, event->scenePos().y() -35);
            qDebug()<<"放下位置: " <<event->scenePos();
            this->addItem(radar);
            Item_List.append(radar);

            connect(radar, &RadarItem::destroyed, [=](){
                this->removeItem(radar);
                Item_List.removeOne(radar);
            });
        }
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else {
        event->ignore();
    }
    //qDebug() << "放下位置: " << event->scenePos() <<endl;
}

void MainWindowNewScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{

    if (event->mimeData()->hasFormat(DragListWidget::puzzleMimeType())) {
        //设置为移动而不是复制
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

