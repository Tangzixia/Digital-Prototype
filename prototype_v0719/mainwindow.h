#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow *ui;
    OpenGL *openGL;

    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_H
