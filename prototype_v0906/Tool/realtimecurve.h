#ifndef REALTIMECURVE_H
#define REALTIMECURVE_H

#include <QMainWindow>

namespace Ui {
class RealTimeCurve;
}

class RealTimeCurve : public QMainWindow
{
    Q_OBJECT

public:
    explicit RealTimeCurve(QWidget *parent = nullptr);
    ~RealTimeCurve();

private:
    Ui::RealTimeCurve *ui;
    int p, t;   //p represent the x coordinate of the start point of painting, t represent the  x coordinate of time
    QTimer *timer;
    QPoint *point;
    QPainterPath *path;
    void virtual paintEvent(QPaintEvent *event);

private slots:
    void timerUpdate();
    void startPainting();
    void stopPainting();
};

#endif // REALTIMECURVE_H
