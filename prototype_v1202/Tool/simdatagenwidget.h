#ifndef SIMDATAGENWIDGET_H
#define SIMDATAGENWIDGET_H

#include <QWidget>

namespace Ui {
class SimDataGenWidget;
}

class SimDataGenWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimDataGenWidget(QWidget *parent = nullptr);
    ~SimDataGenWidget();

private slots:
//    void on_pushButton_select_clicked();

    void on_buttonBox_accepted();

    void on_pushButton_load_clicked();

    void on_buttonBox_rejected();

private:
    Ui::SimDataGenWidget *ui;
};

#endif // SIMDATAGENWIDGET_H
