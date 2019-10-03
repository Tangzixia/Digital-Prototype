#include "mainwindow_radar.h"
#include "radarscene.h"
#include "ui_mainwindow_radar.h"
#include <QDebug>
#include <QCloseEvent>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QToolBar>
#include <QDir>
#include <QFileDialog>
#include <QDesktopServices>
#include <QLineEdit>
#include <QPushButton>
#include <utils.h>
#include <QCompleter>
#include <mainwindownew.h>

#include "arrow.h"
#include "compproperty.h"
#include "clickablelabel.h"
const int InsertTextButton = 10;
/**
* @projectName   prototype_v0719
* @brief         编辑雷达功能的主窗口。
* @author        Antrn
* @date          2019-08-12
*/

bool MainWindow_Radar::isSave = true;

MainWindow_Radar::MainWindow_Radar(QString id, QWidget *parent) :
    QMainWindow(parent),
    equip_id(id),
    ui(new Ui::MainWindow_Radar)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    //最大窗口
    this->showMaximized();
    setWindowTitle(tr((this->equip_id+" Edit").toUtf8().data()));
    //兼容性？？
    setUnifiedTitleAndToolBarOnMac(true);

    //组件列表设置(左边的窗口)
    ui->actionCom_list->setCheckable(true);
    ui->actionCom_list->setChecked(true);

    createActions();
    createCompBox();
    createMenus();

    scene = new RadarScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(itemInserted(DiagramItem*)),
            this, SLOT(itemInserted(DiagramItem*)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
            this, SLOT(itemSelected(QGraphicsItem*)));
    createToolbars();
    //点击run菜单项（或工具栏图标）触发
    connect(ui->actionRunRadar,SIGNAL(triggered()),
            scene,SLOT(startRunCode()));
 //    connect(ui->action_Stop,SIGNAL(triggered()),graphicsScene,SLOT(stopSimulation()));
    connect(scene,SIGNAL(startRun()),
            this,SLOT(On_start()));
    connect(scene,SIGNAL(overRun()),
            this,SLOT(On_over()));
    connect(scene,SIGNAL(rateSignal(float)),
            this,SLOT(On_rateSignal(float)));

    ui->graphicsView->setScene(scene);

    // xy坐标标签
    label_xy=new QLabel(this);
    // 状态栏左下角加入xy坐标
    ui->statusbar->addWidget(label_xy);
    // 设置鼠标跟踪开启
    ui->graphicsView->setMouseTracking(true);

    ui->graphicsView->setAcceptDrops(true);
    //设置可以使鼠标在视图上拖拽出橡皮筋框选择多个组件
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    connect(scene,SIGNAL(signal_xy(double,double)),this,SLOT(xy_show(double,double)));

    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
            this, SLOT(textInserted(QGraphicsTextItem*)));

    // 关联是否保存xml的按钮可用的信号和槽函数
    connect(scene, &RadarScene::isSave2False, this, &MainWindow_Radar::On_isSave2False);

    //打开自动读取已有的xml文件
    autoloadXmlById(getEquip_id());
    //    QGridLayout *layout = new QGridLayout;
//    layout->addWidget(ui->dockCompList);

//    layout->addWidget(ui->graphicsView);

    //抓手
    //ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag );

//    QWidget *widget = new QWidget;
//    widget->setLayout(layout);

//    setCentralWidget(widget);
    // FIXME dock无法自由拖动了，只能保持固定宽度能拖出来但是不能自动贴边
//    ui->dockCompList->setAllowedAreas(Qt::AllDockWidgetAreas);
    connect(ui->listWidget, &RadarCompDraglistWidget::add_one_Comp, this, &MainWindow_Radar::update_Comp_property);
    u = new Utils;
    // 当新增组件时候，设置插入模式和插入的组件的类型
    connect(ui->listWidget, &RadarCompDraglistWidget::setComp_typeandMode, this, &MainWindow_Radar::setComp_typeandMode);
}


void MainWindow_Radar::sceneScaleChanged(const QString &scale)
{
    isSave = false;
    toggleSaveXml(1);
    qDebug() << "xml由于场景缩放比例变化而改变";
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = ui->graphicsView->matrix();
    ui->graphicsView->resetMatrix();
    ui->graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
    ui->graphicsView->scale(newScale, newScale);
}

void MainWindow_Radar::pointerGroupClicked(int )
{
    scene->setMode(RadarScene::Mode(pointerTypeGroup->checkedId()));
}
//scene槽函数
void MainWindow_Radar::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *textItem =
    qgraphicsitem_cast<DiagramTextItem *>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}

void MainWindow_Radar::toggleSaveXml(int flag){
    if(flag == 1){
        ui->actionsave->setEnabled(true);
    }
    if(flag == 0){
        ui->actionsave->setEnabled(false);
    }
}

