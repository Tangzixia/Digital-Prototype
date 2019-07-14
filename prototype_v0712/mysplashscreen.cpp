#include "mysplashscreen.h"
#include <QDateTime>
#include <QProgressBar>
#include <QTimer>
#include <QDebug>
#include "unistd.h"
MySplashScreen::MySplashScreen(QPixmap &pixmap, int time):QSplashScreen (pixmap),
    elapseTime(time)
{
    //用来存放pixmap
    w = new QWidget(this);
    //大小固定为启动图像大小
    w->setGeometry(0,0, 480, 270);
//    w->setGeometry(0,0, 512, 256);
    progressBar = new QProgressBar(this);
    progressBar->setGeometry(0, w->height()-14, w->width(), 14);

    progressBar->setStyleSheet("QProgressBar{color:Gray;background-color:white;font:italic 10pt;text-align:center;}QProgressBar::chunk{background-color:rgb(0,0,0)}");
    progressBar->setRange(0,100);
    progressBar->setValue(0);
    generateAscendRandomNumber();
    setProgress();
}

MySplashScreen::~MySplashScreen()
{

}

void MySplashScreen::setProgress()
{
    int tempTime = elapseTime/100;
    for (int i=0; i<100; i++) {
        QTimer::singleShot(i*tempTime, this, SLOT(updateProgress()));
    }
    QTimer::singleShot(elapseTime, this, SLOT(close()));
}

void MySplashScreen::generateAscendRandomNumber()
{
    int i;
    qsrand(static_cast<uint>(QTime(0,0,0).secsTo(QTime::currentTime())));
    for (i=0; i<100; i++) {
        numberList.append(qrand()%101);
    }
    std::sort(numberList.begin(), numberList.end());
    //qDebug() << "生成数字完毕: " << numberList;
}

void MySplashScreen::updateProgress()
{
    static int num=0;
    progressBar->setValue(numberList[num]);
    //qDebug()<< numberList[num];
    num++;
}
