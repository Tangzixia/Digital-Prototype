#include "qudong.h"
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QApplication>
#include <QDir>
QuDong::QuDong()
{

}

void QuDong::startRun(QString fileName, QString outName)
{
//    QProcess *p1 = new QProcess;//./home/a913/Qt_project/20190910/prototype_v0906/Projects/test/code/radar_test.out
//    p1->start("g++", QStringList() << "/home/a913/Qt_project/20190910/prototype_v0906/Projects/test/code/radar_test.cpp"<< "-fopenmp"<< "-lfftw3"<< "-lm"<< "-o" <<"/home/a913/Qt_project/20190910/prototype_v0906/Projects/test/code/radar_test.out");
//    p1->waitForStarted();
//    p1->close();
//    p1->waitForFinished();
    QStringList args;
    args.append(fileName);
    args.append("-o");
    args.append(outName);
    args.append("-lvsip");
    args.append("-lfftw3");
    args.append("-lm");
//    args.append("-lpthread");

//    QString command = args.join(" ");
//    qDebug() << "command: " << command;

    QProcess *p1 = new QProcess;
    p1->start("gcc", args);
    p1->waitForStarted();
    p1->waitForFinished();
    p1->close();
    p1->kill();

    QProcess *p = new QProcess;
    // 修改此处的参数
//    QString workpath = "/home/a913/Qt_project/20191130/prototype_v1202/Projects/test/code/";
    QDir dir(QDir::currentPath());
    dir.cdUp();
    dir.cd("prototype_v1202/Projects/test/code");
    QString workpath = dir.absolutePath();
    p->setWorkingDirectory(workpath);
    qDebug() << p->environment();
    p->start("./radar.out");
    p->waitForStarted();
    p->waitForFinished();

    QString strTemp=QString::fromLocal8Bit(p->readAllStandardOutput());
    QString strError=QString::fromLocal8Bit(p->readAllStandardError());

    p->close();
    p->kill();

    // 输出控制台信息
    qDebug() << "输出信息： " << strTemp << ";" << strError;
    QMessageBox testMassage;
    testMassage.setText(strTemp+strError);
    testMassage.exec();
}

/**
 * @brief QuDong::startRunCpp
 * @param fileName 执行代码的完整文件名
 * @param outName 输出文件名，一般是.o
 * @param radar_name 雷达ID，即雷达名字
 */
void QuDong::startRunCpp(QString fileName, QString outName, QString radar_name)
{
    QStringList args;
    args.append(fileName);
    args.append("-o");
    args.append(outName);

    QProcess *p1 = new QProcess;
    p1->start("gcc", args);
    p1->waitForStarted();
    p1->waitForFinished();
    p1->close();
    p1->kill();

    QProcess *p = new QProcess;
    QString workpath = QDir::currentPath()+"/radar/"+radar_name+"/room/";
//    p->setWorkingDirectory(workpath);
//    qDebug() << "环境是" << p->environment();
    p->start(workpath+"/test.exe");
    p->waitForStarted();
    p->waitForFinished();

    QString strTemp=QString::fromLocal8Bit(p->readAllStandardOutput());
    QString strError=QString::fromLocal8Bit(p->readAllStandardError());

    p->close();
    p->kill();

    // 输出控制台信息
    qDebug() << "输出信息： " << strTemp << ";" << strError;
    QMessageBox testMassage;
    testMassage.setText(strTemp+strError);
    testMassage.setWindowTitle("输出");
    testMassage.exec();
}