void MainWindow_Radar::init5Comp()
{
    //----------------------------------------
    QString Compname = "脉冲压缩";
    // 1
    QListWidgetItem *item1 = new QListWidgetItem();
    DiagramItem item(DiagramItem::Comp1, itemMenu);
    QIcon icon(item.image());
    item1->setIcon(icon);
    item1->setText(tr(Compname.toUtf8().data()));
    //这里的用户角色存储用户数据
    item1->setData(Qt::UserRole, QPixmap(item.image()));
    item1->setData(Qt::UserRole+1, Compname);
    item1->setData(Qt::UserRole+2, ui->listWidget->count());
    item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
    ui->listWidget->addDragItem(item1);
    update_Comp_property(Compname);

    // 2
    Compname = "恒虚警率";
    item1 = new QListWidgetItem();
    DiagramItem item2(DiagramItem::Comp2, itemMenu);
    QIcon icon2(item2.image());
    item1->setIcon(icon2);
    item1->setText(tr(Compname.toUtf8().data()));
    //这里的用户角色存储用户数据
    item1->setData(Qt::UserRole, QPixmap(item2.image()));
    item1->setData(Qt::UserRole+1, Compname);
    item1->setData(Qt::UserRole+2, ui->listWidget->count());
    item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
    ui->listWidget->addDragItem(item1);
    update_Comp_property(Compname);


    // 3
    Compname = "输入";
    item1 = new QListWidgetItem();
    DiagramItem item4(DiagramItem::Comp3, itemMenu);
    QIcon icon4(item4.image());
    item1->setIcon(icon4);
    item1->setText(tr(Compname.toUtf8().data()));
    //这里的用户角色存储用户数据
    item1->setData(Qt::UserRole, QPixmap(item4.image()));
    item1->setData(Qt::UserRole+1, Compname);
    item1->setData(Qt::UserRole+2, ui->listWidget->count());
    item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
    ui->listWidget->addDragItem(item1);
    update_Comp_property(Compname);

    // 4
    Compname = "动目标检测";
    item1 = new QListWidgetItem();
    DiagramItem item3(DiagramItem::Comp4, itemMenu);
    QIcon icon3(item3.image());
    item1->setIcon(icon3);
    item1->setText(tr(Compname.toUtf8().data()));
    //这里的用户角色存储用户数据
    item1->setData(Qt::UserRole, QPixmap(item3.image()));
    item1->setData(Qt::UserRole+1, Compname);
    item1->setData(Qt::UserRole+2, ui->listWidget->count());
    item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
    ui->listWidget->addDragItem(item1);
    update_Comp_property(Compname);

    // 5
    Compname = "输出";
    item1 = new QListWidgetItem();
    DiagramItem item5(DiagramItem::Comp5, itemMenu);
    QIcon icon5(item5.image());
    item1->setIcon(icon5);
    item1->setText(tr(Compname.toUtf8().data()));
    //这里的用户角色存储用户数据
    item1->setData(Qt::UserRole, QPixmap(item5.image()));
    item1->setData(Qt::UserRole+1, Compname);
    item1->setData(Qt::UserRole+2, ui->listWidget->count());
    item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
    ui->listWidget->addDragItem(item1);
    update_Comp_property(Compname);
}

// This is available in all editors.
/**
* @projectName   prototype_v0906
* @brief         简介 根据用户搜索名字找到组件，目前是用的xmls文件夹里的xml文件代替组件xml
* @author        Antrn
* @date          2019-09-27
*/
void MainWindow_Radar::loadCompByName(QString strText)
{
    QString m_sProjectPath = QDir::currentPath()+"/xmls/";/*文件夹全路径名*/
    QDir dir(m_sProjectPath);
    /*判断文件夹是否存在*/
    if (!dir.exists())
    {
        u->alert(geometry(), tr("读取文件夹出错!"));
        return;
    }
    dir.setFilter(QDir::Files); /*设置dir的过滤模式,表示只遍历本文件夹内的文件*/
    QStringList filterList;
    filterList << "*.xml";
    foreach(QFileInfo fileInfo, dir.entryInfoList(filterList)){
        QString absolute_file_path = fileInfo.absoluteFilePath();
        QString fileName = fileInfo.baseName();/*获取文件后名(不带后缀的文件名)*/
        if(fileName == strText){
            QListWidgetItem *item0 = new QListWidgetItem();
            item0->setIcon(QIcon(":/img/Comp.png"));
            item0->setText(tr(fileName.toUtf8().data()));
            //这里的用户角色存储用户数据
            item0->setData(Qt::UserRole, QPixmap(":/img/Comp.png"));
            item0->setData(Qt::UserRole+1, fileName);
            item0->setData(Qt::UserRole+2, ui->listWidget->count()+1);
            item0->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
            ui->listWidget->addDragItem(item0);
            update_Comp_property(fileName);
            break;
        }
    }
}
// This is available in all editors.
/**
* @projectName   prototype_v0906
* @brief         简介 加载所有组件， 目前只是加载了默认的五个组件
* @author        Antrn
* @date          2019-09-27
*/
void MainWindow_Radar::loadAllComps()
{
//    QString m_sProjectPath = QDir::currentPath()+"/xmls/";/*文件夹全路径名*/
//    QDir dir(m_sProjectPath);
//    /*判断文件夹是否存在*/
//    if (!dir.exists())
//    {
//        u->alert(geometry(), tr("读取文件夹出错!"));
//        return;
//    }
//    dir.setFilter(QDir::Files); /*设置dir的过滤模式,表示只遍历本文件夹内的文件*/
//    QStringList filterList;
//    filterList << "*.xml";
//    foreach(QFileInfo fileInfo, dir.entryInfoList(filterList)){
//        QString absolute_file_path = fileInfo.absoluteFilePath();
//        QString fileName = fileInfo.baseName();/*获取文件后名(不带后缀的文件名)*/
//        QListWidgetItem *item0 = new QListWidgetItem();
//        item0->setIcon(QIcon(":/img/Comp.png"));
//        item0->setText(tr(fileName.toUtf8().data()));
//        //这里的用户角色存储用户数据
//        item0->setData(Qt::UserRole, QPixmap(":/img/Comp.png"));
//        item0->setData(Qt::UserRole+1, fileName);
//        item0->setData(Qt::UserRole+2, ui->listWidget->count());
//        item0->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
//        ui->listWidget->addDragItem(item0);
//        update_Comp_property(fileName);
//    }
    ui->listWidget->clear();
    // 加入新建雷达按钮项
    ui->listWidget->addItem(tr("自定义组件"));
    ui->listWidget->addCompButton = ui->listWidget->item(0);
    ui->listWidget->addCompButton->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    // addRadarButton->setBackgroundColor(QColor(211,211,211));
    ui->listWidget->addCompButton->setFlags(Qt::NoItemFlags);
    ui->listWidget->addCompButton->setIcon(QIcon(":/img/newradar.png"));
    ui->listWidget->addCompButton->setToolTip(tr("点击增加自定义组件"));
    // TODO 换成读取组件文件夹里的所有组件
    init5Comp();
}

