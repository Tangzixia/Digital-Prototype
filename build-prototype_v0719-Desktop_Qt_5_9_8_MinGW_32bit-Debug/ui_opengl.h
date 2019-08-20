/********************************************************************************
** Form generated from reading UI file 'opengl.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENGL_H
#define UI_OPENGL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <myglwidget.h>

QT_BEGIN_NAMESPACE

class Ui_OpenGL
{
public:
    QGridLayout *gridLayout;
    myGLWidget *openGLWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSlider *xRotSlider;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSlider *yRotSlider;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSlider *zRotSlider;

    void setupUi(QWidget *OpenGL)
    {
        if (OpenGL->objectName().isEmpty())
            OpenGL->setObjectName(QStringLiteral("OpenGL"));
        OpenGL->resize(640, 480);
        gridLayout = new QGridLayout(OpenGL);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        openGLWidget = new myGLWidget(OpenGL);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));

        gridLayout->addWidget(openGLWidget, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(OpenGL);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        xRotSlider = new QSlider(OpenGL);
        xRotSlider->setObjectName(QStringLiteral("xRotSlider"));
        xRotSlider->setMaximum(360);
        xRotSlider->setSingleStep(16);
        xRotSlider->setPageStep(15);
        xRotSlider->setOrientation(Qt::Horizontal);
        xRotSlider->setTickPosition(QSlider::TicksAbove);
        xRotSlider->setTickInterval(15);

        horizontalLayout->addWidget(xRotSlider);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(OpenGL);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        yRotSlider = new QSlider(OpenGL);
        yRotSlider->setObjectName(QStringLiteral("yRotSlider"));
        yRotSlider->setMaximum(360);
        yRotSlider->setSingleStep(16);
        yRotSlider->setPageStep(15);
        yRotSlider->setOrientation(Qt::Horizontal);
        yRotSlider->setTickPosition(QSlider::TicksAbove);
        yRotSlider->setTickInterval(15);

        horizontalLayout_2->addWidget(yRotSlider);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(OpenGL);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        zRotSlider = new QSlider(OpenGL);
        zRotSlider->setObjectName(QStringLiteral("zRotSlider"));
        zRotSlider->setMaximum(360);
        zRotSlider->setSingleStep(16);
        zRotSlider->setPageStep(15);
        zRotSlider->setOrientation(Qt::Horizontal);
        zRotSlider->setTickPosition(QSlider::TicksAbove);
        zRotSlider->setTickInterval(15);

        horizontalLayout_3->addWidget(zRotSlider);


        verticalLayout->addLayout(horizontalLayout_3);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);


        retranslateUi(OpenGL);
        QObject::connect(xRotSlider, SIGNAL(valueChanged(int)), openGLWidget, SLOT(setXRotation(int)));
        QObject::connect(yRotSlider, SIGNAL(valueChanged(int)), openGLWidget, SLOT(setYRotation(int)));
        QObject::connect(zRotSlider, SIGNAL(valueChanged(int)), openGLWidget, SLOT(setZRotation(int)));

        QMetaObject::connectSlotsByName(OpenGL);
    } // setupUi

    void retranslateUi(QWidget *OpenGL)
    {
        OpenGL->setWindowTitle(QApplication::translate("OpenGL", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("OpenGL", "xRot", Q_NULLPTR));
        label_2->setText(QApplication::translate("OpenGL", "yRot", Q_NULLPTR));
        label_3->setText(QApplication::translate("OpenGL", "zRot", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class OpenGL: public Ui_OpenGL {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENGL_H
