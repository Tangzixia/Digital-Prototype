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
#include <QDatetime>

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
    // 用户选择了其他位置时执行
    if (!spath.isEmpty() && directory != spath+"/")
    {
        QFile file(spath+"/"+filename); // 这个斜杠很关键
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
            file.close();
        }else{
            QTextStream out(&file);
            scene->getDoc()->save(out, 4); //EncodingFromDocument
            file.close();
            qDebug() << "场景保存成功!路径为："+spath;
        }
    }
    // 保存到默认文件夹，必执行
    if (!directory.isEmpty())
    {
        QFile file(directory+"/"+filename); // 这个斜杠很关键
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
            file.close();
            return -1;
        }else{
            QTextStream out(&file);
            scene->getDoc()->save(out, 4); //EncodingFromDocument
            file.close();
            qDebug() << "路径为："+directory;
            return 1;
        }
    }else{
        qDebug() << "请选择存储位置后再保存!";
        return 0;
    }
}

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

QDir Utils::openDirOrCreate(QString dirPath)
{
    QDir dir(dirPath);
    if(!dir.exists()){
        dir.mkdir(dirPath);//创建多级目录
    }
    return dir;
}

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
//        qDebug() << fileName;
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)) {
            qDebug() << "文件打开出错！";
            file.close();
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
        QMap<QString, QString> infomap;
        while(!m.isNull()){
            std::string tagName = m.nodeName().toStdString();
            if(m.isElement()){
                // 每个元素item
                QString content = m.toElement().text();
                // 保存起来
//                qDebug() << QString::fromStdString(tagName) << ": " << content;
                infomap.insert(QString::fromStdString(tagName), content);
            }
            m = m.nextSibling();
        }
        ac.setInfo(infomap);
        // Description
        QDomNode descNode = doc.elementsByTagName("Description").at(0);
        QString desc = descNode.toElement().text();
//        qDebug() << "desc: " << desc;
        ac.setDesc(desc);

        // 大的标签是Parameter的时候
        QDomNode ParaNode = doc.elementsByTagName("Parameter").at(0);
        // 就是arrow了，因为箭头就一种
        QDomNode m1 = ParaNode.firstChild();
        QString describe, value;
        QMap<QString, QMap<QString, QString> >paramap;
        // 遍历所有的箭头
        while(!m1.isNull()){
            std::string tagName = m1.nodeName().toStdString();
            if(m1.isElement()){
                // 每个元素item
                QDomElement e = m1.toElement();
                describe = e.attribute("describe");
                value = e.attribute("value");
//                qDebug() << QString::fromStdString(tagName) << ": describe： " << describe << "; " << "value: " << value;
                QMap<QString, QString> mm;
                mm.insert("describe", describe);
                mm.insert("value", value);
                paramap.insert(QString::fromStdString(tagName), mm);
            }
            m1 = m1.nextSibling();
        }
        ac.setParam(paramap);
        QFileInfo fi = QFileInfo(fileName);
        QString file_name = fi.fileName().split(".")[0];  //获取文件名
//        qDebug() << "短名字： " << file_name;
        ac.setFileName(file_name);
//        qDebug() << ac.getInfo()["ID"];
        return ac;
    }else {
        // TODO 文件名为空，啥也没选，提示
        Utils::alert(QApplication::desktop()->screen()->rect(), "请选择文件!");
        return ac;
    }
}