void MainWindow_Radar::itemInserted(DiagramItem *item)
{
    isSave = false;
    toggleSaveXml(1);
    qDebug() << "xml由于组件添加而改变";
    //当有图形项插入到场景中的时候，应该将指针组改为移动指针
    pointerTypeGroup->button(int(RadarScene::MoveItem))->setChecked(true);
    scene->setMode(RadarScene::Mode(pointerTypeGroup->checkedId()));
    //取消原按钮的选中状态
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}

void MainWindow_Radar::textInserted(QGraphicsTextItem *)
{
    isSave = false;
    toggleSaveXml(1);
    qDebug() << "xml由于文字添加而改变";
    //取消选中状态
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(RadarScene::Mode(pointerTypeGroup->checkedId()));
}

// 删除doc中的箭头和图形项
void MainWindow_Radar::deleteItemArrowById(int id)
{
    QDomNode itemNode = scene->getDoc().elementsByTagName("Items").at(0);
    QDomNodeList nodeList = itemNode.childNodes();
    for (int i=0; i<nodeList.size(); i++) {
        if(nodeList.at(i).isElement()){
            QDomElement elem = nodeList.at(i).toElement();
            if(elem.attribute("id") == QString::number(id)){
                itemNode.removeChild(nodeList.at(i));
                // 将id列表中的那个id删除
                scene->idList.removeOne(id);
                qDebug() << "找到图形项，已删除,id=" << id;
                break;
            }
        }
    }

    QDomNode arrowNode = scene->getDoc().elementsByTagName("Arrs").at(0);
    /**
     * 注意这里面不能先定义变量QDomNodeList arrowList = arrowNode.childNodes()；，然后下面使用arrowList，
     * 因为当你删除一个标签节点之后，总长度size变了，每次都要重新获取size，而且找到一次之后index从0开始继续遍历
     **/
    for (int j=0; j<arrowNode.childNodes().size(); j++) {
        if(arrowNode.childNodes().at(j).isElement()){
            QDomElement elem2 = arrowNode.childNodes().at(j).toElement();
            int arrowId = elem2.attribute("id").toInt();
            qDebug() << "start_item_id: " << elem2.attribute("start_item_id") << "end_item_id" << elem2.attribute("end_item_id");
            if(elem2.attribute("start_item_id") == QString::number(id) || elem2.attribute("end_item_id") == QString::number(id)){
                arrowNode.removeChild(arrowNode.childNodes().at(j));
                // 将id列表中的那个id删除
                scene->idList.removeOne(arrowId);
                qDebug() << "找到箭头，已删除,id=" << arrowId;
                // NOTE 这里千万注意不要写成了0，不然会少删除item，item和arrow不对应导致程序崩溃
                j=-1;
            }
        }
    }
}

void MainWindow_Radar::deleteArrowById(int id)
{
    QDomNode arrowNode = scene->getDoc().elementsByTagName("Arrs").at(0);
    for (int j=0; j<arrowNode.childNodes().size(); j++) {
        if(arrowNode.childNodes().at(j).isElement()){
            QDomElement elem2 = arrowNode.childNodes().at(j).toElement();
            int arrowId = elem2.attribute("id").toInt();
            if(elem2.attribute("id") == QString::number(id)){
                arrowNode.removeChild(arrowNode.childNodes().at(j));
                // 将id列表中的那个id删除
                scene->idList.removeOne(arrowId);
                qDebug() << "单独找到箭头将它删除,id=" << arrowId;
                j=0;
            }
        }
    }
}

//删除按钮触发
void MainWindow_Radar::deleteItem()
{
    if(scene->selectedItems().isEmpty()){
        qDebug() << "没有要删除的选择对象";
        QMessageBox::information(this, "alert", "请先选择组件!", QMessageBox::Ok);
    }else{
        isSave = false;
        toggleSaveXml(1);
        qDebug() << "xml由于删除添加控件而改变";
        //删除相连的箭头
        foreach (QGraphicsItem *item, scene->selectedItems()) {
            if (item->type() == Arrow::Type) {
                scene->removeItem(item);
                Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
                arrow->startItem()->removeArrow(arrow);
                arrow->endItem()->removeArrow(arrow);

                // 删除doc中的此图形项对应的标签
                deleteArrowById(arrow->itemId);
                delete item;
            }
        }
        DiagramItem *item_ = nullptr;
        foreach (QGraphicsItem *item, scene->selectedItems()) {
             if (item->type() == DiagramItem::Type){
                  item_ = qgraphicsitem_cast<DiagramItem *>(item);
                  item_->removeArrows();
                  // 删除doc中的此图形项对应的标签
                  deleteItemArrowById(item_->itemId);
             }
             scene->removeItem(item);
             delete item;
        }
    }
}
//变换背景
void MainWindow_Radar::backgroundButtonGroupClicked(QAbstractButton *button)
{
    QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    foreach (QAbstractButton *myButton, buttons) {
        if (myButton != button)
            button->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("Blue Grid"))
        scene->setBackgroundBrush(QPixmap(":/img/background1.png"));
    else if (text == tr("White Grid"))
        scene->setBackgroundBrush(QPixmap(":/img/background2.png"));
    else if (text == tr("Gray Grid"))
        scene->setBackgroundBrush(QPixmap(":/img/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/img/background4.png"));

    scene->update();
    ui->graphicsView->update();
}

