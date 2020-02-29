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
#include <QDateTime>
#include <QFileDialog>

/**
* @projectName   prototype_v0906
* @brief         简介:工具类
* @author        Antrn
* @date          2019-09-27
*/
Utils::Utils()
{
    //构造函数
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
    // 5.6.1
    label->setWindowFlags(Qt::FramelessWindowHint);
    // 5.9.8
    // label->setWindowFlag(Qt::FramelessWindowHint);
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
 * @brief Utils::saveFile 简介: 保存文件到某个文件夹的文件中
 * @param qw 父类组件
 * @param dirp 要打开的文件目录
 * @param filename 文件名
 * @param scene 场景对象
 * @param isPrompt 是否弹窗询问
 * @param spath
 * @return 返回结果表明：
 * @author Antrn
 * @date 2019-09-27
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
            qDebug() << "雷达场景保存路径为："+directory;
            return 1;
        }
    }else{
        qDebug() << "请选择存储位置后再保存!";
        return 0;
    }
}


/**
 * @brief Utils::saveImage 保存快照截图
 * @param f flag：场景还是视图
 * @param scene 场景指针
 * @param view 视图指针
 * @param path 保存路径
 * @param name 文件名字
 * @return 整型flag
 * @author Antrn
 * @date 2019-09-27
 */
