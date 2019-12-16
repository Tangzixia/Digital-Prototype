#include "realtimecurve.h"
#include "ui_realtimecurve.h"

#include <QDateTime>
#include <QPainter>
#include <QTimer>

RealTimeCurve::RealTimeCurve(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RealTimeCurve)
{
    ui->setupUi(this);
    this->resize(600,500);      //resize the main window
    ui->startButton->setStyleSheet("background: white");
    ui->stopButton->setStyleSheet("background: white");
    p = t = 0;
    timer = new QTimer;
    path = new QPainterPath;
    point = new QPoint;
    connect(ui->startButton, SIGNAL(clicked()),this,SLOT(startPainting()));
    connect(ui->stopButton, SIGNAL(clicked()),this,SLOT(stopPainting()));
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
}

RealTimeCurve::~RealTimeCurve()
{
    delete ui;
    delete timer;
    delete point;
    delete path;
}

void RealTimeCurve::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::green)); //adjust the painter size and color
    QPoint originPoint(p,0);
    painter.translate(originPoint);  //adjust the origin point of the painting
    painter.drawPath(*path);  //painting
    QWidget::paintEvent(event);
}

void RealTimeCurve::timerUpdate()
{
    t += 10;
    //create random number
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    int rand=qrand()%this->height();
    //the next point of the painted line
    point->setX(t);
    point->setY(rand);
    path->lineTo(*point);
    if(t > this->width())
    {
        p -= 10;
    }

    //in case update() method is called, the system will trigger paintEvent to repaint the window
    this->update();
}

void RealTimeCurve::startPainting()
{
    timer->start(100);
}

void RealTimeCurve::stopPainting()
{
    timer->stop();
}