void MainWindow_Radar::buttonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons) {
        if (buttonGroup->button(id) != button)
            button->setChecked(false);
    }
    //判断是否是文本框
    if (id == InsertTextButton) {
        scene->setMode(RadarScene::InsertText);
    } else {
        scene->setItemType(DiagramItem::DiagramType(id));
        scene->setMode(RadarScene::InsertItem);
    }
}

//弹出属性窗口
void MainWindow_Radar::showItemProperties()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == DiagramItem::Type) {
            // DELETE
            qDebug() << "Show Component Property";
            CompProperty *p = new CompProperty();
            p->exec();
        }
    }
}

// 退出的时候应该让用户选择是否保存当前场景
// FIXME 默认保存场景到name+.xml好像有点不合理，应该可以自己选择路径
// 先判断是否退出钱保存？是，则保存退出；否，则直接退出。
void MainWindow_Radar::closeEvent(QCloseEvent *event)
{
    //还未保存
    if(isSave == false){
        this->showNormal();
        this->raise();
        int ret1 = QMessageBox::question(this, tr("确认"), tr("确定退出前保存场景到桌面?"), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if(ret1 == QMessageBox::Yes){
            save2XmlFile();
            isSave = true;
            toggleSaveXml(0);
            ui->actionRunRadar->setEnabled(true);
            qDebug() << "保存退出";
            event->accept();
            MainWindowNew::main_radar_list.removeOne(this);
            //提醒父类更新列表
            emit iClose(this);

        }else if(ret1 == QMessageBox::No){
            qDebug() << "do not save";
            // 直接退出
            event->accept();
            MainWindowNew::main_radar_list.removeOne(this);
            //提醒父类更新列表
            emit iClose(this);
        }else {
            // 拒绝关闭
            qDebug() << "拒绝关闭!!!";
            event->ignore();
        }
    }else{
        event->accept();
        MainWindowNew::main_radar_list.removeOne(this);
        //提醒父类更新列表
        emit iClose(this);
    }
}

//组件层叠关系
void MainWindow_Radar::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
    isSave = false;
    toggleSaveXml(1);
    qDebug() << "xml由于组件上移一层而改变";
}

void MainWindow_Radar::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    // NOTE 这个排序应该是从浅入深
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    //只把selectedItem设置为最高的z-index
    selectedItem->setZValue(zValue);
    isSave = false;
    toggleSaveXml(1);
    qDebug() << "xml由于组件下移一层而改变";
}

