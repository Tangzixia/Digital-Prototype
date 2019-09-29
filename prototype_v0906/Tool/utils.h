#ifndef UTILS_H
#define UTILS_H

#include <QGraphicsView>
#include <QRect>
#include <QString>
#include <QWidget>
#include <radarscene.h>



// This is available in all editors.
/**
* @projectName   prototype_v0906
* @brief         简介
* @author        Antrn
* @date          2019-09-27
*/
class Utils
{
public:
    Utils();
    void alert(QRect rect, QString content);
    int saveFile(QWidget *qw, QString dirp, QString filename, RadarScene *scene, bool isPrompt);
    int saveImage(int f, RadarScene *scene,  QGraphicsView *view,  QString path, QString name);
};

#endif // UTILS_H
