#include "draglistwidget.h"
#include <QDrag>
/**
* @projectName   prototype_v0719
* @brief         整体工程项目左边的拖拽列表：雷达/电子对抗设备，是以list的形式表现的。
* @author        Antrn
* @date          2019-08-12
*/
DragListWidget::DragListWidget(QWidget *parent) : QListWidget(parent)
{
    //    设置允许接收拖拽
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setSpacing(5);

    //icon图标显示
    setViewMode(QListView::ListMode);
    //TODO 设置ICON大小
    setIconSize(QSize(50, 50));

    this->setDropIndicatorShown(true);
    //设置拖放模式为移动项目，否则为复制项目
    this->setDragDropMode(QAbstractItemView::InternalMove);
    this->setAttribute(Qt::WA_PendingMoveEvent);

//    加入新建雷达按钮项
    this->addItem("新建雷达");
    addRadarButton = this->item(0);
    addRadarButton->setTextAlignment(1);
    addRadarButton->setBackgroundColor(QColor(211,211,211));
    addRadarButton->setFlags(Qt::NoItemFlags);
    addRadarButton->setIcon(QIcon(":/img/newradar.png"));

//    设置初始尺寸
    this->resize(250,200);

}

//添加拖拽项，使用此方法会动态resize整个widget
void DragListWidget::addDragItem(QListWidgetItem*item){

    int count = this->count();
    int height = count*40;
    int minHeight = 150;
    int maxHeight = 400;
    if(height < minHeight){
        height = minHeight;
    }
    else if(height > maxHeight){
        height = maxHeight;
    }
    this->resize(240,height);
    this->addItem(item);
    this->setVisible(false);
    this->setVisible(true);
    emit repaintWidget();

}

//添加拖拽项，使用此方法会动态resize整个widget
void DragListWidget::addDragItem(const QString &label){
    int count = this->count();
    int height = count*40;
    int minHeight = 150;
    int maxHeight = 400;
    if(height < minHeight){
        height = minHeight;
    }
    else if(height > maxHeight){
        height = maxHeight;
    }
    this->resize(240,height);
    this->addItem(label);
//    我发现隐藏显示以后，尺寸会变成layout内的默认尺寸，相当于 resize无效，这个方法废了喝喝
    this->setVisible(false);
    this->setVisible(true);
    emit repaintWidget();

}

void DragListWidget::startDrag(Qt::DropActions /*supportedActions*/)
{
    //可以直接使用currentItem获得当前的组件
    QListWidgetItem *item = currentItem();
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    //分别从用户角色中获取信息
    QPixmap pixmap = qvariant_cast<QPixmap>(item->data(Qt::UserRole));
    QString str_name = qvariant_cast<QString>(item->data(Qt::UserRole+1));

    //设置dataStream
    dataStream << pixmap << str_name;
    qDebug() << "pixmap: " << pixmap;
    qDebug() << "picture str_name: " << str_name;

    QMimeData *mimeData = new QMimeData;
    //设置自定义mime数据
    mimeData->setData(DragListWidget::puzzleMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    //鼠标始终保持在元件的中心
    drag->setHotSpot(QPoint(25, 25));
    drag->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio));

    //拖动而不是复制
    if (drag->exec(Qt::MoveAction) == Qt::CopyAction){
        //删除掉当前组件
//        delete takeItem(row(item));
        //TODO  未来保留组件库的元素
        qDebug() << "复制动作";
    }
}

//重写鼠标点击操作.
void DragListWidget::mousePressEvent(QMouseEvent *event)
{
    //确保左键拖拽.
    if (event->button() == Qt::LeftButton)
    {
//        qDebug() << "item被点击";
        //先保存拖拽的起点.
        m_dragPoint = event->pos();
        //保留被拖拽的项.
        m_dragItem = this->itemAt(event->pos());
//        如果点击项是新建项则新建雷达
        if(m_dragItem == addRadarButton){
            int count = this->count();
            QString newname = "雷达"+QString::number(count);
            QListWidgetItem *item1 = new QListWidgetItem();
            item1->setIcon(QIcon(":/img/radar.png"));
            item1->setText(tr(newname.toUtf8().data()));
            //这里的用户角色存储用户数据
            item1->setData(Qt::UserRole, QPixmap(":/img/radar.png"));
            item1->setData(Qt::UserRole+1, QString("leida"));
            item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
            this->addDragItem(item1);
        }
    }
    //保留原QListWidget部件的鼠标点击操作.
    QListWidget::mousePressEvent(event);
}

//void DragListWidget::mouseMoveEvent(QMouseEvent *event)
//{
////        如果拖拽项是新建项则退出函数
//    if(m_dragItem == addRadarButton){
//        return;
//    }

//    //确保按住左键移动.
//    if (event->buttons() & Qt::LeftButton)
//    {
//        QPoint temp = event->pos() - m_dragPoint;
//        //只有这个长度大于默认的距离,才会被系统认为是形成拖拽的操作.
//        if (temp.manhattanLength() > QApplication::startDragDistance())
//        {
//            QDrag *drag = new QDrag(this);

//            QMimeData *mimeData = new QMimeData;
//            int dragIndex = this->row(m_dragItem);
////            QString mimetype = "dragIndex";
////            mimeData->setText(m_dragItem->text());
//            const QByteArray data = QByteArray(QString::number(dragIndex).toUtf8().data());
//            mimeData->setData(QString("dragIndex"), data);
//            drag->setMimeData(mimeData);
//            auto action = drag->exec(Qt::CopyAction | Qt::MoveAction);

//            if (action == (Qt::CopyAction) || (action == Qt::MoveAction))
//            {

//            }
//        }
//    }
//    QListWidget::mouseMoveEvent(event);
//}

//void DragListWidget::dragEnterEvent(QDragEnterEvent *event)
//{
//    //设置动作为移动动作.
//    event->setDropAction(Qt::MoveAction);
//    //然后接受事件.这个一定要写.
//    event->accept();
//}

//void DragListWidget::dragMoveEvent(QDragMoveEvent *event)
//{
//    event->setDropAction(Qt::MoveAction);
//    event->accept();
//}

////当拖拽项被放下时的操作.
//void DragListWidget::dropEvent(QDropEvent *event)
//{
//    QByteArray str = event->mimeData()->data(QString("dragIndex"));
//    int index = str.toInt();
//    qDebug()<<index;

////    event->setDropAction(Qt::MoveAction);
//    event->accept();
//}
