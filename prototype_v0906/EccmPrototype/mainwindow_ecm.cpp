#include "mainwindow_ecm.h"
#include "ui_mainwindow_ecm.h"

#include<QLabel>
#include <QDebug>
#include<QGraphicsScene>
#include "GraphicsScenePublic.h"
#include <QToolButton>
MainWindow_ECM::MainWindow_ECM(QString id , QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_ECM)
{
    ui->setupUi(this);

    //控制台打印id："电子对抗2"
    qDebug() <<"打开了"+ id ;
    //TODO 用法不清楚
    this->setAttribute(Qt::WA_DeleteOnClose);
    //设置刚打开全屏显示
   this->showMaximized();
    //使用公共场景
//   GraphicsScenePublic *sceneECM = new GraphicsScenePublic(this->itemMenu);
    sceneECM= new GraphicsScenePublic(this->itemMenu);
    //设置图形窗体大小（不设拖控件会出问题）
    sceneECM->setSceneRect(QRectF(0,0,5000,5000));
    // 用来显示xy坐标的标签
    label_xy=new QLabel(this);
    // 状态栏左下角加入xy坐标
    ui->statusbar->addWidget(label_xy);
    // 设置鼠标跟踪开启
    ui->graphicsView_ecm->setMouseTracking(true);

    ui->graphicsView_ecm->setAcceptDrops(true);
    // 鼠标移动触发xy更新
    connect(sceneECM,&GraphicsScenePublic::signal_xy,this,[=](double x,double y){
        this->xy_show(x,y);
    });
    connect(sceneECM, SIGNAL(itemInserted(DiagramItem*)),
            this, SLOT(itemInserted(DiagramItem*)));
//    connect(sceneECM, SIGNAL(textInserted(QGraphicsTextItem*)),
//            this, SLOT(textInserted(QGraphicsTextItem*)));
//    connect(sceneECM, SIGNAL(itemSelected(QGraphicsItem*)),
//            this, SLOT(itemSelected(QGraphicsItem*)));
    connect(sceneECM,SIGNAL(startRun()),
            this,SLOT(On_start()));
    connect(sceneECM,SIGNAL(overRun()),
            this,SLOT(On_over()));
    ui->graphicsView_ecm->setScene(sceneECM);

    //设置可以使鼠标在视图上拖拽出橡皮筋框选择多个组件
    ui->graphicsView_ecm->setDragMode(QGraphicsView::RubberBandDrag);


    //加入算法组件
    createCompBox();
}
MainWindow_ECM::~MainWindow_ECM()
{
    delete ui;
}

//设置左下角坐标
void MainWindow_ECM::xy_show(double x, double y)
{
    QString str=QString("x=%1,y=%2").arg(QString::number(x)).arg(QString::number(y));
    label_xy->setText(str);
}

void MainWindow_ECM::createCompBox(){
    buttonGroup = new QButtonGroup(ui->dockWidgetContents);
    buttonGroup->setExclusive(true);
    //表格布局
    QGridLayout *layout = new QGridLayout;
    // TODO 支持点击修改组件名
    layout->addWidget(createCellWidget(tr("*脉冲压缩"), DiagramItem::Comp1), 0, 0); // Component_1
    layout->addWidget(createCellWidget(tr("*恒虚警率"), DiagramItem::Comp2),0, 1); // Component_2
    layout->addWidget(createCellWidget(tr("*动目标检测"), DiagramItem::Comp4), 1, 0); // Component_4
    layout->addWidget(createCellWidget(tr("输入"), DiagramItem::Comp3), 1, 1); // Component_3
    layout->addWidget(createCellWidget(tr("输出"), DiagramItem::Comp5), 2, 0); // Component_5

    //设置行和列的比例
    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    ui->listWidget_arithmetic->setLayout(layout);
    //自定义的slot函数：与graphics交互的关键（id--点的哪个传哪个 QString类型）
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
};

QWidget *MainWindow_ECM::createCellWidget(const QString &text, DiagramItem::DiagramType type)
    {
        DiagramItem item(type, itemMenu);
        QIcon icon(item.image());
        //专门用来显示图片的按钮组件
        QToolButton *button = new QToolButton;
        button->setIcon(icon);
        button->setIconSize(QSize(50, 50));
        button->setCheckable(true);
        buttonGroup->addButton(button, int(type));
       //名称和按钮在一个表格布局里
        QGridLayout *layout = new QGridLayout;
        layout->addWidget(button, 0, 0, Qt::AlignHCenter);
        layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);
        QWidget *widget = new QWidget();
        widget->setLayout(layout);
    //返回一个含有名称的toolbutton---->是否该换成自已控件？
        return widget;
    }

void MainWindow_ECM::buttonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons) {
        if (buttonGroup->button(id) != button)
            button->setChecked(false);
    }
    //判断是否是文本框
    if (id == 10) {//InsertTextButton=10 我没定义
        sceneECM->setMode(GraphicsScenePublic::InsertText);
    } else {
        sceneECM->setItemType(DiagramItem::DiagramType(id));
        sceneECM->setMode(GraphicsScenePublic::InsertItem);
    }
}
//按理说应该修改样式让被选中的算法组件明显一点，还没做
void MainWindow_ECM::itemSelected(QGraphicsItem *item)
{
    qDebug()<<"itemSelected";
//    DiagramTextItem *textItem =
//    qgraphicsitem_cast<DiagramTextItem *>(item);

//    QFont font = textItem->font();
//    fontCombo->setCurrentFont(font);
//    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
//    boldAction->setChecked(font.weight() == QFont::Bold);
//    italicAction->setChecked(font.italic());
//    underlineAction->setChecked(font.underline());
}

void MainWindow_ECM::itemInserted(DiagramItem *item)
{

//    isSave = false;
    qDebug() << "ECM item Inserted finish";
    //当有图形项插入到场景中的时候，应该将指针组改为移动指针
//    pointerTypeGroup->button(int(GraphicsScenePublic::MoveItem))->setChecked(true);
    //设置成上面那个
//   sceneECM->setMode(GraphicsScenePublic::Mode(pointerTypeGroup->checkedId()));
   sceneECM->setMode(GraphicsScenePublic::MoveItem);
    //取消原按钮的选中状态
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
    ui->listWidget_arithmetic->clear();
}

void MainWindow_ECM::sceneScaleChanged(const QString &scale)
{
//    isSave = false;
    qDebug() << "1117";
   // double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
//    QMatrix oldMatrix = ui->graphicsView->matrix();
//    ui->graphicsView->resetMatrix();
//    ui->graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
//    ui->graphicsView->scale(newScale, newScale);
}
