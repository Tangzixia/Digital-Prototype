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

private:
    Ui::RunCompConf *ui;
};

#endif // RUNCOMPCONF_H