//create相关
// This is available in all editors.
/**
* @projectName   prototype_v0902
* @brief         修改： 组件名修改
* @author        Antrn
* @date          2019-09-03
*/
void MainWindow_Radar::createCompBox()
{
    // FIXME 样式设置没用...
    ui->listWidget->setStyleSheet("QDockWidget#listWidget{border: 1px solid #FF00FF; border-radius: 5px");
    init5Comp();

    //-----------旧版使用buttonGroup-----------------------
    buttonGroup = new QButtonGroup(ui->dockWidgetContents);
    buttonGroup->setExclusive(true);
    //自定义的slot函数：与graphics交互的关键
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
    // TODO 支持点击修改组件名
    layout->addWidget(createCellWidget(tr("脉冲压缩"), DiagramItem::Comp1), 0, 0); // Component_1 0,0
    layout->addWidget(createCellWidget(tr("恒虚警率"), DiagramItem::Comp2), 0, 1); // Component_2 0,1
    layout->addWidget(createCellWidget(tr("输入"), DiagramItem::Comp3), 1, 1); // Component_3 1,1
    layout->addWidget(createCellWidget(tr("动目标检测"), DiagramItem::Comp4), 1, 0); // Component_4 1,0
    layout->addWidget(createCellWidget(tr("输出"), DiagramItem::Comp5), 2, 0); // Component_5 2,0
    // 文字按钮，在场景添加文字，暂时保留
    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/img/textpointer.png")));
    textButton->setIconSize(QSize(50, 50));
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    //1,0表示1行0列 TODO 文字
    ClickableLabel *text = new ClickableLabel(tr("Text"));
    // 能被选中
    connect(text, &ClickableLabel::lostFocus, text, &ClickableLabel::labelLostFocus);
    text->setTextInteractionFlags(Qt::TextSelectableByMouse);
    textLayout->addWidget(text, 1, 0, Qt::AlignCenter);

    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 2, 1); // 2,1

    //设置行和列的比例
    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    backgroundButtonGroup = new QButtonGroup(ui->dockWidgetContents);
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));

    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
                                                           ":/img/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
                                                           ":/img/background2.png"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
                                                           ":/img/background3.png"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
                                                           ":/img/background4.png"), 1, 1);
    backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);

    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);


    //========布局搜索框和listWidget=============
    QVBoxLayout *searchlayout = new QVBoxLayout;
    QHBoxLayout *pSearchLayout = new QHBoxLayout;
    m_pSearchLineEdit = new QLineEdit;

    word_list<<"Java"<<"C++"<<"Python"<<"脉冲压缩"<<"恒虚警率"<<"输入"<<"输出"<<"动目标检测"<<"FDPC"<<"MTD"<<"CFAR";
    QCompleter *completer = new QCompleter;
    string_list_model = new QStringListModel(word_list, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setModel(string_list_model);
    m_pSearchLineEdit->setCompleter(completer);
    // 自动补全
    connect(m_pSearchLineEdit, SIGNAL(editingFinished()), this, SLOT(editComplete()));

    m_pSearchLineEdit->setStyleSheet("height:24px;");
    m_pSearchLineEdit->setPlaceholderText(tr("输入组件名称"));
    QPushButton *pSearchButton = new QPushButton;
    pSearchButton->setCursor(Qt::PointingHandCursor);
    pSearchButton->setToolTip(QStringLiteral("搜索"));
    pSearchButton->setFixedSize(20, 20);
    pSearchButton->setStyleSheet("QPushButton{border-image:url(:/img/search.png); background:transparent;} \
                                         QPushButton:hover{border-image:url(:/img/search_hover.png)} \
                                         QPushButton:pressed{border-image:url(:/img/search_press.png)}");
    //防止文本框输入内容位于按钮之下
    QMargins margins = m_pSearchLineEdit->textMargins();
    m_pSearchLineEdit->setTextMargins(margins.left(), margins.top(), pSearchButton->width(), margins.bottom());
    m_pSearchLineEdit->setPlaceholderText(QStringLiteral("请输入搜索内容"));
    pSearchLayout->addStretch();
    pSearchLayout->addWidget(pSearchButton);
    pSearchLayout->setSpacing(0);
    pSearchLayout->setContentsMargins(0, 0, 0, 0);
    m_pSearchLineEdit->setLayout(pSearchLayout);

    connect(pSearchButton, SIGNAL(clicked(bool)), this, SLOT(search()));

    searchlayout->addWidget(m_pSearchLineEdit);
    searchlayout->addWidget(ui->listWidget);
    QWidget *bigW = new QWidget;
    searchlayout->setStretch(0,0);
    searchlayout->setStretch(1,12);
    bigW->setLayout(searchlayout);
    //========================

    QToolBox *toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(bigW, tr("组件列表"));
    toolBox->addItem(itemWidget, tr("基础雷达组件"));
    toolBox->addItem(backgroundWidget, tr("背景设置"));
    // TODO 如何只在组件打开的时候才显示属性框？或者可以将这三个分成三个小窗口
    ui->dockCompList->setWidget(toolBox);
}

void MainWindow_Radar::createActions()
{
    toFrontAction = new QAction(QIcon(":/img/bringtofront.png"), tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));

    sendBackAction = new QAction(QIcon(":/img/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/img/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    propertyAction = new QAction(QIcon(":/img/property.png"), tr("Property"), this);
    propertyAction->setShortcut(tr("Property"));
    propertyAction->setStatusTip(tr("Show item's properties"));
    connect(propertyAction, &QAction::triggered, this, &MainWindow_Radar::showItemProperties);

    exitAction = new QAction(QIcon(":/img/exit.png"), tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit radar edit window"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(false);
    QPixmap pixmap(":/img/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/img/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(false);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/img/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(false);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    //初始化为不可单击
    toggleSaveXml(0);
    ui->actionRunRadar->setEnabled(false);
}

void MainWindow_Radar::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addAction(propertyAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    // 比例条
    progressBar=new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
    // 运行时间
    label_time=new QLabel(tr("Running： "),this);
    // 把时间标签加在状态栏中
    ui->statusbar->addPermanentWidget(label_time);
    ui->statusbar->addPermanentWidget(progressBar);
    itemMenu->setEnabled(false);
}

void MainWindow_Radar::createToolbars()
{
    editToolBar = addToolBar(tr("Edit"));
    //默认不显示
    editToolBar->setHidden(true);
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    //bottom: 2 top: 64
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/img/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textButtonTriggered()));

    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/img/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(fillButtonTriggered()));

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/img/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()),
            this, SLOT(lineButtonTriggered()));

    textToolBar = addToolBar(tr("Font"));
    // 默认不显示
    textToolBar->setHidden(true);
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);

    colorToolBar = addToolBar(tr("Color"));
    // 默认不显示
    colorToolBar->setHidden(true);
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    //默认是指针移动
    pointerButton->setIcon(QIcon(":/img/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/img/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    //后面参数是id唯一标识
    pointerTypeGroup->addButton(pointerButton, int(RadarScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(RadarScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);//100%
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sceneScaleChanged(QString)));

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);

}

QWidget *MainWindow_Radar::createBackgroundCellWidget(const QString &text, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

QWidget *MainWindow_Radar::createCellWidget(const QString &text, DiagramItem::DiagramType type)
{
    DiagramItem item(type, itemMenu);
    QIcon icon(item.image());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

QMenu *MainWindow_Radar::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}

QIcon MainWindow_Radar::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centred horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}

QIcon MainWindow_Radar::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}

