#ifndef GRAPHICSVIEWPUBLIC_H
#define GRAPHICSVIEWPUBLIC_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "diagramitem.h"
class GraphicsScenePublic : public QGraphicsScene
{
    Q_OBJECT
public:
    //InsertItem默认
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };
    explicit GraphicsScenePublic(QMenu *itemMenu, QObject *parent = nullptr);
    void modifyXmlItems(QPointF pos, DiagramItem *item);
    void updateXmlItemsPos(QPointF pos, DiagramItem *item);
    void modifyXmlArrows(Arrow *arrow, DiagramItem *startItem, DiagramItem *endItem);
    QList<int> idList;

    int generateUniqueid()
    {
        int i,j;
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        idList.append(qrand()%10);
        for(i=0;i<idList.size();i++)
        {
            bool flag=true;
            while(flag)
            {
                for(j=0;j<i;j++)
                {
                    if(idList[i]==idList[j])
                    {
                        break;
                    }
                }
                if(j<i)
                {
                    idList[i]=rand()%10;
                }
                if(j==i)
                {
                    flag=!flag;
                }
            }
        }
        return idList.back();
    }
signals:
   void signal_xy(double x,double y);
   //算法组件添加
    void itemInserted(DiagramItem *item);
    void itemSelected(QGraphicsItem *item);
    //运行，结束
     void startRun();
     void overRun();

public slots:
    void setMode(Mode mode);
    void setItemType(DiagramItem::DiagramType itemType);
protected:
     //为了XY
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    //
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
private:

     Mode myMode;
     QGraphicsLineItem *line;
     DiagramItem::DiagramType myItemType;
     QMenu *myItemMenu;
    bool isItemChange(int type);
};

#endif // GRAPHICSVIEWPUBLIC_H
