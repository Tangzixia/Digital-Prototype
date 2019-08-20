/********************************************************************************
** Form generated from reading UI file 'mainwindow_radar.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_RADAR_H
#define UI_MAINWINDOW_RADAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow_Radar
{
public:
    QAction *actionCom_list;
    QAction *actionOpenXml;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QMenu *menu;
    QStatusBar *statusbar;
    QDockWidget *dockCompList;
    QWidget *dockWidgetContents;

    void setupUi(QMainWindow *MainWindow_Radar)
    {
        if (MainWindow_Radar->objectName().isEmpty())
            MainWindow_Radar->setObjectName(QStringLiteral("MainWindow_Radar"));
        MainWindow_Radar->resize(921, 479);
        actionCom_list = new QAction(MainWindow_Radar);
        actionCom_list->setObjectName(QStringLiteral("actionCom_list"));
        actionCom_list->setCheckable(true);
        actionCom_list->setChecked(false);
        actionOpenXml = new QAction(MainWindow_Radar);
        actionOpenXml->setObjectName(QStringLiteral("actionOpenXml"));
        centralwidget = new QWidget(MainWindow_Radar);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        MainWindow_Radar->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow_Radar);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 921, 26));
        menu = new QMenu(menubar);
        menu->setObjectName(QStringLiteral("menu"));
        MainWindow_Radar->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow_Radar);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow_Radar->setStatusBar(statusbar);
        dockCompList = new QDockWidget(MainWindow_Radar);
        dockCompList->setObjectName(QStringLiteral("dockCompList"));
        dockCompList->setFloating(false);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        dockCompList->setWidget(dockWidgetContents);
        MainWindow_Radar->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockCompList);

        menubar->addAction(menu->menuAction());
        menu->addAction(actionCom_list);
        menu->addAction(actionOpenXml);

        retranslateUi(MainWindow_Radar);

        QMetaObject::connectSlotsByName(MainWindow_Radar);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow_Radar)
    {
        MainWindow_Radar->setWindowTitle(QApplication::translate("MainWindow_Radar", "\347\274\226\350\276\221\351\233\267\350\276\276", Q_NULLPTR));
        actionCom_list->setText(QApplication::translate("MainWindow_Radar", "\347\273\204\344\273\266\345\210\227\350\241\250", Q_NULLPTR));
        actionOpenXml->setText(QApplication::translate("MainWindow_Radar", "\346\211\223\345\274\200xml", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow_Radar", "\345\267\245\345\205\267&T", Q_NULLPTR));
        dockCompList->setWindowTitle(QApplication::translate("MainWindow_Radar", "\351\233\267\350\276\276\347\273\204\344\273\266\345\210\227\350\241\250", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow_Radar: public Ui_MainWindow_Radar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_RADAR_H
