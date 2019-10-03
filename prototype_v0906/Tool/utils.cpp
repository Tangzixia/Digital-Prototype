#include "algorithmcomp.h"
#include "utils.h"
#include <QDesktopWidget>
#include <QFileDialog>
#include <QLabel>
#include <QPainter>
#include <QRect>
#include <QTextStream>
#include <QTimer>
#include <radarscene.h>
#include <QApplication>
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
//    label->setGeometry(int((rect.width()-label->width())/2), int((rect.height()-label->height())/2), label->width(), label->height());
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
int Utils::saveFile(QWidget *qw, QString dirp, QString filename, RadarScene *scene, bool isPrompt, QString &spath)
{
    QString directory = dirp;
    openDirOrCreate(directory);
    if(isPrompt){
        spath = QFileDialog::getExistingDirectory(
                qw,
                "选择文件夹",
                directory,
                QFileDialog::ShowDirsOnly);
    }
    if (!spath.isEmpty() && directory != spath+"/")
    {
        QFile file(spath+"/"+filename); // 这个斜杠很关键
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
            file.close();
        }else{
            QTextStream out(&file);
            scene->getDoc().save(out, 4); //EncodingFromDocument
            file.close();
            qDebug() << "场景保存成功!路径为："+spath;
        }
    }
    // 保存到默认文件夹
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
            qDebug() << "路径为："+directory;
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

void Utils::openDirOrCreate(QString dirPath)
{
    QDir dir(dirPath);
    if(!dir.exists()){
        dir.mkdir(dirPath);//创建多级目录
    }
}

// This is available in all editors.
/**
* @projectName   prototype_v0906
* @brief         简介 读取算法组件的xml文件
* @author        Antrn
* @date          2019-10-02
*/
AlgorithmComp Utils::readPluginXmlFile(QString fileName)
{
    AlgorithmComp ac;
    QDomDocument doc;
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)) {
            qDebug() << "文件打开出错！";
            return ac;
        }
        if(!doc.setContent(&file)){
            file.close();
            qDebug() << "打开失败";
            return ac;
        }
        file.close();
        // 根元素component
        QDomElement docElem = doc.documentElement();
        // 第一个孩子是<Information>
//        QDomNode n = docElem.firstChild();
        QDomNode itemNode = doc.elementsByTagName("Information").at(0);
        // 子孩子就是标签名为Name、ID...
        QDomNode m = itemNode.firstChild();
        while(!m.isNull()){
            std::string tagName = m.nodeName().toStdString();
            if(m.isElement()){
                // 每个元素item
                QString content = m.toElement().text();
                // 保存起来
                qDebug() << QString::fromStdString(tagName) << ": " << content;
                ac.getInfo().insert(QString::fromStdString(tagName), content);
            }
            m = m.nextSibling();
        }
        // Description
        QDomNode descNode = doc.elementsByTagName("Description").at(0);
        QString desc = descNode.toElement().text();
        qDebug() << "desc: " << desc;
        ac.setDesc(desc);

        // 大的标签是Parameter的时候
        QDomNode ParaNode = doc.elementsByTagName("Parameter").at(0);
        // 就是arrow了，因为箭头就一种
        QDomNode m1 = ParaNode.firstChild();
        QString describe, value;
        // 遍历所有的箭头
        while(!m1.isNull()){
            std::string tagName = m1.nodeName().toStdString();
            if(m1.isElement()){
                // 每个元素item
                QDomElement e = m1.toElement();
                describe = e.attribute("describe");
                value = e.attribute("value");
                qDebug() << QString::fromStdString(tagName) << ": describe： " << describe << "; " << "value: " << value;
                QMap<QString, QString> mm;
                mm.insert(describe, value);
                ac.getParam().insert(QString::fromStdString(tagName), mm);
            }
            m1 = m1.nextSibling();
        }
        return ac;
    }else {
        // TODO 文件名为空，啥也没选，提示
        Utils::alert(QApplication::desktop()->screen()->rect(), "请选择文件!");
        return ac;
    }
}
