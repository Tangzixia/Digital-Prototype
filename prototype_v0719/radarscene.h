#ifndef RADARSCENE_H
#define RADARSCENE_H

#include <QGraphicsScene>
#include <QObject>

class RadarScene : public QGraphicsScene
{
    Q_OBJECT
public:
    RadarScene();
    ~RadarScene();

signals:
    void signal_xy(double x,double y);
};

#endif // RADARSCENE_H
