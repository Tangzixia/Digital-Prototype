#include "opengl.h"
#include "ui_opengl.h"
#include <QKeyEvent>

OpenGL::OpenGL(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenGL)
{
    ui->setupUi(this);
    connect(ui->openGLWidget, SIGNAL(xRotationChanged(int)), ui->xRotSlider, SLOT(setValue(int)));
    connect(ui->openGLWidget, SIGNAL(yRotationChanged(int)), ui->yRotSlider, SLOT(setValue(int)));
    connect(ui->openGLWidget, SIGNAL(zRotationChanged(int)), ui->zRotSlider, SLOT(setValue(int)));
}
OpenGL::~OpenGL()
{
    delete ui;
}

void OpenGL::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}