//视图快照
/**
* @projectName   prototype_v0719
* @brief         保存场景或者视图快照
* @author        Antrn
* @date          2019-08-12
*/
// NOTE 这里也是将保存的图片直接保存到默认文件夹，也就是当前桌面文件夹下，后期需要改动
void MainWindow_Radar::saveSnapshot(int flag)
{
    // 当前路径
    QString currDir = QDir::currentPath();
    // 桌面路径
    QString deskTop =  QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    switch (flag) {
        case 1: {
//            QSize mysize(static_cast<int>(scene->width()), static_cast<int>(scene->height()));
//            QImage image(mysize, QImage::Format_RGB32);
//            QPainter painter(&image);
//            painter.setRenderHint(QPainter::Antialiasing);
//            scene->render(&painter);//也可以使用视图保存，只保存视图窗口的快照
//            painter.end();

            QString dir_str = deskTop+"/snapshots/";
////            QString dir_str = currDir+"/snapshots/";
//            // 检查目录是否存在，若不存在则新建
//            QDir dir;
//            if (!dir.exists(dir_str))
//            {
//                dir.mkpath(dir_str);
//            }
//            image.save(dir_str+"scene.png");

            u->saveImage(flag, scene, nullptr, dir_str, "scene.png");
            break;
        }
        case 2: {
//            QSize mysize(static_cast<int>(ui->graphicsView->width()), static_cast<int>(ui->graphicsView->height()));
//            QImage image(mysize, QImage::Format_RGB32);
//            QPainter painter(&image);
//            painter.setRenderHint(QPainter::Antialiasing);
//            ui->graphicsView->render(&painter);//也可以使用视图保存，只保存视图窗口的快照
//            painter.end();

            QString dir_str = deskTop+"/snapshots/";
//            QString dir_str = currDir+"/snapshots/";
            // 检查目录是否存在，若不存在则新建
//            QDir dir;
//            if (!dir.exists(dir_str))
//            {
//                dir.mkpath(dir_str);
//            }
//            image.save(dir_str+"view.png");
            u->saveImage(flag, nullptr, ui->graphicsView, dir_str, "view.png");
            break;
        }
    }
}

//handleFontChange相关
void MainWindow_Radar::currentFontChanged(const QFont &)
{
    handleFontChange();
}

void MainWindow_Radar::fontSizeChanged(const QString &)
{
    handleFontChange();
}

void MainWindow_Radar::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
}


//ColorChanged相关
void MainWindow_Radar::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/img/textpointer.png",
                                     qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}

void MainWindow_Radar::itemColorChanged()
{
    isSave = false;
    toggleSaveXml(1);
    qDebug() << "组件颜色改变";
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/img/floodfill.png",
                                     qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}

void MainWindow_Radar::lineColorChanged()
{
    isSave = false;
    toggleSaveXml(1);
    qDebug() << "线段颜色改变";
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/img/linecolor.png",
                                     qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}

void MainWindow_Radar::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}

void MainWindow_Radar::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}

void MainWindow_Radar::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}

// xml相关
void MainWindow_Radar::on_actionOpenXml_triggered()
{
    QString dirpath = QDir::currentPath()+"/xmls/";
    QDir dir(dirpath);
    if(!dir.exists()){
        dir.mkdir(dirpath);//创建多级目录
    }
    // 打开xml文件读取
    const QString fileName = QFileDialog::getOpenFileName(this, tr("打开xml"), QString(dirpath), tr("xml files (*.xml)"));
    readXmlConf(fileName);
}

