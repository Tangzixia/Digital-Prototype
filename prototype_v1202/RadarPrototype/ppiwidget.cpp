#include "ppiwidget.h"
#include "ui_ppiwidget.h"
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QtMath>
const static double PI = 3.1415926;
PPIWidget::PPIWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PPIWidget)
{
    ui->setupUi(this);
    setStyleSheet("background-color:black;");

    qsrand(uint (time(nullptr)));
    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(16);
    connect(m_updateTimer, &QTimer::timeout, this, [this](){ update(); });
//    start();
    timer = new QTimer(this);
    w_width = width();
    w_height = height();

    radius = (qMin(w_width, w_height)-80)/2;
//    qDebug() << "radiu1s: " << radius;

    connect(timer, &QTimer::timeout, this, [=]
    {
        if(!m_points.isEmpty()){
            for(int i = 0; i < 200; ++i)
            {
                m_points.removeAt(i);
                int alpha = qrand() % 100 + 40;
                int ang = -30;
                int radiu = qrand() % radius;
                int angle = qrand()%360;
                double px = radiu*cos(angle * qvariant_cast<double>(PI/180));
                double py = radiu*sin(angle * qvariant_cast<double>(PI/180));
                QPoint center(int(w_width / 2), int(w_height / 2));
                m_points.append(Point(QPointF(center.x()+px, center.y()+py), alpha, ang));
            }
        }else{
            get_data4m_points();
        }

    });
}

PPIWidget::~PPIWidget()
{
    delete ui;
}

void PPIWidget::start()
{
    get_data4m_points();
    m_drawable = true;
    m_angle = 90;
    timer->start(1000);
    m_updateTimer->start();
}

void PPIWidget::stop()
{
    m_drawable = false;
    timer->stop();
    m_updateTimer->stop();
    m_points.clear();
    update();
}

void PPIWidget::get_data4m_points()
{
    for(int i = 0; i < 1000; ++i)
    {
        int alpha = qrand() % 100 + 40;
        int ang = -30;
//            qDebug() << "radius2: " << radius;
        int radiu = qrand() % radius;
        int angle = qrand()%360;
        double px = radiu*cos(angle * qvariant_cast<double>(PI/180));
        double py = radiu*sin(angle * qvariant_cast<double>(PI/180));
        QPoint center(int(w_width / 2), int(w_height / 2));
//            qDebug() << "center: " << center;
        m_points.append(Point(QPointF(center.x()+px, center.y()+py), alpha, ang));
    }
}

void PPIWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // 格子
//    for(int i = 0; i < 500; i += 20)
//        painter.drawLine(i + 0.5, 0, i + 0.5, 500);
//    for(int j = 0; j < 500; j += 20)
//        painter.drawLine(0, j + 0.5, 500, j + 0.5);

    int min = int(qMin(width(), height())-80);
    QPoint center(int(width() / 2), int(height() / 2));

    painter.setBrush(Qt::black);
    painter.drawEllipse(center, min / 2, min / 2);

    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(QColor(230,230,250, 250),0.5));
    // 画三个环
    painter.drawEllipse(center, min / 2, min / 2);
    painter.drawEllipse(center, min / 3, min / 3);
    painter.drawEllipse(center, min / 6, min / 6);
    // 横竖直线
    painter.drawLine(QPointF(center.x(), center.y()-min/2), QPointF(center.x(), center.y()+min/2));
    painter.drawLine(QPointF(center.x()-min/2, center.y()), QPointF(center.x()+min/2, center.y()));
    // 撇捺直线
    painter.drawLine(QPointF(center.x()-((min/2)/sqrt(2)), center.y()-(min/2)/sqrt(2)), QPointF(center.x()+(min/2)/sqrt(2), center.y()+(min/2)/sqrt(2)));
    painter.drawLine(QPointF(center.x()+((min/2)/sqrt(2)), center.y()-(min/2)/sqrt(2)), QPointF(center.x()-(min/2)/sqrt(2), center.y()+(min/2)/sqrt(2)));

    // 大刻度
    for (int i=0; i<360;i+=10) {
        qreal x = center.x() + min/2 * cos((i-90)*3.14159265/180);
        qreal y = center.y() + min/2 * sin((i-90)*3.14159265/180);
        QPointF p(x-10 * cos((i-90)*3.14159265/180), y-10 * sin((i-90)*3.14159265/180));
        painter.drawLine(p, QPointF(x,y));
    }
    // 小刻度
    for (int i=0; i<360;i+=1) {
        qreal x = center.x() + min/2 * cos((i-90)*3.14159265/180);
        qreal y = center.y() + min/2 * sin((i-90)*3.14159265/180);
        QPointF p(x-5 * cos((i-90)*3.14159265/180), y-5 * sin((i-90)*3.14159265/180));
        painter.drawLine(p, QPointF(x,y));
    }

//    for (int i=0; i<180;i+=10) {
//        qreal x = center.x() + 250 * cos((i-90)*3.14159265/180);
//        qreal y = center.y() + 250 * sin((i-90)*3.14159265/180);
//        QPointF p2(x+10 * cos((i-90)*3.14159265/180), y+20 * sin((i-90)*3.14159265/180));
//        if(i<100 && i!=0)
//            painter.drawText(p2, '0'+QString::number(i));
//        else
//            painter.drawText(p2, QString::number(i));
//    }

    QFont font;
    font.setPointSize(10);
    // 刻度上数字
    for (int i=0; i<360;i+=10) {
        qreal x = center.x() + min/2 * cos((i-90)*3.14159265/180);
        qreal y = center.y() + min/2 * sin((i-90)*3.14159265/180);

        painter.setFont(font);
        painter.setPen(Qt::black);
        QFontMetrics fm = painter.fontMetrics();
        int width = fm.width(QString::number(i));
        int height = fm.ascent()+fm.descent();
        QRectF rect(x+18 * cos((i-90)*3.14159265/180)-width/2, y+18 * sin((i-90)*3.14159265/180)-height/2, width, height);
        painter.drawText(rect, QString::number(i));
    }

    if (m_drawable)
    {
        int diff = int(qAbs(width() - height()) / 2)+40;
        // 圆锥渐变
        QConicalGradient gradient(center.x(), center.y(), m_angle+240);
        gradient.setColorAt(0.1, QColor(34, 214, 78, 255));
        gradient.setColorAt(0.3, QColor(34, 214, 78, 150));
        gradient.setColorAt(0.5, QColor(34, 214, 78, 0));
        painter.setBrush(gradient);
        painter.setPen(QPen(Qt::transparent));
        if (width() > height())
            painter.drawPie(diff, 40, min, min, m_angle * 16, 60 * 16);
        else
            painter.drawPie(40, diff, min, min, m_angle * 16, 60 * 16);
        if(!m_points.isEmpty()){
            for(int i = 0; i < 1000; ++i)
            {
                painter.setBrush(QBrush(QColor(34, 214, 78, m_points.at(i).alpha)));
//                painter.drawEllipse(m_points.at(i).point, 3, 3);
                painter.drawChord(m_points.at(i).point.x()-28, m_points.at(i).point.y()-10, 30, 10, m_points.at(i).angle*16, 60*16);
            }
        }
        m_angle -= 1;
    }
}

void PPIWidget::resizeEvent(QResizeEvent *)
{
    w_width = width();
    w_height = height();
    radius = int(qMin(w_width, w_height)-80)/2;
//    qDebug() << "radius: " << radius;
}
