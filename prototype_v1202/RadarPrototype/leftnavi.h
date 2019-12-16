#ifndef LEFTNAVI_H
#define LEFTNAVI_H

#include <QFrame>
#include <QListWidget>
#include <QSplitter>
#include <QToolBar>
#include <QToolButton>


class LeftNavi : public QFrame
{
    Q_OBJECT

public:
    explicit LeftNavi(QFrame *parent = nullptr);
    ~LeftNavi();
private:
     QToolBar *naviToolBar;
     void fillToolBar();
     QAction *actSearch;
     QToolButton *netHead;
     QListWidget *lw, *lw2;
     QSplitter *vSplit;
     QByteArray vSplitState;

private slots:
     void showNetTree();
};
#endif // LEFTNAVI_H