void MainWindow_Radar::readXmlConf(QString xmlname)
{
    QDomDocument doc;
    if(!xmlname.isEmpty()){
        QFile file(xmlname);
        if(!file.open(QIODevice::ReadOnly)) return;
        if(!doc.setContent(&file)){
            file.close();
            qDebug() << "打开失败";
            return;
        }
        file.close();
        //根元素component
        QDomElement docElem = doc.documentElement();
        int wid,hei;
        wid = docElem.attribute("width").toInt();
        hei = docElem.attribute("height").toInt();
        scene->setSceneRect(QRectF(0, 0, wid, hei));
        // 第一个孩子是Arrs或者Items
//        QDomNode n = docElem.firstChild();
        QDomNode itemNode = doc.elementsByTagName("Items").at(0);
        int id;
        // 子孩子就是标签名为comp_1...
        QDomNode m = itemNode.firstChild();
        while(!m.isNull()){
            std::string tagName = m.nodeName().toStdString();
            if(m.isElement()){
                // 每个元素item
                QDomElement e = m.toElement();
                id = e.attribute("id").toInt();
                qreal posx = e.attribute("pos_x").toInt();
                qreal poxy = e.attribute("pos_y").toInt();
                QString s = e.elementsByTagName("color").at(0).toElement().text();
                QColor colour(s);

                // 只能先用if/else了，switch也不能用
                DiagramItem::DiagramType type;
                qDebug() << "组件名: " << QString::fromStdString(tagName);

                // FIXME 这部分出大问题，不可能每次都枚举吧
//                if(tagName == "comp_1"){
//                    type = DiagramItem::DiagramType::Comp1;
//                }else if(tagName == "comp_2"){
//                    type = DiagramItem::DiagramType::Comp2;
//                }else if(tagName == "comp_3"){
//                    type = DiagramItem::DiagramType::Comp3;
//                }else if(tagName == "comp_4"){
//                    type = DiagramItem::DiagramType::Comp4;
//                }else {
//                    type = DiagramItem::DiagramType::Comp5;
//                }

                // 成功啦
                QMetaObject mo = DiagramItem::staticMetaObject;
                int index = mo.indexOfEnumerator("DiagramType");
                QMetaEnum metaEnum = mo.enumerator(index);
                // QMetaEnum metaEnum = QMetaEnum::fromType<DiagramItem::DiagramType>();
//                for (int i=0; i<metaEnum.keyCount(); ++i)
//                {
//                    qDebug() << metaEnum.key(i);
//                    qDebug()<<  metaEnum.valueToKey(2);              //  enum转string
//                    qDebug()<<  metaEnum.keysToValue("Comp4");       //  string转enum
//                }
                type = DiagramItem::DiagramType(metaEnum.keysToValue(tagName.c_str()));
                DiagramItem *item_ = new DiagramItem(type, scene->getItemMenu());
                QPointF pos(posx, poxy);
                item_->setPos(pos);
                item_->setBrush(colour);
                item_->itemId = id; //id不变
                scene->idList.append(id);
                qDebug() << "scene的id列表" << scene->idList;
                scene->addItem(item_);
                emit itemInserted(item_);
                //更新xml
                scene->modifyXmlItems(pos, item_);
            }
            m = m.nextSibling();
        }
        // 大的标签是Arrs的时候
        QDomNode arrowNode = doc.elementsByTagName("Arrs").at(0);
        // 就是arrow了，因为箭头就一种
        QDomNode m1 = arrowNode.firstChild();
        int start_item_id, end_item_id, arrow_id;

        // 遍历所有的箭头
        while(!m1.isNull()){
            if(m1.isElement()){
                // 每个元素item
                QDomElement e = m1.toElement();
                arrow_id = e.attribute("id").toInt();
                start_item_id = e.attribute("start_item_id").toInt();
                end_item_id = e.attribute("end_item_id").toInt();
                QString cs = e.elementsByTagName("color").at(0).toElement().text();
                //qDebug() << "箭头颜色： " << cs;
                QColor line_colour(cs);

                DiagramItem *startItem = getDiagramItemById(start_item_id);
                DiagramItem *endItem = getDiagramItemById(end_item_id);
                Arrow *arrow = new Arrow(startItem, endItem);
                arrow->setColor(line_colour);
                arrow->itemId = arrow_id; // id不变
                scene->idList.append(arrow_id);
                qDebug() << "scene的id列表" << scene->idList;
                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                arrow->setZValue(-1000.0);
                scene->addItem(arrow);
                arrow->updatePosition();

                scene->modifyXmlArrows(arrow, startItem, endItem);

            }
            m1 = m1.nextSibling();
        }
    }else {
        // TODO 文件名为空，啥也没选，提示
    }
}

DiagramItem *MainWindow_Radar::getDiagramItemById(int item_id)
{
    QList<QGraphicsItem *> items = scene->items();
    DiagramItem *im = nullptr;
    for (int i=0; i<items.size(); i++) {
        if((im = dynamic_cast<DiagramItem*>(items.at(i)))){
            if(item_id == im->itemId){
                //qDebug() << "找到了";
               return im;
            }
        }
    }
    // 如果没找到，理论上不会。。。
    return nullptr;
}

//触发保存
void MainWindow_Radar::on_actionsave_triggered()
{
    save2XmlFile();
}

// 保存为xml文件
// TODO 新建线程保存文件
void MainWindow_Radar::save2XmlFile(){
    ui->statusbar->showMessage("正在保存场景...", 1000);
    qDebug() << tr("to close radar window");
    // 根元素
    QDomElement rootElem = scene->getDoc().documentElement();
    rootElem.setAttribute("width", scene->width());
    rootElem.setAttribute("height", scene->height());
    //[更新箭头和组件的color
    DiagramItem *d;
    Arrow *a;
    // 遍历场景中的items
    for (int i=0; i<scene->items().size(); i++) {
        //转换为DiagramItem成功，只算item，排除箭头
        if((d = dynamic_cast<DiagramItem*>(scene->items().at(i)))){
    //                qDebug() << d->brush().color().name();
            // WARNING elementById()报错说没有实现
    //                if(d->brush().color().name() != scene->getDoc().elementById(QString::number(d->itemId)).firstChild().toElement().text()){
    //                    scene->getDoc().elementById(QString::number(d->itemId)).firstChild().setNodeValue(d->brush().color().name());
    //                }
            QDomNodeList list = scene->getDoc().elementsByTagName("Items").at(0).childNodes();
            for (int k=0; k<list.count(); k++) {
                QDomElement e = list.at(k).toElement();
    //                    qDebug() << e.attribute("id") << "; " << QString::number(d->itemId);
                //找到id对应的元素
                if(e.attribute("id").compare(QString::number(d->itemId))==0){
    //                        e.firstChild().setNodeValue(d->brush().color().name());
    //                        qDebug() << d->brush().color().name();
    //                        qDebug() << e.elementsByTagName("color").at(0).toElement().text();
                    e.elementsByTagName("color").at(0).toElement().firstChild().setNodeValue(d->brush().color().name());
                }
            }
        }
        // 如果是箭头
        if((a = dynamic_cast<Arrow *>(scene->items().at(i)))){
            QDomNodeList list = scene->getDoc().elementsByTagName("Arrs").at(0).childNodes();
            for (int k=0; k<list.count(); k++) {
                QDomElement e = list.at(k).toElement();
                //找到id对应的元素
                if(e.attribute("id").compare(QString::number(a->itemId))==0){
                    e.elementsByTagName("color").at(0).toElement().firstChild().setNodeValue(a->getColor());
                }
            }
        }
    }
    //]更新color

    // 保存雷达组件数据
    QString dirp = QDir::currentPath() + "/xmls/";

    // 弹窗用户选择存储路径， WARNING 用户不能自定义文件名
    switch (u->saveFile(this, dirp, getEquip_id()+".xml", scene, true)) {
        case 1:
            ui->statusbar->showMessage("保存成功", 1000);
            u->alert(geometry(), "场景保存成功!");
            break;
        case -1:
            ui->statusbar->showMessage("保存出错", 1000);
            u->alert(geometry(), tr("场景保存出错，请重新尝试!"));
            break;
        case 0:
            ui->statusbar->showMessage("保存失败", 1000);
            u->alert(geometry(), tr("场景保存失败！请选择存储位置后再保存!"));
            break;
    }
    // 不弹窗，自己保存到默认文件夹，如果用户也选择默认文件夹，那就是一个覆盖的事情
    u->saveFile(this, dirp, getEquip_id()+".xml", scene, false);

    // TODO 是否需要快照应该由用户决定，后期需要完善
//     saveSnapshot(1); //保存场景快照
    // saveSnapshot(2); //保存视图快照
    isSave = true;
    toggleSaveXml(0);
    ui->actionRunRadar->setEnabled(true);
//    QMessageBox qm;
//    qm.setText(tr("场景保存成功!"));
//    qm.exec();
//    u->alert(geometry(), tr("场景保存成功!"));
}

