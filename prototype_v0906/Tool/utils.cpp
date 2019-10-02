#include "utils.h"

#include <QFileDialog>
#include <QLabel>
#include <QPainter>
#include <QRect>
#include <QTextStream>
#include <QTimer>
#include <radarscene.h>
// This is available in all editors.
/**
* @projectName   prototype_v0906
* @brief         简介:工具类
* @author        Antrn
* @date          2019-09-27
*/
Utils::Utils()
{

}

/**
* @projectName   prototype_v0906
* @brief         简介: 弹窗工具
* @author        Antrn
* @date          2019-09-27
*/
void Utils::alert(QRect rect, QString content)
{
    QLabel *label = new QLabel;
    label->setWindowFlag(Qt::FramelessWindowHint);
    label->setStyleSheet("background-color:rgb(212,237,218)");
    label->setMaximumWidth(200);
    label->setMaximumHeight(50);
    label->setMinimumWidth(200);
    label->setMinimumHeight(50);

    label->setText(content);
    label->setAlignment(Qt::AlignCenter);
    label->setGeometry(int((rect.width()-label->width())/2), int((rect.height()-label->height())/2), label->width(), label->height());
    label->show();
    QTimer::singleShot(1000, label, [=](){
       label->close();
       delete label;
    });
}

// This is available in all editors.
/**
* @projectName   prototype_v0906
* @brief         简介: 保存文件到某个文件夹的文件中
* @author        Antrn
* @date          2019-09-27
*/
int Utils::saveFile(QWidget *qw, QString dirp, QString filename, RadarScene *scene, bool isPrompt)
{
    QString directory = dirp;
    QDir dir(directory);
    if(!dir.exists()){
        dir.mkdir(directory);
    }
    if(isPrompt){
        directory = QFileDialog::getExistingDirectory(
                qw,
                "选择文件夹",
                directory,
                QFileDialog::ShowDirsOnly);
    }

    if (!directory.isEmpty())
    {
        QFile file(directory+"/"+filename); // 这个斜杠很关键
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
            file.close();
            return -1;
        }else{
            QTextStream out(&file);
            scene->getDoc().save(out, 4); //EncodingFromDocument
            file.close();
            qDebug() << "场景保存成功!路径为："+directory;
            return 1;
        }
    }else{
        qDebug() << "请选择存储位置后再保存!";
        return 0;
    }
}

// This is available in all editors.
/**
* @projectName   prototype_v0906
* @brief         简介 保存快照截图
* @author        Antrn
* @date          2019-09-27
*/
int Utils::saveImage(int f, RadarScene *scene, QGraphicsView *view,  QString path, QString name)
{
    int width = 0, height = 0;
    if(f == 1){
        width = static_cast<int>(scene->width());
        height = static_cast<int>(scene->height());
    }else{
        width = static_cast<int>(view->width());
        height = static_cast<int>(view->height());
    }
    QSize mysize(width, height);
    QImage image(mysize, QImage::Format_RGB32);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    if(f == 1){
        scene->render(&painter);//也可以使用视图保存，只保存视图窗口的快照
    }else{
        view->render(&painter);
    }
    painter.end();
    QString dir_str = path;
    // 检查目录是否存在，若不存在则新建
    QDir dir;
    if (!dir.exists(dir_str))
    {
        dir.mkpath(dir_str);
    }
    image.save(dir_str+name);
    return 0;
}
