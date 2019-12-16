#ifndef RUNCOMPCONF_H
#define RUNCOMPCONF_H

#include "qcustomplot.h"

#include <QDialog>

namespace Ui {
class RunCompConf;
}

class RunCompConf : public QDialog
{
    Q_OBJECT

public:
    explicit RunCompConf(QWidget *parent = nullptr);
    ~RunCompConf();
    //设置qcustomplot画图属性，实时
    void setupRealtimeDataDemo(QCustomPlot *customPlot);
    void readFile(QString filePath);

signals:
    void rateSignal(float rate);
    void overRun();
    void startRun();

public slots:
    void On_start();
    void On_pause();
    void On_over();
    void On_rateSignal(float rate);
private slots:
    //添加实时数据槽
    void realtimeDataSlot();

private:
    Ui::RunCompConf *ui;
    //定时器，周期调用realtimeDataSlot()槽，实现动态数据添加到曲线
    QTimer dataTimer;
    QVector<double> realArray;
    int i=0;
};

#endif // RUNCOMPCONF_H