//打开自动读取已有的xml文件
void MainWindow_Radar::autoloadXmlById(QString id)
{
    QFileInfoList list = getFileList(QDir::currentPath()+"/xmls/");
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QString fname = fileInfo.fileName();
        if(fname.startsWith(id)){
            QString filepath;
            filepath.append(fileInfo.path());
            filepath+="/"+fname;
            qDebug() << "找到xml!" << filepath;
            readXmlConf(filepath);
            isSave = true;
            toggleSaveXml(0);
            qDebug() << "自动读取，不算是手动保存的操作，所以恢复已保存状态";
            ui->actionRunRadar->setEnabled(true);
            break;
        }
    }
}

QFileInfoList MainWindow_Radar::getFileList(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++)
    {
         QString name = folder_list.at(i).absoluteFilePath();
         QFileInfoList child_file_list = getFileList(name);
         file_list.append(child_file_list);
    }

    return file_list;
}

//进度条相关
// 当已经开始运行的时候
void MainWindow_Radar::On_start()
{
    // 关闭几个按钮和动作
    ui->actionRunRadar->setEnabled(false);
    // 打开停止按钮和动作
//    ui->action_Stop->setEnabled(true);
    // 进度条为0
    progressBar->setValue(0);
}

// 当运行结束的时候
void MainWindow_Radar::On_over()
{
//    ui->action_Stop->setEnabled(false);
    ui->actionRunRadar->setEnabled(true);
    label_time->setText(tr("Done"));
}

// 当对信号进行评估进度的时候，实时设置进度条的值
void MainWindow_Radar::On_rateSignal(float rate)
{
    progressBar->setValue(int(rate));
}

// 当isSave标志变为false时就把保存xml按钮置为可点击
void MainWindow_Radar::On_isSave2False(QString message)
{
    // 运行时未保存，需要弹窗
    if(!message.isNull()&&!message.isEmpty()){
        int res = QMessageBox::question(this, tr("提示"), message, QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if(res == QMessageBox::Yes){
            qDebug() << "save";
            save2XmlFile();
            isSave = true;
            toggleSaveXml(0);
            ui->actionRunRadar->setEnabled(true);
            return;
        }else if(res == QMessageBox::No){
            qDebug() << "do not save";
            // 直接退出
        }else {
            // 拒绝关闭
            qDebug() << "拒绝关闭!!!";
        }
    }
    ui->actionsave->setEnabled(true);
}

void MainWindow_Radar::update_Comp_property(QString name)
{
    ui->lineEdit_CompName->setText(name);
}

void MainWindow_Radar::setComp_typeandMode(int id)
{
    scene->setItemType(DiagramItem::DiagramType(id));
    scene->setMode(RadarScene::InsertItem);
}

// 搜索框
void MainWindow_Radar::search()
{
    QString strText = m_pSearchLineEdit->text();
    if (!strText.isEmpty())
    {
//        QMessageBox::information(this, QStringLiteral("搜索"), QStringLiteral("搜索内容为%1").arg(strText));

        ui->listWidget->clear();
        loadCompByName(strText);
    }else{
        loadAllComps();
    }
}

void MainWindow_Radar::editComplete()
{
    QString text = m_pSearchLineEdit->text();
    if(QString::compare(text, QString("")) != 0) {
        bool is_contains = word_list.contains(text, Qt::CaseInsensitive);
        if(!is_contains) {
           word_list<<text;
           string_list_model->setStringList(word_list);
           //completer->setModel(new QStringListModel(wordList, this));
        }
    }
}

QString MainWindow_Radar::getEquip_id() const
{
    return equip_id;
}

void MainWindow_Radar::setEquip_id(const QString &value)
{
    equip_id = value;
}

//左边组件窗口显示与隐藏
void MainWindow_Radar::on_actionCom_list_triggered()
{
    //显示工具箱（文本编辑器）
    if(ui->dockCompList->isVisible()){
        ui->dockCompList->close();
    }else{
        ui->dockCompList->show();
    }
}
//左下角XY
void MainWindow_Radar::xy_show(double x, double y)
{
    QString str=QString("x=%1,y=%2").arg(QString::number(x)).arg(QString::number(y));
    label_xy->setText(str);
}

MainWindow_Radar::~MainWindow_Radar()
{
    delete ui;
}
