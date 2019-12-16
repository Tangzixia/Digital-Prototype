#ifndef PPIWIDGET_H
#define PPIWIDGET_H

#include <QPainter>
#include <QWidget>

namespace Ui {
class PPIWidget;
}

struct Point
{
    QPointF point;
    int alpha;
    int angle;
    Point(const QPointF &p, int a, int ang) : point(p) , alpha(a), angle(ang) { }
};

class PPIWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PPIWidget(QWidget *parent = nullptr);
    ~PPIWidget();
    void start();
    void stop();
    void get_data4m_points();

public slots:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);
private:
    Ui::PPIWidget *ui;
    bool m_drawable = false;
    int m_angle = 0;
    QList<Point> m_points;
    QTimer *m_updateTimer;
    QTimer *timer;
    int radius;
    int w_width, w_height;
};

#endif // PPIWIDGET_H
