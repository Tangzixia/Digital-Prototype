#ifndef RUNCOMPCONF_H
#define RUNCOMPCONF_H

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

signals:
    void rateSignal(float rate);
    void overRun();
    void startRun();

public slots:
    void On_start();
    void On_pause();
    void On_over();
    void On_rateSignal(float rate);
private:
    Ui::RunCompConf *ui;
};

#endif // RUNCOMPCONF_H
