#ifndef UTILS_H
#define UTILS_H

#include "algorithmcomp.h"

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
    static void alert(QRect rect, QString content);
    static int saveFile(QWidget *qw, QString dirp, QString filename, RadarScene *scene, bool isPrompt, QString &spath);
    static int saveImage(int f, RadarScene *scene,  QGraphicsView *view,  QString path, QString name);
    static void openDirOrCreate(QString dirPath);
    static AlgorithmComp readPluginXmlFile(QString fileName);
};

#endif // UTILS_H
