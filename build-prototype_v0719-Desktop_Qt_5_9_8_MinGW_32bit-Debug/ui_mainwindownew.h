/********************************************************************************
** Form generated from reading UI file 'mainwindownew.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOWNEW_H
#define UI_MAINWINDOWNEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <ecmdraglistwidget.h>
#include "draglistwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindowNew
{
public:
    QAction *actionnewFile;
    QAction *actionsaveFile;
    QAction *actionabout_DPSP;
    QAction *actionabout_us;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QPushButton *RadarListButton;
    DragListWidget *RadarListWidget;
    QPushButton *ECMListButton;
    ECMDragListWidget *ECMListWidget;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QMenu *menufile;
    QMenu *menuedit;
    QMenu *menusetting;
    QMenu *menuhelp;
    QMenu *menuabout;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowNew)
    {
        if (MainWindowNew->objectName().isEmpty())
            MainWindowNew->setObjectName(QStringLiteral("MainWindowNew"));
        MainWindowNew->resize(810, 496);
        actionnewFile = new QAction(MainWindowNew);
        actionnewFile->setObjectName(QStringLiteral("actionnewFile"));
        actionsaveFile = new QAction(MainWindowNew);
        actionsaveFile->setObjectName(QStringLiteral("actionsaveFile"));
        actionabout_DPSP = new QAction(MainWindowNew);
        actionabout_DPSP->setObjectName(QStringLiteral("actionabout_DPSP"));
        actionabout_us = new QAction(MainWindowNew);
        actionabout_us->setObjectName(QStringLiteral("actionabout_us"));
        centralwidget = new QWidget(MainWindowNew);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setFrameShape(QFrame::StyledPanel);
        graphicsView->setFrameShadow(QFrame::Plain);
        graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

        gridLayout->addWidget(graphicsView, 0, 1, 1, 1);

        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 129, 431));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        RadarListButton = new QPushButton(scrollAreaWidgetContents);
        RadarListButton->setObjectName(QStringLiteral("RadarListButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RadarListButton->sizePolicy().hasHeightForWidth());
        RadarListButton->setSizePolicy(sizePolicy);
        RadarListButton->setMinimumSize(QSize(0, 35));
        RadarListButton->setMaximumSize(QSize(16777215, 35));
        RadarListButton->setAutoDefault(false);
        RadarListButton->setFlat(false);

        verticalLayout->addWidget(RadarListButton);

        RadarListWidget = new DragListWidget(scrollAreaWidgetContents);
        RadarListWidget->setObjectName(QStringLiteral("RadarListWidget"));
        RadarListWidget->setMinimumSize(QSize(100, 50));
        RadarListWidget->setMaximumSize(QSize(100000, 100000));

        verticalLayout->addWidget(RadarListWidget);

        ECMListButton = new QPushButton(scrollAreaWidgetContents);
        ECMListButton->setObjectName(QStringLiteral("ECMListButton"));
        ECMListButton->setMinimumSize(QSize(0, 35));

        verticalLayout->addWidget(ECMListButton);

        ECMListWidget = new ECMDragListWidget(scrollAreaWidgetContents);
        ECMListWidget->setObjectName(QStringLiteral("ECMListWidget"));

        verticalLayout->addWidget(ECMListWidget);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer = new QSpacerItem(20, 150, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 5);
        MainWindowNew->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindowNew);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 810, 23));
        menufile = new QMenu(menubar);
        menufile->setObjectName(QStringLiteral("menufile"));
        menuedit = new QMenu(menubar);
        menuedit->setObjectName(QStringLiteral("menuedit"));
        menusetting = new QMenu(menubar);
        menusetting->setObjectName(QStringLiteral("menusetting"));
        menuhelp = new QMenu(menubar);
        menuhelp->setObjectName(QStringLiteral("menuhelp"));
        menuabout = new QMenu(menubar);
        menuabout->setObjectName(QStringLiteral("menuabout"));
        MainWindowNew->setMenuBar(menubar);
        statusBar = new QStatusBar(MainWindowNew);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowNew->setStatusBar(statusBar);

        menubar->addAction(menufile->menuAction());
        menubar->addAction(menuedit->menuAction());
        menubar->addAction(menusetting->menuAction());
        menubar->addAction(menuhelp->menuAction());
        menubar->addAction(menuabout->menuAction());
        menufile->addAction(actionnewFile);
        menufile->addAction(actionsaveFile);
        menuabout->addAction(actionabout_DPSP);
        menuabout->addAction(actionabout_us);

        retranslateUi(MainWindowNew);

        RadarListButton->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindowNew);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowNew)
    {
        MainWindowNew->setWindowTitle(QApplication::translate("MainWindowNew", "MainWindow", Q_NULLPTR));
        actionnewFile->setText(QApplication::translate("MainWindowNew", "newFile", Q_NULLPTR));
        actionsaveFile->setText(QApplication::translate("MainWindowNew", "saveFile", Q_NULLPTR));
        actionabout_DPSP->setText(QApplication::translate("MainWindowNew", "about DPSP", Q_NULLPTR));
        actionabout_us->setText(QApplication::translate("MainWindowNew", "about us", Q_NULLPTR));
        RadarListButton->setText(QApplication::translate("MainWindowNew", "\351\233\267\350\276\276", Q_NULLPTR));
        ECMListButton->setText(QApplication::translate("MainWindowNew", "\345\257\271\346\212\227", Q_NULLPTR));
        menufile->setTitle(QApplication::translate("MainWindowNew", "file", Q_NULLPTR));
        menuedit->setTitle(QApplication::translate("MainWindowNew", "edit", Q_NULLPTR));
        menusetting->setTitle(QApplication::translate("MainWindowNew", "setting", Q_NULLPTR));
        menuhelp->setTitle(QApplication::translate("MainWindowNew", "help", Q_NULLPTR));
        menuabout->setTitle(QApplication::translate("MainWindowNew", "about", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindowNew: public Ui_MainWindowNew {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOWNEW_H