int Utils::saveImage(int f, RadarScene *scene, QGraphicsView *view,  QString path, QString name)
{
    int width = 0, height = 0;
    // 场景
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

/**
 * @brief 没有就创建目录
 * @param dirPath
 * @return 成功与否
 */
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
* @brief         简介 读取算法组件的xml文件到Algorithm
* @author        Antrn
* @date          2019-10-02
*/
AlgorithmComp Utils::readPluginXmlFile(QString fileName)
{
    AlgorithmComp ac;
    QDomDocument doc;
    if(!fileName.isEmpty()){
//        qDebug() << fileName;
        // 中文时会读取失败，所以转化为utf-8
        QFile file(fileName.toUtf8());
        if(!file.open(QIODevice::ReadOnly)) {
            qDebug() << "算法组件的xml文件打开出错！";
            file.close();
            return ac;
        }
        if(!doc.setContent(&file)){
            file.close();
            qDebug() << "算法组件的xml文件读取失败, 请查看是否是中文命名";
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

//        QFileInfo fi = QFileInfo(fileName);
//        QString file_name = fi.fileName().split(".")[0];  //获取文件名
//        qDebug() << "短名字： " << file_name;
//        ac.setFileName(file_name);
        ac.setFileName(ac.getInfo()["Name"]);
//        qDebug() << ac.getInfo()["ID"];
        return ac;
    }else {
        // 文件名为空，啥也没选，提示
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
        // 5.9.8
//        qDebug() << "参数Map：" << j.value().toStdMap();
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

/**
 * @brief 将名字写到图片上，并保存到文件夹中
 * @param nm 图片上要写的名字
 * @return 是否成功
 */
bool Utils::generateIcon(QString nm, QString eQuip_id)
{
    QString p = QDir::currentPath()+"/images/base.png";
    QImage image = QPixmap(p).toImage();
    QPainter painter(&image); //为这个QImage构造一个QPainter
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    //设置画刷的组合模式CompositionMode_SourceOut这个模式为目标图像在上。
    painter.setRenderHint(QPainter::TextAntialiasing);
    //改变画笔和字体
    QPen pen = painter.pen();
    pen.setColor(Qt::red);
    pen.setWidth(20);
    QFont font = painter.font();
    font.setBold(true);//加粗
    font.setWeight(81);
    font.setPixelSize(11);//改变字体大小
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(image.rect(),Qt::AlignCenter, nm);
    //将nmame写在Image的中心
    int n = 100;//这个为图片的压缩度。0/100
    //将画好的图片保存起来。
    // 当为空的时候说明是往算法组件库中添加组件
    qDebug() << "-------[重要名字]---------" << nm;
    if(eQuip_id==""){
        qDebug() << "-------[重要]---------" << "是否遭存在组件库？？？";
        image.save(QDir::currentPath()+"/images/"+nm+".ico", "ico", n);
    }else{
        // WARNING 这容易出bug，保存多次。 往小的雷达项目中添加组件
        image.save(QDir::currentPath()+"/radar/"+eQuip_id+"/images/"+nm+".ico", "ico", n);
    }
    return true;
}

/**
 * @brief 新建工程目录文件
 * @param p_name 工程名
 * @return 成功与否
 */
bool Utils::createProject(QString p_name)
{
    qDebug() << "雷达id：" << p_name;
    QDir d(QDir::currentPath());
    if(d.exists(QDir::currentPath()+"/radar/"+p_name)){
        qDebug() << "目录已存在!";
        return true;
    }
    qDebug() << "测试是否执行";
    QString path1 = QDir::currentPath()+"/radar/"+p_name+"/images/";
    QString path2 = QDir::currentPath()+"/radar/"+p_name+"/room/algoXml/";
    QDir dir;
    //创建多级目录，如果已存在则会返回去true
    if(dir.mkpath(path1) && dir.mkpath(path2)){
        return true;
    }
    else{
        return false;
    }
}

/**
 * @brief 读取工程的.dpsp配置文件
 * @param project_path 项目文件路径
 * @param infomap 保存读取的工程信息
 * @param compmap 工程中其他元素的信息
 * @return 是否读取成功
 */
bool Utils::readProjectXml(QString project_path, QMap<QString, QString> &infomap, QMap<QString, QString> &compmap)
{
    QDomDocument doc;
    if(!project_path.isEmpty()){
        QFile file(project_path);
        if(!file.open(QIODevice::ReadOnly)) {
            qDebug() << "project的dpsp文件打开出错,请查看是否存在！";
            file.close();
            return false;
        }
        if(!doc.setContent(&file)){
            file.close();
            qDebug() << "project的dpsp文件读取失败!";
            return false;
        }
        file.close();

        // project-root
        QDomElement docElem = doc.documentElement();
        // 第一个孩子是<information>
        // QDomNode n = docElem.firstChild();
        QDomNode itemNode = doc.elementsByTagName("information").at(0);
        // 子孩子就是标签名为name、id
        QDomNode m = itemNode.firstChild();
        while(!m.isNull()){
            std::string tagName = m.nodeName().toStdString();
            if(m.isElement()){
                // 每个元素item
                QString content = m.toElement().text();
                // 保存起来
                // qDebug() << QString::fromStdString(tagName) << ": " << content;
                infomap.insert(QString::fromStdString(tagName), content);
            }
            m = m.nextSibling();
        }

        // 大的标签是components的时候,小标签是radar,eccm,target
        QDomNode m1 = doc.elementsByTagName("components").at(0).childNodes().at(0);
        while(!m1.isNull()){
            if(m1.isElement()){
                QString text = m1.toElement().tagName();
                QString id,name;
                if(text.compare("radar")==0){
                    id = m1.toElement().attribute("id");
                    name = m1.toElement().attribute("name");
                    qDebug() << "读取到radar： id：" << id << "name:" << name;

                }else if(text.compare("eccm")==0){
                    qDebug() << "读取到eccm:[待完善]";
                }else if(text.compare("target")==0){
                    qDebug() << "读取到target:[待完善]";
                }
                // 测试时，当遍历到eccm和target时，id和name都为""
                if(!id.isEmpty() && !name.isEmpty()){
                    // 插入后会根据id的英文字母自动排序，所以存储的顺序不是插入顺序
                    compmap.insert(id, name);
                }
            }
            m1 = m1.nextSibling();
        }
        return true;
    }else {
        // 文件名为空，啥也没选，提示
        Utils::alert(QApplication::desktop()->screen()->rect(), "请选择文件!");
        return false;
    }
}

/**
 * @brief Utils::writeProjectXml 保存工程文件到dpsp
 * @return
 */
bool Utils::writeProjectXml(QString pat, QString name)
{
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("project-root");
    doc.appendChild(root);

    QDomElement Info = doc.createElement("information");

    QDomElement xname = doc.createElement("name");
    QDomText domname = doc.createTextNode(name);
    xname.appendChild(domname);
    Info.appendChild(xname);
    QDomElement xid = doc.createElement("id");
    // 随机生成uuid
    QDomText domid = doc.createTextNode(QUuid::createUuid().toString());
    xid.appendChild(domid);
    Info.appendChild(xid);

    QDomElement Comps = doc.createElement("components");
    QDomElement xradar = doc.createElement("radar");
    QDomAttr xrid = doc.createAttribute("id");
    xrid.setValue("sfsjkdfnskdjfnsdkjf");
    QDomAttr xrname = doc.createAttribute("name");
    xrname.setValue("雷达4");
    xradar.setAttributeNode(xrid);
    xradar.setAttributeNode(xrname);
    QDomElement xeccm = doc.createElement("eccm");
    QDomElement xtarget = doc.createElement("target");
    Comps.appendChild(xeccm);
    Comps.appendChild(xtarget);
    Comps.appendChild(xradar);
    root.appendChild(Info);
    root.appendChild(Comps);

    // 【千万要注意】，改动doc之后要手动保存，我在这上面绊了一脚
    QFile file(pat);
    // 每次打开文件都会先删除后重写
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        file.close();
        qDebug() << "打开文件失败";
        return false;
    }else{
        QTextStream out(&file);
        doc.save(out, 4); //EncodingFromDocument
        file.close();
        return true;
    }
}

/**
 * @brief Utils::importXml 导入rad/ecm/targ工程的配置并调用读取
 * @param listWidget
 * @param id_inde
 * @param nameSet
 * @param id_item
 * @param flag 1新建 2导入
 * @param name 新建，不同的draglist调用的时候的表明是雷达还是电子对抗还是目标，为了公用性
 */
void Utils::importXml(QListWidget *listWidget, int *id_inde, QSet<QString> *nameSet, QMap<QString, QListWidgetItem*> *id_item, int flag, QString name)
{
    // 导入
    if(flag == 2){
        QFileInfo fileinfo;
        QString fileName = QFileDialog::getOpenFileName(nullptr, "打开rad/ecm/targ工程", QString(QDir::currentPath()), "files (*.rad *.ecm *.targ)");
        qDebug() << "长路径名字：" << fileName;

        if(fileName.compare("") && !fileName.isEmpty()){
            // 先判断导入的是哪种对象
            fileinfo = QFileInfo(fileName);
            // 文件后缀
            // qDebug() << fileinfo.suffix();
            // 文件名
            QString n = fileinfo.baseName();
            if(fileinfo.suffix().compare("rad")==0){
                add_listItem(listWidget, QString("雷达"),id_inde, nameSet, id_item, flag, n);
            }
            else if(fileinfo.suffix().compare("ecm")==0){
                add_listItem(listWidget, QString("电子对抗"),id_inde, nameSet, id_item, flag, n);
            }else{
                add_listItem(listWidget, QString("目标环境"),id_inde, nameSet, id_item, flag, n);
            }
        }
    }
    // 新建
    if(flag == 1){
        add_listItem(listWidget, name, id_inde, nameSet, id_item, flag, "");
    }
}


/**
 * @brief Utils::add_listItem 分导入和新建两种，新建则需要计数文件夹数量，向后递增新建item；导入则是保留原有的item名字
 * @param listWidget 组件对象指针
 * @param name 区分是雷达/电子对抗/目标
 * @param id_inde 列表中的id数量
 * @param nameSet 名字集合
 * @param id_item map，名字到list item指针的映射
 * @param flag 旗帜，1新建，2导入
 * @param n 导入之后的名字
 */
void Utils::add_listItem(QListWidget *listWidget, QString name, int *id_inde, QSet<QString> *nameSet, QMap<QString, QListWidgetItem*> *id_item, int flag, QString n) {
    QString path=":/img/radar.png";
    // FIXME 有必要枚举 待修缮
    qDebug() << "id_inde: " << *id_inde;

    // 不同目标的文件夹
    QString path_;
    if(name.compare("雷达")==0){
        path=":/img/radar.png";
        path_=QDir::currentPath()+"/radar";
    }
    else if(!name.compare("电子对抗机")) {
        path=":/img/ECM.png";
        path_=QDir::currentPath()+"/ecm";
    }
    else if(!name.compare("目标环境")){
        path=":/img/object.png";
        path_=QDir::currentPath()+"/object";
    }


    // 新建
    if(flag == 1){
        int fileCount=*id_inde;
        // 先检查每种对象分别有几个已存在
        QDir dir(path_);
        qDebug() << "path_ " << path_;
        QFileInfoList fileInfoList = dir.entryInfoList();
        foreach ( QFileInfo fileInfo, fileInfoList )
        {
            if ( fileInfo.fileName() == "." || fileInfo.fileName() == ".." )
                continue;
            else if ( fileInfo.isDir() )
            {
                nameSet->insert(fileInfo.fileName());
//                qDebug() << fileInfo.fileName();
                fileCount++;
//                qDebug() << fileCount;
            }else{
                continue;
            }
        }
        if(fileCount!=0){
            *id_inde = fileCount;
        }
        addItem2List(path, name, id_inde, nameSet, id_item, listWidget, n);
    }else{
        // 导入
        addItem2List(path, name, id_inde, nameSet, id_item, listWidget, n);
    }
}



/**
 * @brief Utils::addItem2List 将item新建并加入到listwidget中
 * @param path icon的路径
 * @param name 雷达/电子对抗/目标其中之一
 * @param id_inde listwidget中的id计数器，传入指针！不然会崩溃
 * @param nameSet 名字集合，不能重复，程序中保证不能重复
 * @param id_item 存放名字和item的map映射，也要指针
 * @param listWidget 传过来界面上的listwidget的指针
 * @param n 名字，如果是导入的话，需要解析导入文件的名字，也就是导入之后列表项的名字
 */
void Utils::addItem2List(QString path, QString name, int *id_inde, QSet<QString> *nameSet, QMap<QString, QListWidgetItem*> *id_item, QListWidget *listWidget, QString n)
{
    QString newName;
    // 导入
    if(!n.isEmpty() && n.compare("")){
        newName = n;
    }else{
        //新建item，添加到左边的listwidget
        newName = name+QString::number(*id_inde++);

    //    while(this->id_item.contains(newName))
        while(nameSet->contains(newName)){
            // 名称已经存在，换一个
            newName = name+QString::number(*id_inde++);
        }
    }
    if(nameSet->contains(newName)){
        qDebug() << "名字已存在, 导入失败, 说明已经导入相同名字的组件。";
        return;
    }else{
        nameSet->insert(newName);
        QListWidgetItem *item = new QListWidgetItem();
        id_item->insert(newName,item);
        item->setIcon(QIcon(path));
        item->setText(newName);

        //这里的用户角色存储用户数据（和拖入场景有关）
        item->setData(Qt::UserRole, QPixmap(path));
        item->setData(Qt::UserRole+1, newName);
        item->setData(Qt::UserRole+2, name);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
        listWidget->addItem(item);
        qDebug()<<"additem了, name:"<<newName;
    }
}

/**
 * @brief Utils::readProjectList 开始页面读取项目列表
 * @return 结果map集合
 */
QMap<QString, QString> Utils::readProjectList()
{
    QString project_path = QDir::currentPath()+"/project/projectlist.pl";
    QDomDocument doc;
    if(!project_path.isEmpty()){
        QFile file(project_path);
        if(!file.open(QIODevice::ReadOnly)) {
            qDebug() << "project的pl文件打开出错,请查看是否存在！";
            file.close();
            return QMap<QString,QString>();
        }
        if(!doc.setContent(&file)){
            file.close();
            qDebug() << "project的pl文件读取失败!";
            return QMap<QString,QString>();
        }
        file.close();

        // projects
        QDomElement docElem = doc.documentElement();
        // 第一个孩子是<project>
        QDomNode p = docElem.firstChild();
        QString name, path, dtime;
        QMap<QString,QString> resMap;
        while(!p.isNull()){
            if(p.isElement()){
                // 每个元素item
                QDomElement e = p.toElement();
                name = e.attribute("name");
                path = e.attribute("path");
                resMap.insert(name, path);
            }
            p = p.nextSibling();
        }
        return resMap;
    }else {
        // 文件名为空，啥也没选，提示
        Utils::alert(QApplication::desktop()->screen()->rect(), "请选择文件!");
        return QMap<QString,QString>();
    }
}

/**
 * @brief Utils::delete1Project
 * @param pname 项目名字，根据他对文件中的项目进行删除
 * @return 是否成功
 */
bool Utils::delete1Project(QString pname)
{
    QString ppath = QDir::currentPath()+"/project/projectlist.pl";
    QFile file(ppath); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return false;
    QDomDocument doc;
    if(!doc.setContent(&file)){
        file.close();
        qDebug() << "project的pl文件读取失败!";
        return false;
    }
    file.close();
    // projects
    QDomElement docElem = doc.documentElement();
    // 第一个孩子是<project>
    // 这种方法可以，下面的也可以
//    QDomNode p = docElem.childNodes().at(0);
//    while(!p.isNull()){
//        if(p.isElement()){
//            // 每个元素item
//            if(!p.toElement().attribute("name").compare(pname)){
//                qDebug()<< "找到对应项目名称：" <<p.toElement().attribute("name");
//                docElem.removeChild(p);
//                return true;
//            }
//        }
//        p = p.nextSibling();
//    }

    QDomNodeList list = doc.elementsByTagName("project"); //由标签名定位
    for(int i=0;i<list.count();i++)
    {
        QDomElement e=list.at(i).toElement();
        //以属性名定位，类似于hash的方式，warning：这里仅仅删除一个节点，其实可以加个break
        if(e.attribute("name")==pname || !e.attribute("name").compare(pname)){
            qDebug()<< "找到对应项目名称：" <<e.attribute("name");
            docElem.removeChild(list.at(i));
            // 【千万要注意】，改动doc之后要手动保存，我在这上面绊了一脚
            QFile file(ppath);
            // 每次打开文件都会先删除后重写
            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
                file.close();
                qDebug() << "打开文件失败";
            }else{
                QTextStream out(&file);
                doc.save(out, 4); //EncodingFromDocument
                file.close();
            }
            return true;
        }
    }
    return false;
}

/**
 * @brief Utils::addProject2Pl 向projectlist.pl中添加一条project
 * @param name 项目名字
 * @param ppath 项目路径
 * @return 是否成功
 */
bool Utils::addProject2Pl(QString name, QString ppath)
{
    QString fpath = QDir::currentPath()+"/project/projectlist.pl";
    QFile file(fpath); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return false;
    QDomDocument doc;
    if(!doc.setContent(&file)){
        file.close();
        qDebug() << "project的pl文件读取失败!";
        return false;
    }
    file.close();
    // projects
    QDomElement docElem = doc.documentElement();
    QDomElement project = doc.createElement("project");
    QDomAttr pl_name = doc.createAttribute("name");
    QDomAttr pl_path = doc.createAttribute("path");
    QDomAttr pl_dtime = doc.createAttribute("dtime");

    pl_name.setValue(name);
    pl_path.setValue(ppath);

    project.setAttributeNode(pl_name);
    project.setAttributeNode(pl_path);
    project.setAttributeNode(pl_dtime);

    docElem.appendChild(project);

    // 【千万要注意】，改动doc之后要手动保存，我在这上面绊了一脚
    QFile file2(fpath);
    // 每次打开文件都会先删除后重写
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        file.close();
        qDebug() << "打开文件失败";
        return false;
    }else{
        QTextStream out(&file);
        doc.save(out, 4); //EncodingFromDocument
        file.close();
        return true;
    }
}

