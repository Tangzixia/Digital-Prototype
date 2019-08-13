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
#include "arrow.h"

const int InsertTextButton = 10;
/**
* @projectName   prototype_v0719
* @brief         编辑雷达功能的主窗口。
* @author        Antrn
* @date          2019-08-12
*/
MainWindow_Radar::MainWindow_Radar(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_Radar)
{
    ui->setupUi(this);
    //TODO ?
    this->setAttribute(Qt::WA_DeleteOnClose);
    //最大窗口
    showMaximized();
    //组件列表
    ui->actionCom_list->setCheckable(true);
    ui->actionCom_list->setChecked(true);

    createActions();
    createCompBox();
    createMenus();
    scene = new RadarScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(itemInserted(DiagramItem*)),
            this, SLOT(itemInserted(DiagramItem*)));
    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
            this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
            this, SLOT(itemSelected(QGraphicsItem*)));
    createToolbars();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(ui->dockCompList);
    ui->graphicsView->setScene(scene);
    layout->addWidget(ui->graphicsView);


    // xy坐标标签
    label_xy=new QLabel(this);
    // 状态栏左边加入xy坐标
    ui->statusbar->addWidget(label_xy);
    // 设置鼠标跟踪开启
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setAcceptDrops(true);
    //设置可以使鼠标在视图上拖拽出橡皮筋框选择多个组件
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    connect(scene,SIGNAL(signal_xy(double,double)),this,SLOT(xy_show(double,double)));

    //抓手
    //ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag );

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    //TODO dock无法自由拖动了，只能保持固定宽度能拖出来但是不能自动贴边
//    ui->dockCompList->setAllowedAreas(Qt::AllDockWidgetAreas);
    setWindowTitle(tr("Radar Edit"));
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow_Radar::~MainWindow_Radar()
{
    delete ui;
}

// BUG WARNING 退出的时候应该让用户选择是否保存当前场景，默认保存场景到my.xml好像有点不合理
// 先判断是否退出钱保存？是，则保存退出；否，则直接退出。
void MainWindow_Radar::closeEvent(QCloseEvent *event)
{
    int ret1 = QMessageBox::question(this, tr("确认"), tr("确定退出雷达编辑?"), QMessageBox::Yes, QMessageBox::No);
    if(ret1 == QMessageBox::Yes){
        qDebug() << tr("close radar window");
        QDomElement rootElem = scene->getDoc().documentElement();
        rootElem.setAttribute("width", scene->width());
        rootElem.setAttribute("height", scene->height());
        //[更新color
        DiagramItem *d;
        Arrow *a;
        for (int i=0; i<scene->items().size(); i++) {
            //转换为DiagramItem成功，只算item，排除箭头
            if((d = dynamic_cast<DiagramItem*>(scene->items().at(i)))){
//                qDebug() << d->brush().color().name();
                // BUG [light] elementById()报错说没有实现
//                if(d->brush().color().name() != scene->getDoc().elementById(QString::number(d->itemId)).firstChild().toElement().text()){
//                    scene->getDoc().elementById(QString::number(d->itemId)).firstChild().setNodeValue(d->brush().color().name());
//                }
                QDomNodeList list = scene->getDoc().elementsByTagName("components").at(0).childNodes();
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

            if((a = dynamic_cast<Arrow *>(scene->items().at(i)))){
                QDomNodeList list = scene->getDoc().elementsByTagName("components").at(0).childNodes();
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
        // NOTE 保存雷达组件数据
        QFile file(QDir::currentPath() + "/my.xml");
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;
        QTextStream out(&file);
        scene->getDoc().save(out, 4);
        file.close();
        // TODO是否需要快照应该由用户决定，后期需要完善
        saveSnapshot(1); //保存场景快照
        saveSnapshot(2); //保存视图快照
        event->accept();
    }else{
        // 直接退出
        event->accept();
    }

}

void MainWindow_Radar::on_actionCom_list_triggered()
{
    //显示工具箱（文本编辑器）
    if(ui->dockCompList->isVisible()){
        ui->dockCompList->close();
    }else{
        ui->dockCompList->show();
    }
}

void MainWindow_Radar::xy_show(double x, double y)
{
    //x-=9;
    //y-=9;
    //(9,9,xx,xx)要减去9
    //qDebug() << ui->graphicsView->geometry();
    //qDebug() << this->graphicsScene->width() << " ; " << this->graphicsScene->height() ;
    QString str=QString("x=%1,y=%2").arg(QString::number(x)).arg(QString::number(y));
    label_xy->setText(str);
}

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
    if (id == InsertTextButton) {
        scene->setMode(RadarScene::InsertText);
    } else {
        scene->setItemType(DiagramItem::DiagramType(id));
        scene->setMode(RadarScene::InsertItem);
    }
}

void MainWindow_Radar::deleteItem()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Arrow::Type) {
            scene->removeItem(item);
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, scene->selectedItems()) {
         if (item->type() == DiagramItem::Type)
             qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
         scene->removeItem(item);
         delete item;
    }
}

void MainWindow_Radar::pointerGroupClicked(int )
{
    scene->setMode(RadarScene::Mode(pointerTypeGroup->checkedId()));
}

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
}

