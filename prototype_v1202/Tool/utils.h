#ifndef UTILS_H
#define UTILS_H
#include "algorithmcomp.h"
#include <QGraphicsView>
#include <QRect>
#include <QString>
#include <QWidget>
#include <mainwindow_radar.h>
#include <radarcompdraglistwidget.h>
#include <radarscene.h>
#include <QSet>

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
    static QDir openDirOrCreate(QString dirPath);
    static AlgorithmComp readPluginXmlFile(QString fileName);
    static void writeAlgorithmComp2Xml(AlgorithmComp ac, QString subPath=nullptr);
    static bool modifyFileName(QString fileName, QString newName);
    static bool deleteXmlFileByName(QString name, QString id);
    static bool generateIcon(QString name, QString eQuip_id);
    static bool createProject(QString p_name);
    static bool readProjectXml(QString project_path, QMap<QString, QString> &infomap, QMap<QString, QString> &compmap);
    static bool writeProjectXml(QString pat, QString name); // FIXME 暂时这样写，后续完善
    static void importXml(QListWidget *listWidget, int *id_inde, QSet<QString> *nameSet, QMap<QString, QListWidgetItem *> *id_item, int flag, QString name);
    static void add_listItem(QListWidget *listWidget, QString name, int *id_inde, QSet<QString> *nameSet, QMap<QString, QListWidgetItem *> *id_item, int flag, QString n);
    static void addItem2List(QString path, QString name, int *id_inde, QSet<QString> *nameSet, QMap<QString, QListWidgetItem*> *id_item, QListWidget *listWidget, QString n);
    static QMap<QString, QString> readProjectList();
    static bool delete1Project(QString pname);
    static bool addProject2Pl(QString name, QString ppath);
    static bool codeGenerate(QList<DiagramItem *> *dlist, QString radar_code_file, QString code_template_start, QString code_template_end);
    static void deleteCppFileByName(QString path, QString cppName);
};
#endif // UTILS_H
