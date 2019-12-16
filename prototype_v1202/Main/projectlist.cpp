#include "mainwindownew.h"
#include "projectlist.h"
#include "ui_projectlist.h"
#include <QMenu>
#include <QMouseEvent>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>

ProjectList::ProjectList(QWidget *parent):
    QListWidget (parent),
    ui(new Ui::ProjectList)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::NoFocus);// 去除item选中时的虚线边框
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//水平滚动条关闭
    initMenu();
    this->setStyleSheet(
                "QListWidget{color:rgb(0,0,0);background:rgb(255,255,255);border:0px solid gray;}"
                "QListWidget::item {height:30px;}"
                "QListWidget::item:hover {background: #ddd;}"
                "QListWidget::item:selected {background: #eee;color: #000;}"
                "QListWidget::item:selected:active{background:#00FFFFFF;color:#FFFFFF;border-width:0;}");
    this->setSpacing(5);
}

void ProjectList::initMenu(){
    blankMenu = new QMenu;
    // 点击空白处没反应，没添加任何action
    projectMenu = new QMenu();
    QAction *openAction = new QAction(tr("打开项目"), this);
    openAction->setIcon(QIcon()); // TODO 没填icon地址，应该和pro_head一样
    projectMenu->addAction(openAction);
//    QListWidgetItem *recentPro = new QListWidgetItem;
//    recentPro->setIcon(QIcon(":img/project.png"));
//    recentPro->setText(tr("prototype_v0906"));
//    recentPro->setToolTip(tr("build-prototype_v0906-Desktop_Qt_5_9_8_MinGW_32bit-Debug/debug"));
//    QListWidgetItem *recentPro1 = new QListWidgetItem;
//    recentPro1->setIcon(QIcon(":img/project.png"));
//    recentPro1->setText(tr("QtDirectUI"));
//    recentPro1->setToolTip(tr("E:/MasterGraduateStudent/Laboratory/723/Qt_demos/QtFramelessWindow-master"));
//    QListWidgetItem *recentPro2 = new QListWidgetItem;
//    recentPro2->setIcon(QIcon(":img/project.png"));
//    recentPro2->setText(tr("SARibbonBar"));
//    recentPro2->setToolTip(tr("E:/MasterGraduateStudent/Laboratory/723/Qt_demos/SARibbon-master"));
//    QListWidgetItem *recentPro3 = new QListWidgetItem;
//    recentPro3->setIcon(QIcon(":img/project.png"));
//    recentPro3->setText(tr("DesktopSnow"));
//    recentPro3->setToolTip(tr("E:/MasterGraduateStudent/Laboratory/723/Qt_demos/QT/DesktopSnow/DesktopSnow.pro"));
//    QListWidgetItem *recentPro4 = new QListWidgetItem;
//    recentPro4->setIcon(QIcon(":img/project.png"));
//    recentPro4->setText(tr("dictionary-reborn-master"));
//    recentPro4->setToolTip(tr("E:/MasterGraduateStudent/Laboratory/723/Qt_demos/QT/dictionary-reborn-master/dictionary-reborn-master.pro"));
//    QListWidgetItem *recentPro5 = new QListWidgetItem;
//    recentPro5->setIcon(QIcon(":img/project.png"));
//    recentPro5->setText(tr("RadarSearch"));
//    recentPro5->setToolTip(tr("E:/MasterGraduateStudent/Laboratory/723/Project-10-23/RadarSearch/RadarSearch.pro"));
//    QListWidgetItem *recentPro6 = new QListWidgetItem;
//    recentPro6->setIcon(QIcon(":img/project.png"));
//    recentPro6->setText(tr("DesktopSnow"));
//    recentPro6->setToolTip(tr("E:/MasterGraduateStudent/Laboratory/723/Qt_demos/QT/DesktopSnow/DesktopSnow.pro"));
//    QListWidgetItem *recentPro7 = new QListWidgetItem;
//    recentPro7->setIcon(QIcon(":img/project.png"));
//    recentPro7->setText(tr("dictionary-reborn-master"));
//    recentPro7->setToolTip(tr("E:/MasterGraduateStudent/Laboratory/723/Qt_demos/QT/dictionary-reborn-master/dictionary-reborn-master.pro"));
//    QListWidgetItem *recentPro8 = new QListWidgetItem;
//    recentPro8->setIcon(QIcon(":img/project.png"));
//    recentPro8->setText(tr("RadarSearch"));
//    recentPro8->setToolTip(tr("E:/MasterGraduateStudent/Laboratory/723/Project-10-23/RadarSearch/RadarSearch.pro"));
//    QListWidgetItem *recentPro9 = new QListWidgetItem;
//    recentPro9->setIcon(QIcon(":img/project.png"));
//    recentPro9->setText(tr("Opengltest"));
//    recentPro9->setToolTip(tr("E:/MasterGraduateStudent/Laboratory/723/Project-10-23/Opengltest_QT5/Opengltest.pro"));
//    this->addItem(recentPro);
//    this->addItem(recentPro1);
//    this->addItem(recentPro2);
//    this->addItem(recentPro3);
//    this->addItem(recentPro4);
//    this->addItem(recentPro5);
//    this->addItem(recentPro6);
//    this->addItem(recentPro7);
//    this->addItem(recentPro8);
//    this->addItem(recentPro9);
    // 换种方式，自定义list使用item和widget绑定
    createListWidgetItem("Opengltest", "E:/MasterGraduateStudent/Laboratory/723/Project-10-23/Opengltest_QT5/Opengltest.pro");
    createListWidgetItem("dictionary-reborn-master", "E:/MasterGraduateStudent/Laboratory/723/Qt_demos/QT/dictionary-reborn-master/dictionary-reborn-master.pro");
    createListWidgetItem("DesktopSnow", "E:/MasterGraduateStudent/Laboratory/723/Qt_demos/QT/DesktopSnow/DesktopSnow.pro");
    createListWidgetItem("QtDirectUI", "E:/MasterGraduateStudent/Laboratory/723/Qt_demos/QtFramelessWindow-master");
    createListWidgetItem("prototype_v0906", "build-prototype_v0906-Desktop_Qt_5_9_8_MinGW_32bit-Debug/debug");
    createListWidgetItem("Opengltest", "E:/MasterGraduateStudent/Laboratory/723/Project-10-23/Opengltest_QT5/Opengltest.pro");
    createListWidgetItem("dictionary-reborn-master", "E:/MasterGraduateStudent/Laboratory/723/Qt_demos/QT/dictionary-reborn-master/dictionary-reborn-master.pro");
    createListWidgetItem("DesktopSnow", "E:/MasterGraduateStudent/Laboratory/723/Qt_demos/QT/DesktopSnow/DesktopSnow.pro");
    createListWidgetItem("QtDirectUI", "E:/MasterGraduateStudent/Laboratory/723/Qt_demos/QtFramelessWindow-master");
    createListWidgetItem("prototype_v0906", "build-prototype_v0906-Desktop_Qt_5_9_8_MinGW_32bit-Debug/debug");
}