void MainWindow_Radar::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    //只把selectedItem设置为最高的z-index
    selectedItem->setZValue(zValue);
}

void MainWindow_Radar::itemInserted(DiagramItem *item)
{
    //当有图形项插入到场景中的时候，应该将指针组改为移动指针
    pointerTypeGroup->button(int(RadarScene::MoveItem))->setChecked(true);
    scene->setMode(RadarScene::Mode(pointerTypeGroup->checkedId()));
    //取消原按钮的选中状态
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}

void MainWindow_Radar::textInserted(QGraphicsTextItem *)
{
    //取消选中状态
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(RadarScene::Mode(pointerTypeGroup->checkedId()));
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

void MainWindow_Radar::createCompBox()
{
    buttonGroup = new QButtonGroup(ui->dockWidgetContents);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Component_1"), DiagramItem::Comp1), 0, 0);
    layout->addWidget(createCellWidget(tr("Component_2"), DiagramItem::Comp2),0, 1);
    layout->addWidget(createCellWidget(tr("Component_3"), DiagramItem::Comp4), 1, 0);
    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/img/textpointer.png")));
    textButton->setIconSize(QSize(50, 50));
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    //1,0表示1行0列
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 2, 0);

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

    QToolBox *toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Basic Radar Components"));
    toolBox->addItem(backgroundWidget, tr("Backgrounds"));
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

    exitAction = new QAction(QIcon(":/img/exit.png"), tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit radar edit window"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/img/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/img/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/img/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));
}

void MainWindow_Radar::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);
}

void MainWindow_Radar::createToolbars()
{
    editToolBar = addToolBar(tr("Edit"));
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
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);

    colorToolBar = addToolBar(tr("Color"));
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

/**
* @projectName   prototype_v0719
* @brief         保存场景或者视图快照
* @author        Antrn
* @date          2019-08-12
*/
// TODO 这里也是将保存的图片直接保存到默认文件夹，也就是当前工程文件夹下，后期需要改动
void MainWindow_Radar::saveSnapshot(int flag)
{
    QString currDir = QDir::currentPath();
    switch (flag) {
        case 1: {
            QSize mysize(static_cast<int>(scene->width()), static_cast<int>(scene->height()));
            QImage image(mysize, QImage::Format_RGB32);
            QPainter painter(&image);
            painter.setRenderHint(QPainter::Antialiasing);
            scene->render(&painter);//也可以使用视图保存，只保存视图窗口的快照
            painter.end();
            image.save(currDir+"/scene.png");
            break;
        }
        case 2: {
            QSize mysize(static_cast<int>(ui->graphicsView->width()), static_cast<int>(ui->graphicsView->height()));
            QImage image(mysize, QImage::Format_RGB32);
            QPainter painter(&image);
            painter.setRenderHint(QPainter::Antialiasing);
            ui->graphicsView->render(&painter);//也可以使用视图保存，只保存视图窗口的快照
            painter.end();
            image.save(currDir+"/view.png");
            break;
        }
    }
}
void MainWindow_Radar::currentFontChanged(const QFont &)
{
    handleFontChange();
}

void MainWindow_Radar::fontSizeChanged(const QString &)
{
    handleFontChange();
}

void MainWindow_Radar::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = ui->graphicsView->matrix();
    ui->graphicsView->resetMatrix();
    ui->graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
    ui->graphicsView->scale(newScale, newScale);
}

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
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/img/floodfill.png",
                                     qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}

void MainWindow_Radar::lineColorChanged()
{
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

/**
* @projectName   prototype_v0719
* @brief         读取xml
* @author        Antrn
* @date          2019-08-13
*/
// WARNING BUG 有一个大bug，当按照此函数读取xml进来之后，再退出会清空xml
void MainWindow_Radar::on_actionOpenXml_triggered()
{
    // TODO 打开xml文件读取
    const QString fileName = QFileDialog::getOpenFileName(this, tr("打开xml"), QString(QDir::currentPath()), tr("xml files (*.xml)"));
    QDomDocument doc;
    if(!fileName.isEmpty()){
        QFile file(fileName);
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
        QDomNode n = docElem.firstChild();
        QList<int> idList;
        while(!n.isNull()){
            // Items
            if(n.toElement().tagName().compare(QString("Items")) == 0){
                QDomNode m = n.firstChild();
                while(!m.isNull()){
                    qDebug() << m.nodeName();
                    if(m.isElement()){
                        //每个元素item
                        QDomElement e = m.toElement();
                        idList.append(e.attribute("id").toInt());
                        int posx = e.attribute("pos_x").toInt();
                        int poxy = e.attribute("pos_y").toInt();
                        QString s = e.elementsByTagName("color").at(0).toElement().text();
                        QColor colour(s);
                        // FIXME type写死了
                        DiagramItem::DiagramType type = DiagramItem::Comp1;
                        DiagramItem *item_ = new DiagramItem(type, scene->getItemMenu());
                        item_->setPos(QPoint(posx, poxy));
                        item_->setBrush(colour);
                        scene->addItem(item_);
                        emit itemInserted(item_);
                    }
                    m = m.nextSibling();
                }
            }else{
                // TODO 是Arrs的时候怎么操作
            }
            n = n.nextSibling();
        }
    }else {
        // TODO 什么操作
    }
}
