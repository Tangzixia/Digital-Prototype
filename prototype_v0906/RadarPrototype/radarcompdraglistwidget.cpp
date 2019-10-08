#include "radarcompdraglistwidget.h"
#include <QDrag>
#include <QDebug>
#include <QMimeData>
#include <QMouseEvent>
#include <QApplication>
#include <QSizePolicy>
#include <QGridLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <qfiledialog.h>
#include <algorithmcomp.h>
#include <parameditradardialog.h>
#include "utils.h"
#include <QApplication>
#include <QDesktopWidget>
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
    addCompButton->setTextColor(QColor(255,0,0));
    addCompButton->setFlags(Qt::NoItemFlags);
    addCompButton->setIcon(QIcon(":/img/newradar.png"));
    addCompButton->setToolTip(tr("点击增加自定义组件"));
    connect(this, &QListWidget::itemDoubleClicked, this, &RadarCompDraglistWidget::onCurrentDoubleClicked);
    connect(this, &QListWidget::itemChanged, this, &RadarCompDraglistWidget::onCurrentTextChanged);
}

void RadarCompDraglistWidget::addDragItem(QListWidgetItem*item){
    this->addItem(item);
    this->setVisible(true);
}
// This is an auto-generated comment.
/**
* @projectName   prototype_v0906
* @brief         简介 新建组件
* @author        Antrn
* @date          2019-10-05
*/
void RadarCompDraglistWidget::createNewComp()
{
    AlgorithmComp ac;
    // 新建
    ParamEditRadarDialog dlg(ac);
    if(dlg.exec() == QDialog::Accepted)
    {
        qDebug() << "确定新建";
        algorithms.insert(dlg.ac.getInfo()["ID"], dlg.ac);
        qDebug() << "刚增加的id:" << dlg.ac.getInfo()["ID"];
        qDebug() << "algorithms： " << algorithms.keys() << "; 大小： " << algorithms.size();
        Utils::writeAlgorithmComp2Xml(dlg.ac);
        emit add_one_Comp(dlg.ac);
        emit toRefreshCompList(); //刷新列表
        Utils::alert(QApplication::desktop()->screen()->rect(), "添加组件成功!");
    }else{
        qDebug() << "取消新建";
    }
}

void RadarCompDraglistWidget::onCurrentTextChanged(QListWidgetItem *item)
{
    if(item!=addCompButton){
        QString lastName = item->text();
        qDebug() << "item名字变为： " << lastName << "； item内容有变化";
        if(nameList.contains(lastName)){
            qDebug() << "与现有的文件名: " << lastName << "重复!";
            Utils::alert(QApplication::desktop()->screen()->rect(), "已有重复名称存在，请重试");
        }else{
            nameList.removeOne(oldName);
            qDebug() << oldName << "→" << lastName;
            if(Utils::modifyFileName(oldName, lastName)){
                qDebug() << "重命名成功!";
            }else{
                qDebug() << "重命名失败";
            }
        }
    }
}

void RadarCompDraglistWidget::onCurrentDoubleClicked(QListWidgetItem *item)
{
    QString preName = item->text();
    if(!nameList.contains(preName)){
        qDebug() << "原名字: " << preName << "不存在，出现错误";
    }else{
        oldName = preName;
        qDebug() << "原名字： " << preName;
    }
}

void RadarCompDraglistWidget::deleteItemSlot()
{
    int ch = QMessageBox::warning(nullptr, "提醒",
                                      "您确定要删除此组件吗?\n若已有场景中使用了此组件，您的删除会造成致命错误。",
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);
    if ( ch != QMessageBox::Yes )
        return;
    QListWidgetItem * item = currentItem();
    if( item == nullptr )
        return;
    QString na = item->text();
    QString id = item->data(Qt::UserRole+2).toString();
    qDebug() << "删除组件: " << na;
    takeItem(row(item));
    // nameList删除
    nameList.removeOne(na);
    // 这里面也要删
    algorithms.take(id);
    qDebug() << "algorithms： " << algorithms.keys() << "; 大小： " << algorithms.size();
    // 文件也要删除
    QFile file(QDir::currentPath()+"/algoXml/"+na+".xml");
    file.remove();
    delete item;
}

void RadarCompDraglistWidget::editItemParamSlot()
{
    QListWidgetItem * item = currentItem();
    if( item == nullptr )
        return;
    qDebug() << "要编辑的组件名字为: " << item->text();
    AlgorithmComp ac;
    ac = algorithms.value(item->data(Qt::UserRole+2).toString());
    ParamEditRadarDialog dlg(ac);
    if(dlg.exec() == QDialog::Accepted)
    {
        qDebug() << "确定编辑";
        Utils::writeAlgorithmComp2Xml(dlg.ac);
        emit add_one_Comp(dlg.ac);
        emit toRefreshCompList(); //刷新列表
    }else{
        qDebug() << "取消编辑";
    }
}

void RadarCompDraglistWidget::createItemParamSlot()
{
    createNewComp();
}

