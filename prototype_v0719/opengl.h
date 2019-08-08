#ifndef OPENGL_H
#define OPENGL_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class OpenGL;
}

class OpenGL : public QDialog
{
    Q_OBJECT

public:
    explicit OpenGL(QWidget *parent = nullptr);
    ~OpenGL();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::OpenGL *ui;
};

#endif // OPENGL_H
