#ifndef RADARCOMPDRAGLISTWIDGET_H
#define RADARCOMPDRAGLISTWIDGET_H

#include <QListWidget>

// This is available in all editors.
/**
* @projectName   prototype_v0906
* @brief         简介:将组件列表改为ListWidget形式，可以拖动
* @author        Antrn
* @date          2019-09-27
*/
class RadarCompDraglistWidget : public QListWidget
{
    Q_OBJECT
public:
    RadarCompDraglistWidget(QWidget *parent = nullptr);
    void addDragItem(QListWidgetItem*item);
    void addDragItem(const QString &label);
    static QString puzzleMimeType() {
        return QStringLiteral("image/x-Comp-piece");
    }
public slots:

signals:
    void add_one_Comp(QString name);
    void setComp_typeandMode(int id);

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
    QListWidgetItem *addCompButton;
};

#endif // RADARCOMPDRAGLISTWIDGET_H
