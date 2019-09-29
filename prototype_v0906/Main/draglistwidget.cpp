#include "draglistwidget.h"
#include <QDrag>
#include <QMessageBox>
#include<QAbstractButton>

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
    this->setViewMode(QListView::ListMode);
    //、设置ICON大小
   this-> setIconSize(QSize(40, 40));
    this->setDropIndicatorShown(true);
    //设置拖放模式为移动项目，否则为复制项目
    this->setDragDropMode(QAbstractItemView::InternalMove);
    this->setAttribute(Qt::WA_PendingMoveEvent);
    //旧代码
{
//    this->addItem(tr("新建雷达"));
//    addRadarButton = this->item(0);
//    addRadarButton->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
////    addRadarButton->setBackgroundColor(QColor(211,211,211));
//    addRadarButton->setFlags(Qt::NoItemFlags);
//    addRadarButton->setIcon(QIcon(":/img/newradar.png"));
}
    //    加入新建雷达按钮项
    this->listItem_add("雷达");
//    设置初始尺寸
   // this->resize(250,200);

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
//    qDebug() << "pixmap: " << pixmap;
//    qDebug() << "picture str_name: " << str_name;

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
        // TODO  未来保留组件库的元素
        qDebug() << "复制动作";
    }
}
//重写鼠标点击操作.
void DragListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidgetItem * item= new QListWidgetItem();
    item=this->itemAt(event->pos());
    //确保左键拖拽.
    if (event->button() == Qt::LeftButton)
    {
//        qDebug() << "item被点击";
        //先保存拖拽的起点.
        m_dragPoint = event->pos();
        //保留被拖拽的项.
        m_dragItem = item;

//        老代码
        {
//        如果点击项是新建项则新建雷达

//        if(m_dragItem == addRadarButton){
//            int count = this->count();
//            QString newname = "雷达"+QString::number(count);
//            QListWidgetItem *item1 = new QListWidgetItem();
//            item1->setIcon(QIcon(":/img/radar.png"));
//            item1->setText(tr(newname.toUtf8().data()));
//            //这里的用户角色存储用户数据
//            item1->setData(Qt::UserRole, QPixmap(":/img/radar.png"));
//            item1->setData(Qt::UserRole+1, newname);
//            item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
//            this->addDragItem(item1);
//        }
        }
    }

    if (event->button() == Qt::RightButton&&item!=this->item(0)){

        if(item){
            item->checkState();
            QString id=this->itemAt(event->pos())->text();

            Menu_iteamOperation *menu=new Menu_iteamOperation();
            connect(menu , &Menu_iteamOperation::itemOperate,
                    [=](Menu_iteamOperation::OperateType operate){
                     //信号---->信号（向父容器传递）
                        //确定才删除
                        if(operate==Menu_iteamOperation::del){
                            //先询问是否确定
                            if(QMessageBox::Ok==QMessageBox::question(this,"question","删除此控件，场景中已经添加的组件也将被删除！是否还要继续？", QMessageBox::Ok| QMessageBox::Cancel))
                                emit itemOperate(operate,id);

                        }else {emit itemOperate(operate,id);}
                });
            menu->setAttribute(Qt::WA_DeleteOnClose);
            menu->exec(QPoint(event->globalPos().x(),event->globalPos().y()));
            delete menu;
       }
    }

    //保留原QListWidget部件的鼠标点击操作.
    QListWidget::mousePressEvent(event);
}