/**
* @projectName   prototype_v0906
* @brief         简介 将算法组件存到xml文件中,
* @author        Antrn
* @date          2019-10-03
* @param ac 算法对象
* @param subPath 子目录，代表用户拖动到场景中复制出来的算法组件
*/
void Utils::writeAlgorithmComp2Xml(AlgorithmComp ac, QString subPath)
{
    QString savePath;
    if(subPath.isNull()){
        savePath = QDir::currentPath()+"/algoXml/";
    }else{
        QDir *dir = new QDir;
        dir->mkdir(QDir::currentPath()+subPath);
        savePath = QDir::currentPath()+ subPath +"/algoXml/";
    }

    openDirOrCreate(savePath);

    QMap<QString, QString> infoMap = ac.getInfo();
    QMap<QString, QMap<QString, QString>> paraMap = ac.getParam();
    QString desc = ac.getDesc();
    QDomDocument doc;
    QDomElement Info, Desc, Param;
    //添加处理指令即xml头部说明和场景的属性
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("Plugins");
    doc.appendChild(root);
    Info = doc.createElement("Information");
    Desc = doc.createElement("Description");
    root.appendChild(Info);
    root.appendChild(Desc);
    Param = doc.createElement("Parameter");
    root.appendChild(Param);

    // info
    for (QMap<QString, QString>::iterator i =infoMap.begin();i!=infoMap.end();i++) {
        QDomElement info = doc.createElement(i.key());

        QDomText c = doc.createTextNode(i.value());
#if 0
        // 更新时间，不为空说明是复制到工程空间的，需要更改时间和id等信息
        if(!subPath.isNull()){
            QString dtime;
            if(!i.key().compare("Time")){
                QDateTime *dt = new QDateTime;
                dtime = dt->currentDateTime().toString();
                c = doc.createTextNode(dtime);
            }else{
                c = doc.createTextNode(i.value());
            }
        }
#endif
        info.appendChild(c);
        Info.appendChild(info);
    }
    QDomText d = doc.createTextNode(ac.getDesc());
    Desc.appendChild(d);
    // params
    for (QMap<QString, QMap<QString, QString>>::iterator j =paraMap.begin();j!=paraMap.end();j++) {
        QDomElement para = doc.createElement(j.key());
        QDomAttr describe = doc.createAttribute("describe");
        QDomAttr value = doc.createAttribute("value");
        describe.setValue(j.value().value("describe"));
        qDebug() << "参数Map：" << j.value().toStdMap();
        value.setValue(j.value().value("value"));
        para.setAttributeNode(describe);
        para.setAttributeNode(value);
        Param.appendChild(para);
    }
    QString id_ = ac.getInfo().take("ID");

    QString filename;
    if(!subPath.isNull()){
        // 当有子目录存在的时候，存在子目录中的算法组件命名为name+id,由于id两边有大括号，所以去除
        filename = savePath+ac.getFileName()+id_.mid(1,id_.length()-2)+".xml";
    }else{
        filename = savePath+ac.getFileName()+".xml";
    }
    QFile file(filename); // 这个斜杠很关键
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        file.close();
        qDebug() << "打开文件失败";
    }else{
        QTextStream out(&file);
        doc.save(out, 4); //EncodingFromDocument
        file.close();
        qDebug() << "算法组件保存成功!路径为： "+filename;
    }
}

/**
* @projectName   prototype_v0906
* @brief         简介 重命名文件，必须要使用静态方法，且绝对路径，否则失败
* @author        Antrn
* @date          2019-10-04
*/
bool Utils::modifyFileName(QString fileName, QString newName)
{
    QString filePath = QDir::currentPath()+"/algoXml/"+fileName+".xml";
    qDebug() << "修改文件名前文件完整路径： " << filePath;
    AlgorithmComp ac = readPluginXmlFile(filePath);
    ac.setFileName(newName);
    if(QFile::exists(filePath)){
//        bool b = QFile::rename(filePath, QDir::currentPath()+"/algoXml/"+newName+".xml");
//        return b;
        QFile::remove(filePath);
    }else{
        qDebug() << "源文件不存在!";
//        return false;
    }
    writeAlgorithmComp2Xml(ac);
    return true;
}

/**
 * @brief 动态删除场景里产生的xml拷贝算法组件空间的文件
 * @param dname 文件夹路径
 * @return 是否成功
 */
bool Utils::deleteXmlFileByName(QString dname, QString id)
{
    QDir dir(dname);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    qDebug() << "id_split: " << id;
    for(int i = 0; i < file_list.size(); ++i){
        if(file_list.at(i).fileName().split(".")[0].endsWith(id)){
            QFile file(file_list.at(i).absoluteFilePath());
//            qDebug() << "finfo.absoluteFilePath()： " << file_list.at(i).absoluteFilePath();
            if (file.exists())
            {
                qDebug() << "删除文件--->"<<file_list.at(i).absoluteFilePath();
                file.remove();
                return true;
            }
        }
    }
    return false;
}