// This is an auto-generated comment.
/**
* @projectName   prototype_v0906
* @brief         简介 及定义一个函数用来生成最近项目 name:显示项目名称 desc：项目路径
* @author        Antrn
* @date          2019-11-29
*/
void ProjectList::createListWidgetItem(QString name, QString desc)
{
    QWidget *widget = new QWidget(this);

    //创建自定义的item
    widget->setStyleSheet("background:transparent;");

    QLabel *TypeLabel = new QLabel(widget);
    QPixmap myPix(QStringLiteral(":img/project.png"));
    TypeLabel->setFixedSize(40, 32);
    TypeLabel->setPixmap(myPix);
    TypeLabel->setScaledContents(true);
    TypeLabel->setStyleSheet("QLabel{padding-left:2px;padding-right:10px;}");

    QWidget *frontArea = new QWidget(widget);
    frontArea->setFixedHeight(32);

    QLabel *TextLabel = new QLabel(frontArea);
    TextLabel->setStyleSheet("QLabel{color:green;}\
    QLabel:hover{color:blue;}");
    TextLabel->setText(name);

    QLabel *TotalCopyLabel = new QLabel(frontArea);
    TotalCopyLabel->setText(desc);

    QPushButton *CloseButton = new QPushButton(widget);
    CloseButton->setToolTip(QStringLiteral("关闭"));
    // WARNING 这里设置Icon无效，只能在background设置
    CloseButton->setFixedSize(24, 24);
    CloseButton->setStyleSheet("QPushButton{ margin-left:0px;margin-right:0px;border:none;color:white;background-image: url(:img/close.png);background-repeat:no-repeat;}"
                               "QPushButton:hover{color:#FFFFFF; background-image: url(:img/close-hover.png);background-repeat:no-repeat; }"
                               "QPushButton:pressed{ color:white; background-image: url(:img/close-press.png);background-repeat:no-repeat; }");
    connect(CloseButton, SIGNAL(clicked()), this, SLOT(HideItem()));

    QVBoxLayout *verLayout = new QVBoxLayout;
    verLayout->setContentsMargins(0, 0, 0, 0);
    verLayout->setMargin(0);
    verLayout->setSpacing(0);
    verLayout->addWidget(TextLabel);
    verLayout->addWidget(TotalCopyLabel);
    frontArea->setLayout(verLayout);


    QHBoxLayout *horLayout = new QHBoxLayout;
    horLayout->setContentsMargins(0, 0, 0, 0);
    horLayout->setMargin(0);
    horLayout->setSpacing(0);
    horLayout->addWidget(TypeLabel);
    horLayout->addWidget(frontArea);
    horLayout->addWidget(CloseButton);
    widget->setLayout(horLayout);

    //将widget作为列表的item
    QListWidgetItem *ITEM = new QListWidgetItem();
    QSize size = ITEM->sizeHint();
    ITEM->setSizeHint(QSize(size.width(), 40));
    ITEM->setToolTip(desc);
    this->addItem(ITEM);
    widget->setSizeIncrement(size.width(), 40);
    this->setItemWidget(ITEM, widget);
}

