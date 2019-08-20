#-------------------------------------------------
#
# Project created by QtCreator 2019-07-12T15:13:11
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prototype_v0719
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

win32:LIBS += -lOpengl32 \
                -lglu32 \
                -lglut

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    myglwidget.cpp \
    opengl.cpp \
    hellodialog.cpp \
    mysplashscreen.cpp \
    mainwindownew.cpp \
    draglistwidget.cpp \
    mainwindownewscene.cpp \
    radaritem.cpp \
    arrowitem.cpp \
    mainwindow_radar.cpp \
    radarproperty.cpp \
    radarscene.cpp \
    diagramitem.cpp \
    arrow.cpp \
    diagramtextitem.cpp \
    compproperty.cpp \
    ecmdraglistwidget.cpp \
    ecmitem.cpp

HEADERS += \
        mainwindow.h \
    myglwidget.h \
    opengl.h \
    hellodialog.h \
    mysplashscreen.h \
    mainwindownew.h \
    draglistwidget.h \
    mainwindownewscene.h \
    radaritem.h \
    arrowitem.h \
    mainwindow_radar.h \
    radarproperty.h \
    radarscene.h \
    diagramitem.h \
    arrow.h \
    diagramtextitem.h \
    compproperty.h \
    ecmdraglistwidget.h \
    ecmitem.h

FORMS += \
        mainwindow.ui \
    opengl.ui \
    hellodialog.ui \
    mainwindownew.ui \
    mainwindow_radar.ui \
    radarproperty.ui \
    compproperty.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
