#include "radarcompdraglistwidget.h"
#include <QDrag>
#include <QDebug>
#include <QMimeData>
#include <QMouseEvent>
#include <QApplication>
#include <QSizePolicy>
#include <QGridLayout>
#include <QInputDialog>
// This is available in all editors.
/**
* @projectName   prototype_v0906
* @brief         简介: 雷达组件列宁
* @author        Antrn
* @date          2019-09-27
*/
RadarCompDraglistWidget::RadarCompDraglistWidget(QWidget *parent) : QListWidget(parent)
{
    // 设置允许接收拖拽
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setSpacing(5);

    // icon图标显示
    setViewMode(QListView::ListMode);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //垂直滚动条
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //水平滚动条
    // 设置ICON大小
    setIconSize(QSize(50, 50));

    this->setDropIndicatorShown(true);
    // 设置拖放模式为移动项目，否则为复制项目
    this->setDragDropMode(QAbstractItemView::InternalMove);
    this->setAttribute(Qt::WA_PendingMoveEvent);

    // 加入新建雷达按钮项
    this->addItem(tr("自定义组件"));
    addCompButton = this->item(0);
    addCompButton->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    // addRadarButton->setBackgroundColor(QColor(211,211,211));
    addCompButton->setFlags(Qt::NoItemFlags);
    addCompButton->setIcon(QIcon(":/img/newradar.png"));
    addCompButton->setToolTip(tr("点击增加自定义组件"));
}

void RadarCompDraglistWidget::addDragItem(QListWidgetItem*item){
    this->addItem(item);
    this->setVisible(true);
}

void RadarCompDraglistWidget::startDrag(Qt::DropActions /*supportedActions*/)
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
//    qDebug() << "pixmap: " << pixmap;
//    qDebug() << "picture str_name: " << str_name;

    QMimeData *mimeData = new QMimeData;
    //设置自定义mime数据
    mimeData->setData(RadarCompDraglistWidget::puzzleMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    //鼠标始终保持在元件的中心
    drag->setHotSpot(QPoint(25, 25));
    //缩放图片
    drag->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio));

    if (drag->exec(Qt::MoveAction) == Qt::CopyAction){
        qDebug() << "复制动作";
    }
}

//重写鼠标点击操作.
void RadarCompDraglistWidget::mousePressEvent(QMouseEvent *event)
{
    //确保左键拖拽.
    if (event->button() == Qt::LeftButton)
    {
        // qDebug() << "item被点击";
        // 先保存拖拽的起点.
        m_dragPoint = event->pos();
        // 保留被拖拽的项.
        m_dragItem = this->itemAt(m_dragPoint);
        // 如果点击项是新建项则新建组件
        if(m_dragItem == addCompButton){
            int flag = 0;
            QString Compname = "";
            while(Compname.isEmpty() || Compname.isNull()){
                // 弹窗填写参数
                QInputDialog dlg;
                dlg.setWindowTitle("参数编辑");
                dlg.setLabelText("组件名：");
                dlg.setInputMode(QInputDialog::TextInput);
                if( dlg.exec() == QInputDialog::Accepted )
                {
                    Compname = dlg.textValue();
                }else{
                    qDebug() << "取消";
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                QListWidgetItem *item1 = new QListWidgetItem();
                item1->setIcon(QIcon(":/img/Comp.png"));
                item1->setText(tr(Compname.toUtf8().data()));
                //这里的用户角色存储用户数据
                item1->setData(Qt::UserRole, QPixmap(":/img/Comp.png"));
                item1->setData(Qt::UserRole+1, Compname);
                item1->setData(Qt::UserRole+2, this->count());
                item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
                this->addDragItem(item1);
                emit add_one_Comp(Compname);
            }
        }else if(m_dragItem){
            emit add_one_Comp(m_dragItem->text());
            // 获取点击的是哪个组件的id，传到radarScene中，知道该渲染出哪个组件
            int id = qvariant_cast<int>(m_dragItem->data(Qt::UserRole+2))-1; //从0开始
            qDebug() << id;
            emit setComp_typeandMode(id);
        }
    }
    //保留原QListWidget部件的鼠标点击操作.
    QListWidget::mousePressEvent(event);
}
