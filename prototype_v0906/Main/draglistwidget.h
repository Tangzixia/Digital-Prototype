#ifndef DRAGLISTWIDGET_H
#define DRAGLISTWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QListWidget>
#include <QApplication>

/**
* @projectName   prototype_v0719
* @brief         整体工程项目左边的拖拽列表：雷达/电子对抗设备，是以list的形式表现的。
* @author        Antrn
* @date          2019-08-12
*/
class DragListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit DragListWidget(QWidget *parent = nullptr);
    void addDragItem(QListWidgetItem*item);
    void addDragItem(const QString &label);
    static QString puzzleMimeType() {
        return QStringLiteral("image/x-module-piece");
    }
//没用到
signals:
    void repaintWidget();

protected:
    //开始拖拽
    void startDrag(Qt::DropActions supportedActions);

   //从该部件中拖拽出去的操作.
    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    //接受外来拖拽的操作.
//    void dragEnterEvent(QDragEnterEvent *event);
//    void dragMoveEvent(QDragMoveEvent *event);
//    void dropEvent(QDropEvent *event);

private:
    //记录拖拽的起点.
    QPoint m_dragPoint;
    //记录被拖拽的项.
    QListWidgetItem *m_dragItem;
    QListWidgetItem *addRadarButton;

public slots:

};

#endif // DRAGLISTWIDGET_H
