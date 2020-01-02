#ifndef RADARSCENE_H
#define RADARSCENE_H

/**
* @projectName   prototype_v0719
* @brief         编辑雷达页面的自定义场景类，上面绘制设计雷达的组件链接。
* @author        Antrn
* @date          2019-08-12
*/
#include "diagramitem.h"
#include "diagramtextitem.h"

#include <QtXml>
#include <QGraphicsScene>
#include <QMenu>
#include <QObject>
#include <algorithmcomp.h>

class RadarScene : public QGraphicsScene
{
    Q_OBJECT

public:

    bool isSelected = false;
    void setIsSelected(bool b){
        isSelected = b;
    }
    //InsertItem默认
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

    explicit RadarScene(QMenu *itemMenu, QObject *parent = nullptr);

    QFont font() const { return myFont; }
    QColor textColor() const { return myTextColor; }
    QColor itemColor() const { return myItemColor; }
    QColor lineColor() const { return myLineColor; }

    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setFont(const QFont &font);

    void modifyXmlItems(QPointF pos, DiagramItem *item);
    void updateXmlItemsPos(QPointF pos, DiagramItem *item);
    void modifyXmlArrows(Arrow *arrow, DiagramItem *startItem, DiagramItem *endItem);

    QMenu *getItemMenu(){return myItemMenu;}

    QDomDocument *getDoc(){return &doc;}

    QList<QString> idList;
#if 0
    // NOTE 最多生成100个不重复的数
    int generateUniqueid()
    {
        int i,j;
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        idList.append(qrand()%1000);
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
                    idList[i]=rand()%100;
                }
                if(j==i)
                {
                    flag=!flag;
                }
            }
        }
        return idList.back();
    }
#endif
    QString getMyItemIconName() const;

    void setDoc(const QDomDocument &value);

    QDomElement *getArrs();
    void setArrs(const QDomElement &value);

    QDomElement *getItems();
    void setItems(const QDomElement &value);

    QColor getMyItemColor() const;
    void setMyItemColor(const QColor &value);

    QMap<QString, AlgorithmComp *> getScene_comps() const;
    void setScene_comps(const QMap<QString, AlgorithmComp *> &value);

    // 向子空间加入文件
    void createFile2zoom(QString sid);

public slots:
    void setMode(Mode mode);
    //    void setItemType(DiagramItem::DiagramType type);
    void setMyItemIconName(QString value);
    void editorLostFocus(DiagramTextItem *item);
    void startRunCode();
    void sendRate(float rate);


    // 接收来自radarcompdraglistwidget的算法信息
    void receiveAlgo4listWidget(AlgorithmComp ap);

signals:
    void signal_xy(double x,double y);

    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);

    void startRun();
    void rateSignal(float rate);
    void overRun();

    // 通知MainWindow_Radar xml已经改变
    void isSave2False(QString message);


protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
//    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    // 支持拖拽
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    bool isItemChange(int type);

//    DiagramItem::DiagramType myItemType;
    QString myItemIconName;
    DiagramTextItem *textItem;
    // 菜单
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;

    QFont myFont;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;

    // 存储场景数据
    QDomDocument doc;
    QDomElement Arrs;
    QDomElement Items;
    // 场景中所有的算法存在列表中
    QMap<QString, AlgorithmComp *> scene_comps;
    // 临时存放listWidget在点击和拖动时传过来的对象
    AlgorithmComp ap;
};

#endif // RADARSCENE_H