void ProjectList::HideItem()
{
    qDebug() << "关闭一条项目";
    QList<QListWidgetItem*> list = this->selectedItems();

    if(list.size() == 0)
        return;

     QListWidgetItem* sel = list[0];
    if (sel)
    {
        int r = this->row(sel);
        this->takeItem(r);
    }
}

// 鼠标点击事件
void ProjectList::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event); // 如果不调用基类mousePressEvent，item被select会半天不响应,调用父类，让QSS起效，因为QSS基于父类QListWidget，子类就是子窗口，就是最上层窗口，是覆盖在父窗口上的，所以先于父窗口捕获消息
    currentItem = this->itemAt(mapFromGlobal(QCursor::pos())); // 鼠标位置的Item，不管右键左键都获取
    if(event->button()==Qt::LeftButton && currentItem!=nullptr) // 如果点击的左键
    {
        // 进入当前item对应的项目面板
        qDebug() << "当前项目： " << currentItem->text();
        MainWindowNew* mainnew = new MainWindowNew;
        mainnew->show();
    }
}

//菜单事件，为了显示菜单，点击鼠标右键响应，鼠标点击事件mousePressEvent优先于contextMenuEvent
void ProjectList::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidget::contextMenuEvent(event);           //调用基类事件
    if(currentItem==nullptr)                           //如果点击到的是空白处
    {
        qDebug() << "右键空白";
//        blankMenu->exec(QCursor::pos());
        return;
    }
    projectMenu->exec(QCursor::pos());
}