// This is an auto-generated comment.
/**
* @projectName   prototype_v0906
* @brief         简介 item右键菜单
* @author        Antrn
* @date          2019-10-05
*/
void RadarCompDraglistWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidgetItem *currItem = itemAt(event->pos());
    QMenu *popMenu = new QMenu(this);
    QAction *addAct = nullptr, *deleteAct = nullptr, *editAct = nullptr;
    if(currItem != nullptr && currItem != addCompButton){
        qDebug() << "------------" << currItem->text();
        deleteAct = new QAction(tr("删除组件"), this);
        editAct = new QAction(tr("修改参数"), this);
        deleteAct->setIcon(QIcon(":/img/delete.png"));
        editAct->setIcon(QIcon(":/img/editComp.png"));
        popMenu->addAction(deleteAct);
        popMenu->addAction(editAct);
        connect(deleteAct, &QAction::triggered, this, &RadarCompDraglistWidget::deleteItemSlot);
        connect(editAct, &QAction::triggered, this, &RadarCompDraglistWidget::editItemParamSlot);
    }else{
        addAct = new QAction(tr("添加组件"), this);
        addAct->setIcon(QIcon(":/img/addComp.png"));
        popMenu->addAction(addAct);
        connect(addAct, &QAction::triggered, this, &RadarCompDraglistWidget::createItemParamSlot);
        qDebug() << "此位置没有组件，或者是第一个自定义按钮";
    }
    popMenu->exec(QCursor::pos());
    delete popMenu;
    delete deleteAct;
    delete editAct;
    delete addAct;
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
            QMessageBox msgBox;
            msgBox.setWindowTitle("添加组件");
            msgBox.setText("添加雷达组件");
            msgBox.setInformativeText("您想要创建一个新的雷达组件，还是导入一个已经有的雷达组件？");
            QPushButton *newButton = msgBox.addButton(tr("新建"), QMessageBox::ActionRole);
            msgBox.addButton(tr("导入"), QMessageBox::ActionRole);
            msgBox.addButton(tr("取消"), QMessageBox::RejectRole);
            msgBox.setDefaultButton(newButton);
            int button_index=msgBox.exec();
            switch (button_index) {
                case 2: qDebug() <<"不添加也不导入，关闭"; break;
                case 0:{
                    createNewComp();
//                    int flag = 0;
//                    QString Compname = "";
//                    while(Compname.isEmpty() || Compname.isNull()){
//                        // 弹窗填写参数
//                        QInputDialog dlg;
//                        dlg.setWindowTitle("参数编辑");
//                        dlg.setLabelText("组件名：");
//                        dlg.setInputMode(QInputDialog::TextInput);
//                        if( dlg.exec() == QInputDialog::Accepted )
//                        {
//                            Compname = dlg.textValue();
//                        }else{
//                            qDebug() << "取消";
//                            flag = 1;
//                            break;
//                        }
//                    }
//                    // 没点取消
//                    if(flag == 0){
//                        QListWidgetItem *item1 = new QListWidgetItem();
//                        item1->setIcon(QIcon(":/img/component.png"));
//                        item1->setText(tr(Compname.toUtf8().data()));
//                        //这里的用户角色存储用户数据
//                        item1->setData(Qt::UserRole, QPixmap(":/img/component.png"));
//                        item1->setData(Qt::UserRole+1, Compname);
//                        item1->setData(Qt::UserRole+2, this->count());
//                        item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
//                        this->addDragItem(item1);
//                        emit add_one_Comp(Compname);
//                    }

                    break;
                }
            case 1:
                // 导入文件
                QString dirpath = QDir::currentPath()+"/algoXml/";
                Utils::openDirOrCreate(dirpath);
                // 打开xml文件读取
                const QString fileName = QFileDialog::getOpenFileName(this, tr("打开组件xml"), QString(dirpath), tr("xml files (*.xml)"));
                qDebug() << "长名字：" << fileName;

//                QFileInfo fi = QFileInfo(fileName);
//                QString file_name = fi.fileName().split(".")[0];  //获取文件名
//                qDebug() << "短名字： " << file_name;
                AlgorithmComp ac = Utils::readPluginXmlFile(fileName);
                QString file_name = ac.getFileName();
                // 将这里的文件复制过来
                QFile::copy(fileName, dirpath+file_name+".xml");

                // 已经有了
                if(algorithms.contains(ac.getInfo()["ID"])){
                    qDebug() << "已经导入了! id:" << ac.getInfo()["ID"] << "组件名字: " << ac.getInfo()["Name"];
                }else{
                    algorithms.insert(ac.getInfo()["ID"], ac);
                    QListWidgetItem *item1 = new QListWidgetItem();
                    item1->setIcon(QIcon(":/img/component.png"));
                    item1->setText(file_name); //ac.getInfo()["Name"].toUtf8().data()
                    //这里的用户角色存储用户数据
                    item1->setData(Qt::UserRole, QPixmap(":/img/component.png"));
                    item1->setData(Qt::UserRole+1, file_name); // ac.getInfo()["Name"]
                    // TODO 向这里的id和之前写的id全要改，以xml中的id为准，唯一标识
                    item1->setData(Qt::UserRole+2, ac.getInfo()["ID"]);
                    item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
                    this->addDragItem(item1);
                    emit add_one_Comp(ac);
                    nameList.append(file_name);
                    qDebug() << "列表的大小：" << algorithms.size();
                }
                break;
            }
        }else if(m_dragItem){
            // 这里的ID也不对
//            int id = qvariant_cast<int>(m_dragItem->data(Qt::UserRole+2))-1; //从0开始
            int id = qvariant_cast<int>(m_dragItem->data(Qt::UserRole+2)); //从0开始
            qDebug() << "当前id:" << id;
            // 点击其他组件,这个id应该为xml中的ID
            emit add_one_Comp(algorithms[QString::number(id)]);
            // 获取点击的是哪个组件的id，传到radarScene中，知道该渲染出哪个组件
//            emit setComp_typeandMode(id);
            emit setComp_typeandMode(qvariant_cast<QString>(m_dragItem->data(Qt::UserRole+1)));
        }
    }
    //保留原QListWidget部件的鼠标点击操作.
    QListWidget::mousePressEvent(event);
}
