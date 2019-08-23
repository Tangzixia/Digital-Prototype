#include "ecmitem.h"
#include "mainwindow_radar.h"
#include "radarproperty.h"
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QMenu>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>

/**
* @projectName   prototype_v0719
* @brief         整体仿真中的对抗组件类。
* @author        zhengyuming
* @date          2019-08-20
*/

ECMItem::ECMItem()
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsMovable);
}

ECMItem::~ECMItem()
{

}

QMainWindow* ECMItem::getMainWindow()
{
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
            return mainWin;
    return nullptr;
}


QRectF ECMItem::boundingRect() const
{
    qreal adjust=0.5;
    return QRectF(0-adjust,0-adjust,70+adjust,70+adjust);
}

void ECMItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawRect(0,0,70,70);
    painter->drawPixmap(11,11,48,48, QPixmap(":/img/ecmdemo.jpg"));
}

void ECMItem::delete_item()
{
    qDebug()<<"删除对抗"<<endl;
    this->destroyed();
}

void ECMItem::show_property()
{
    qDebug() << "show Property";
    RadarProperty *p = new RadarProperty();
    p->exec();
}

void ECMItem::edit_radar()
{
    MainWindow_Radar *main_radar = new MainWindow_Radar;
    //connect(this, &RadarItem::close_mainwindow, getMainWindow(), &QMainWindow::hide);
    //emit close_mainwindow();
    main_radar->show();
}

void ECMItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "鼠标点击对抗了";
    setFocus();
    setSelected(true);
    setCursor(Qt::ClosedHandCursor);
    event->accept();
    QGraphicsItem::mousePressEvent(event);
}

void ECMItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    show_property();
    QGraphicsItem::mouseDoubleClickEvent (event );
}

void ECMItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu *menu = new QMenu;
    menu->addAction(tr("删除"), this, SLOT(delete_item()));
    // FIXME 此处和雷达共用属性窗口
    menu->addAction(tr("属性"), this, SLOT(show_property()));
    // FIXME 此处和雷达共用编辑窗口
    menu->addAction(tr("编辑"), this, SLOT(edit_radar()));
    menu->exec(event->screenPos());
    delete menu;
}
