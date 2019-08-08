#include "mainwindow_radar.h"
#include "radaritem.h"
#include "radarproperty.h"
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QMenu>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
RadarItem::RadarItem(): QGraphicsItem()
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsMovable);
}

RadarItem::~RadarItem()
{

}

QMainWindow* RadarItem::getMainWindow()
{
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
            return mainWin;
    return nullptr;
}


QRectF RadarItem::boundingRect() const
{
    qreal adjust=0.5;
    return QRectF(0-adjust,0-adjust,70+adjust,70+adjust);
}

void RadarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawRect(0,0,70,70);
    painter->drawPixmap(11,11,48,48, QPixmap(":/img/radar.png"));
}

void RadarItem::delete_item()
{
    this->destroyed();
}

void RadarItem::show_property()
{
    qDebug() << "show Property";
    RadarProperty *p = new RadarProperty();
    p->exec();
}

void RadarItem::edit_radar()
{
    MainWindow_Radar *main_radar = new MainWindow_Radar;
    //connect(this, &RadarItem::close_mainwindow, getMainWindow(), &QMainWindow::hide);
    //emit close_mainwindow();
    main_radar->show();
}

void RadarItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "鼠标点击雷达了";
    setFocus();
    setSelected(true);
    event->accept();
    QGraphicsItem::mousePressEvent(event);
}

void RadarItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    show_property();
    QGraphicsItem::mouseDoubleClickEvent (event );
}

void RadarItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu *menu = new QMenu;
    menu->addAction(tr("删除"), this, SLOT(delete_item()));
    menu->addAction(tr("属性"), this, SLOT(show_property()));
    menu->addAction(tr("编辑"), this, SLOT(edit_radar()));
    menu->exec(event->screenPos());
    delete menu;
}