#-------------------------------------------------
#
# Project created by QtCreator 2019-07-12T15:13:11
#
#-------------------------------------------------

QT       += core gui
QT       += xml

#---add manually
INCLUDEPATH += $$PWD/AlgoLib
INCLUDEPATH += $$PWD/CalcMachine
INCLUDEPATH += $$PWD/EccmPrototype
INCLUDEPATH += $$PWD/Main
INCLUDEPATH += $$PWD/ObjectPrototype
INCLUDEPATH += $$PWD/RadarPrototype
INCLUDEPATH += $$PWD/Res
INCLUDEPATH += $$PWD/Simulation
INCLUDEPATH += $$PWD/Tool
#---

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prototype_v0906
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
        Main\main.cpp \
        Main\mainwindow.cpp \
    Tool\myglwidget.cpp \
    Tool\opengl.cpp \
    Main\hellodialog.cpp \
    Main\mysplashscreen.cpp \
    Main\mainwindownew.cpp \
    Main\draglistwidget.cpp \
    Main\mainwindownewscene.cpp \
    RadarPrototype\radaritem.cpp \
    Tool\arrowitem.cpp \
    RadarPrototype\mainwindow_radar.cpp \
    RadarPrototype\radarproperty.cpp \
    RadarPrototype\radarscene.cpp \
    RadarPrototype\diagramitem.cpp \
    Tool\arrow.cpp \
    RadarPrototype\diagramtextitem.cpp \
    Tool\compproperty.cpp \
    EccmPrototype\ecmdraglistwidget.cpp \
    EccmPrototype\ecmitem.cpp \
    Tool\clickablelabel.cpp \
    Tool\realtimecurve.cpp \
    Tool/qudong.cpp

HEADERS += \
        Main\mainwindow.h \
    Tool\myglwidget.h \
    Tool\opengl.h \
    Main\hellodialog.h \
    Main\mysplashscreen.h \
    Main\mainwindownew.h \
    Main\draglistwidget.h \
    Main\mainwindownewscene.h \
    RadarPrototype\radaritem.h \
    Tool\arrowitem.h \
    RadarPrototype\mainwindow_radar.h \
    RadarPrototype\radarproperty.h \
    RadarPrototype\radarscene.h \
    RadarPrototype\diagramitem.h \
    Tool\arrow.h \
    RadarPrototype\diagramtextitem.h \
    Tool\compproperty.h \
    EccmPrototype\ecmdraglistwidget.h \
    EccmPrototype\ecmitem.h \
    Tool\clickablelabel.h \
    Tool\realtimecurve.h \
    Tool/qudong.h

FORMS += \
        Main\mainwindow.ui \
    Tool\opengl.ui \
    Main\hellodialog.ui \
    Main\mainwindownew.ui \
    RadarPrototype\mainwindow_radar.ui \
    RadarPrototype\radarproperty.ui \
    Tool\compproperty.ui \
    Tool\realtimecurve.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Res\res.qrc
