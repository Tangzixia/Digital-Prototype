#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QDateTime>
#include <QLabel>
#include <QMovie>
#include "unistd.h" //usleep()
#include "hellodialog.h"
#include "mysplashscreen.h"
#include <QStyleFactory>
/**
* @projectName   prototype_v0719
* @brief         此程序的main函数，入口类。
* @author        Antrn
* @date          2019-08-12
*/
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("vis")); // 设置vista风格
//    QPixmap pixmap(":/img/start_logo.gif");
    QPixmap pixmap(":/img/startup.gif");
//    QPixmap pixmap(":/img/lixian.jpg");
    MySplashScreen splash(pixmap, 3000);
//    QSplashScreen splash(pixmap);
//    splash.setWindowOpacity(0.5);

    QLabel label(splash.w);
    QMovie mv(":/img/startup.gif");
//    QMovie mv(":/img/start_logo.gif");
    label.setMovie(&mv);
    mv.start();
    splash.show();
    splash.setCursor(Qt::BlankCursor);
    for (int i=0; i<100; i++) {
        app.processEvents();
        usleep(30000);
    }
//    QDateTime n=QDateTime::currentDateTime();
//    QDateTime now;
//    do{
//        now=QDateTime::currentDateTime();
//    } while (n.secsTo(now)<=5);//6为需要延时的秒数

//    for(int i=0; i<5000; i+=mv.speed())
//    {
//        QCoreApplication::processEvents();
//        usleep(500*static_cast<__useconds_t>(mv.speed()));
//    }
    HelloDialog hello;
    hello.show();
//    MainWindow window;
//    window.show();
//    splash.finish(&window);
    splash.finish(&hello);
    return app.exec();
}
