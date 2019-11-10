#ifndef A_SHOW_H
#define A_SHOW_H

#include <QWidget>

namespace Ui {
class A_show;
}

class A_show : public QWidget
{
    Q_OBJECT

public:
    explicit A_show(QWidget *parent = nullptr);
    ~A_show();

public slots:
    void paintEvent(QPaintEvent *);

private:
    Ui::A_show *ui;
};

#endif // A_SHOW_H
