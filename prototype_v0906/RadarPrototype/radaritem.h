#ifndef RADARITEM_H
#define RADARITEM_H
#include <QGraphicsItem>
#include <QMainWindow>
#include "menu_iteamoperation.h"
/**
* @projectName   prototype_v0719
* @brief         整体仿真中的雷达组件类。
* @author        Antrn
* @date          2019-08-12
*/

class RadarItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    RadarItem();
    RadarItem(QString);
    ~RadarItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
    bool canDrag = false;

    QPointF start_click_point;
    QPointF release_click_point;

    QMainWindow* getMainWindow();

    QString getRadar_id() const;
    void setRadar_id(QString value);

public slots:
    void delete_item();
    void show_property();
    void edit_radar();
    void itemOperateSlot(Menu_iteamOperation::OperateType,QString);

signals:
    void close_mainwindow();
    void itemOperate(Menu_iteamOperation::OperateType);



private:
    QString radar_id;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif // RADARITEM_H
