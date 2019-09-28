#ifndef UTILS_H
#define UTILS_H

#include <QGraphicsView>
#include <QRect>
#include <QString>
#include <QWidget>
#include <radarscene.h>



class Utils
{
public:
    Utils();
    void alert(QRect rect, QString content);
    int saveFile(QWidget *qw, QString dirp, QString filename, RadarScene *scene, bool isPrompt);
    int saveImage(int f, RadarScene *scene,  QGraphicsView *view,  QString path, QString name);
};

#endif // UTILS_H
