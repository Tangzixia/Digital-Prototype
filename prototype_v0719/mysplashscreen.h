#ifndef MYSPLASHSCREEN_H
#define MYSPLASHSCREEN_H

#include <QProgressBar>
#include <QSplashScreen>



class MySplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    MySplashScreen(QPixmap &pixmap, int time);
    ~MySplashScreen();
    void setProgress();
    void generateAscendRandomNumber();
    QWidget *w;

private:
    QProgressBar *progressBar;
    //随机数列表
    QList<int> numberList;
    //启动时停留时间
    int elapseTime;
//    QWidget *w;

private slots:
    void updateProgress();
};

#endif // MYSPLASHSCREEN_H
