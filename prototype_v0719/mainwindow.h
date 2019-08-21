#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/**
* @projectName   prototype_v0719
* @brief         整个程序的主窗口：打开现有的工程时。
* @author        Antrn
* @date          2019-08-12
*/
#include "opengl.h"

#include <QCloseEvent>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void closeEvent(QCloseEvent *event);

private slots:

    void on_actionOpenGL_triggered();
    void on_actionexit_triggered();


private:
    Ui::MainWindow *ui;
    OpenGL *openGL;

    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_H
