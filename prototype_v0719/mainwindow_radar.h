#ifndef MAINWINDOW_RADAR_H
#define MAINWINDOW_RADAR_H
#include <QLabel>
#include <QMainWindow>

namespace Ui {
class MainWindow_Radar;
}

class MainWindow_Radar : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_Radar(QWidget *parent = nullptr);
    ~MainWindow_Radar();

protected:
     void closeEvent(QCloseEvent *event);
private slots:
     void on_actionCom_list_triggered();
     void xy_show(double x,double y);

private:
    Ui::MainWindow_Radar *ui;
    QLabel *label_xy;
};

#endif // MAINWINDOW_RADAR_H
