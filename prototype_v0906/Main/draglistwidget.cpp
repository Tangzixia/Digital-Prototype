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
DragListWidget::DragListWidget(QWidget *parent ) : QListWidget(parent)
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
    //修改名称后失去焦点生效
     connect(this, &DragListWidget::itemChanged,this,&DragListWidget::renameSlot);
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
    QString itemType = qvariant_cast<QString>(item->data(Qt::UserRole+2));
    //设置dataStream
    dataStream << pixmap << str_name<<itemType;
//    qDebug() << "pixma    p: " << pixmap;
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
                            if(QMessageBox::Ok==QMessageBox::question(this,"question",
                                                                        "删除此控件，场景中已经添加的组件"
                                                                      "也将被删除！是否还要继续？",
                                                                      QMessageBox::Ok, QMessageBox::Cancel))
                                emit itemOperate(Menu_iteamOperation::del,id);

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
//  必须新建一个QListWidgetItem过渡 否则点击的地方没有item就死了。
    QListWidgetItem * item= new QListWidgetItem();
    item=this->itemAt(event->pos());
    if(item){
    //双击修改名称
    {
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
    //双击进入编辑 暂时弃用
//        this->itemOperateSlot(Menu_iteamOperation::edit,item->text());
    }
    QListWidget::mouseDoubleClickEvent(event);
}

void DragListWidget::renameSlot(QListWidgetItem* item)
{
    //修改名称的过程会触发两次这个slot，所以第一次用来存初原来的名字，第二次用来改名字
    QString newName=item->text();
     //在修改名字
    if(this->forRename.ifEnableChange){
     if(this->currentRow()==forRename.preIndex && newName!=forRename.preName){
            qDebug()<<"##newName:"<<newName<<"preName:"<<forRename.preName;
            //校验新名字
         if(!this->id_item.contains(newName)){
                //不重复，修改其他所关联项目的id
            qDebug()<<"都去改吧！！:1：xml:2：编辑雷达页面 ，3：id_item map数组，4：newedititem数组，5：雷达场景id";
              //交由父类统一下发修改信号
             //itemOperateSlot(Menu_iteamOperation::rename,forRename.preName,newName);
            forRename.ifEnableChange=false;
            emit itemOperate(Menu_iteamOperation::rename,forRename.preName,newName);

         }else{
             QMessageBox::warning(this,"warning","名称已存在，请换一个名字重新修改",QMessageBox::Ok);
             item->setText(forRename.preName);
//             forRename.ifEnableChange=false;
         }
     }
     forRename.preIndex = this->currentRow();
     forRename.preName =  item->text();
    }else {forRename.ifEnableChange=true;}
}

void DragListWidget::itemOperateSlot(Menu_iteamOperation::OperateType operateType, QString id,QString newName){
        QListWidgetItem *item= id_item.find(id).value();
        //触发就选中
        item->isSelected();
       switch (operateType){ 
       case Menu_iteamOperation::del:
       {
           qDebug()<<"left_delete:"<<id;
            this->removeItemWidget(item);
            delete item;
            id_item.remove(id_item.key(item));
            if(!newEditWindowList.isEmpty())newEditWindowList.remove(id);
            break;
       }
       case Menu_iteamOperation::edit:
       {
            qDebug()<<"left_edit:"<<id;
            //新建或者提升编辑窗口
            {
                //查找是否已经创建该子类
             if(!newEditWindowList.contains(id)){
                //新建(每个变量命名不同)
                MainWindow_Radar *SET_RADARNAME(id)=new MainWindow_Radar(id);
                newEditWindowList.insert(id,SET_RADARNAME(id));
                //窗口关闭时更新子类列表：newEditWindowList
                connect(SET_RADARNAME(id),&MainWindow_Radar::iClose,
                        [=](MainWindow_Radar* radar){newEditWindowList.remove(radar->getEquip_id());});
                SET_RADARNAME(id)->setAttribute(Qt::WA_DeleteOnClose);
                SET_RADARNAME(id)->show();
            }else {
                //获取改item对应的mainwindow_radar
                MainWindow_Radar*radar=newEditWindowList.find(id).value();
                        QMessageBox::warning(this,"warning","你已经打开了编辑窗口，不可以重复打开！",QMessageBox::Ok);
                        radar->showNormal();
                          radar->raise();
            }
        }
           break;
       }
       case Menu_iteamOperation::rename:{
           if(  this->id_item.contains(id)){
               this->id_item.insert(newName,this->id_item.find(id).value());
               this->id_item.remove(id);
           }
           if(  this->newEditWindowList.contains(id)){
               MainWindow_Radar *radar=this->newEditWindowList.find(id).value();
               radar->setEquip_id(newName);
               this->newEditWindowList.insert(newName,radar);
               this->newEditWindowList.remove(id);
               radar->setWindowTitle(tr((newName+" Edit").toUtf8().data()));
            }
               item->setData(Qt::UserRole+1, newName);
           break;
       }
       case Menu_iteamOperation::property:
           qDebug()<<"left_property:"<<id;
           break;
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
    else if(name=="电子对抗机") path=":/img/ECM.png";
    else if(name=="目标环境")path=":/img/object.png";
    //新建item，添加到左边的listwidget
    QString newName = name+QString::number(id_inde++);
    while(this->id_item.contains(newName)){
        //名称已经存在，换一个
        newName = name+QString::number(id_inde++);
    }
    QListWidgetItem *item = new QListWidgetItem();
    id_item.insert(newName,item);
    item->setIcon(QIcon(path));
    item->setText(tr(newName.toUtf8().data()));

    //这里的用户角色存储用户数据（和拖入场景有关）
    item->setData(Qt::UserRole, QPixmap(path));
    item->setData(Qt::UserRole+1, newName);
    item->setData(Qt::UserRole+2, itemType);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    this->addDragItem(item);
    qDebug()<<"additem了,name:"<<newName;
}