void DragListWidget:: mouseDoubleClickEvent(QMouseEvent *event){
//      必须新建一个QListWidgetItem过渡 否则点击的地方没有item就死了。
    QListWidgetItem * item= new QListWidgetItem();
    item=this->itemAt(event->pos());
    if(item){
        this->itemOperateSlot(Menu_iteamOperation::edit, this->itemAt(event->pos())->text());
    }
    QListWidget::mouseDoubleClickEvent(event);

}
//item操作
void DragListWidget::itemOperateSlot(Menu_iteamOperation::OperateType operateType, QString id){
//   获取id对应的item
    QListWidgetItem *item=id_item.key(id);
   if(item){
       switch (operateType){ 
       case Menu_iteamOperation::del:   
           qDebug()<<"left_delete:"<<id;
                this->removeItemWidget(item);
                delete item;
                id_item.remove(item);
                if(!newEditWindowList.isEmpty())newEditWindowList.removeOne(id);
           break;
       case Menu_iteamOperation::edit:
            qDebug()<<"left_edit:"<<id;
            //新建或者提升编辑窗口
            {
                //查找是否已经创建该子类
              //qDebug()<<(newEditWindowList->indexOf(id)==-1);
            if(newEditWindowList.isEmpty()||newEditWindowList.indexOf(id)==-1){
                //新建
                MainWindow_Radar *SET_RADARNAME(id)=new MainWindow_Radar(id,this);
                newEditWindowList.append(id);
                qDebug()<<"aa:"<<SET_RADARNAME(id);
                SET_RADARNAME(id)->setAttribute(Qt::WA_DeleteOnClose);
                SET_RADARNAME(id)->show();
            }else {
                //获取改item对应的mainwindow_radar
                QList<MainWindow_Radar*> radarList=this->findChildren<MainWindow_Radar*>();
                foreach(MainWindow_Radar* var,radarList){
                    qDebug()<<var<<":"<<var->getEquip_id();
                    if(var->getEquip_id()==id){
                            QMessageBox::warning(this,"warning","你已经打开了编辑窗口，不可以重复打开！");
                            var->showNormal();
//                        var->raise();
                    }

                }
            }
            }
           break;
       case Menu_iteamOperation::property:
           qDebug()<<"left_property:"<<id;
           break;
       }

    }
   }

void DragListWidget::listItem_add(QString name){
    QListWidgetItem *listItem_top=new QListWidgetItem();
    listItem_top->setFlags(Qt::NoItemFlags);
    listItem_top->setWhatsThis("添加更多的"+name+"组件");
    listItem_top->setText("添加"+name);
    listItem_top->setIcon(QIcon(":/img/newradar.png"));
    this->addItem(listItem_top);
    connect(this,&QListWidget::itemClicked,[=](QListWidgetItem* action_item){
        if (action_item->text()=="添加"+name){
//            qDebug()<<"触发新建item了。";
            QMessageBox msgBox;
            msgBox.setWindowTitle("添加"+name);
            msgBox.setText("添加"+name);
            msgBox.setInformativeText("您想要创建一个新的"+name+"组件，还是导入一个已经有的"+name+"组件？");
            msgBox.addButton(tr("取消"), QMessageBox::ActionRole);
            QPushButton*newButton = msgBox.addButton(tr("新建"), QMessageBox::ActionRole);
            msgBox.addButton(tr("导入"), QMessageBox::ActionRole);
            msgBox.setDefaultButton(newButton);
            int button_index=msgBox.exec();
            switch (button_index) {
                case 0:
//                    不操作
                    break;
            case 1:
                this->add_listItem(name);
                    break;
                case 2:
 //                导入文件
                    break;
            }
        }
    });
}

void DragListWidget::add_listItem(QString name) {
    QString path=":/img/radar.png";
    // 有必要枚举   待修缮
    if(name=="雷达"){path=":/img/radar.png";}
    else if(name=="电子对抗机") path=":/img/radar.png";
    else if(name=="目标环境")path=":/img/radar.png";
    //新建item，添加到左边的listwidget

    QString newname = name+QString::number(id_inde++);
    QListWidgetItem *item = new QListWidgetItem();
    id_item.insert(item,newname);
    item->setIcon(QIcon(path));
    item->setText(tr(newname.toUtf8().data()));

    //这里的用户角色存储用户数据（和拖入场景有关）
    item->setData(Qt::UserRole, QPixmap(path));
    item->setData(Qt::UserRole+1, newname);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    this->addDragItem(item);
    qDebug()<<"additem了。"<<newname;
}
