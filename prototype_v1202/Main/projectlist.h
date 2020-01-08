#ifndef PROJECTLIST_H
#define PROJECTLIST_H

#include <QListWidget>
#include <QObject>
#include <QWidget>

namespace Ui {
class ProjectList;
}

class ProjectList : public QListWidget
{
    Q_OBJECT
public:
    explicit ProjectList(QWidget *parent=nullptr);
    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void initMenu(); //初始化菜单
    QMenu *blankMenu;//点击空白上的菜单
    QMenu *projectMenu;//点击人上的菜单
    QListWidgetItem *currentItem;//当前的项
    void createListWidgetItem(QString name, QString desc);

signals:
    void thisItemCloseClicked(QListWidgetItem *item);

public slots:
    // 点击叉号的时候处理
    void HideItem(QListWidgetItem *item);
private:
    Ui::ProjectList *ui;
};

#endif // PROJECTLIST_H
