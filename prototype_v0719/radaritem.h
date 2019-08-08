#ifndef RADARITEM_H
#define RADARITEM_H
#include <QGraphicsItem>
#include <QMainWindow>


class RadarItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    RadarItem();
    ~RadarItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
    bool canDrag = false;

    QPointF start_click_point;
    QPointF release_click_point;

    QMainWindow* getMainWindow();

public slots:
    void delete_item();
    void show_property();
    void edit_radar();

signals:
    void close_mainwindow();

private:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif // RADARITEM_H
