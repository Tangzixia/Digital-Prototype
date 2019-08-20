/********************************************************************************
** Form generated from reading UI file 'hellodialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELLODIALOG_H
#define UI_HELLODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_HelloDialog
{
public:
    QLabel *label;
    QPushButton *pushButton_open;
    QPushButton *pushButton_new;

    void setupUi(QDialog *HelloDialog)
    {
        if (HelloDialog->objectName().isEmpty())
            HelloDialog->setObjectName(QStringLiteral("HelloDialog"));
        HelloDialog->resize(540, 300);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(HelloDialog->sizePolicy().hasHeightForWidth());
        HelloDialog->setSizePolicy(sizePolicy);
        HelloDialog->setMinimumSize(QSize(540, 300));
        HelloDialog->setMaximumSize(QSize(540, 300));
        HelloDialog->setCursor(QCursor(Qt::ArrowCursor));
        HelloDialog->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        label = new QLabel(HelloDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 40, 220, 220));
        label->setPixmap(QPixmap(QString::fromUtf8(":/img/radar.jpg")));
        pushButton_open = new QPushButton(HelloDialog);
        pushButton_open->setObjectName(QStringLiteral("pushButton_open"));
        pushButton_open->setGeometry(QRect(260, 90, 120, 120));
        sizePolicy.setHeightForWidth(pushButton_open->sizePolicy().hasHeightForWidth());
        pushButton_open->setSizePolicy(sizePolicy);
        pushButton_open->setMinimumSize(QSize(120, 120));
        pushButton_open->setMaximumSize(QSize(120, 120));
        pushButton_open->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_open->setAutoFillBackground(false);
        pushButton_open->setStyleSheet(QStringLiteral(""));
        QIcon icon;
        icon.addFile(QStringLiteral(":/img/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_open->setIcon(icon);
        pushButton_open->setIconSize(QSize(48, 48));
        pushButton_open->setAutoDefault(false);
        pushButton_open->setFlat(true);
        pushButton_new = new QPushButton(HelloDialog);
        pushButton_new->setObjectName(QStringLiteral("pushButton_new"));
        pushButton_new->setGeometry(QRect(400, 90, 120, 120));
        sizePolicy.setHeightForWidth(pushButton_new->sizePolicy().hasHeightForWidth());
        pushButton_new->setSizePolicy(sizePolicy);
        pushButton_new->setMinimumSize(QSize(120, 120));
        pushButton_new->setMaximumSize(QSize(120, 120));
        pushButton_new->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_new->setAutoFillBackground(false);
        pushButton_new->setStyleSheet(QStringLiteral(""));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/img/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_new->setIcon(icon1);
        pushButton_new->setIconSize(QSize(48, 48));
        pushButton_new->setAutoDefault(false);
        pushButton_new->setFlat(true);

        retranslateUi(HelloDialog);

        pushButton_open->setDefault(false);
        pushButton_new->setDefault(false);


        QMetaObject::connectSlotsByName(HelloDialog);
    } // setupUi

    void retranslateUi(QDialog *HelloDialog)
    {
        HelloDialog->setWindowTitle(QApplication::translate("HelloDialog", "\345\274\200\345\247\213", Q_NULLPTR));
        label->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_open->setToolTip(QApplication::translate("HelloDialog", "\346\211\223\345\274\200\345\267\245\347\250\213", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        pushButton_open->setStatusTip(QApplication::translate("HelloDialog", "\346\211\223\345\274\200\345\267\245\347\250\213", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        pushButton_open->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_new->setToolTip(QApplication::translate("HelloDialog", "\346\226\260\345\273\272\345\267\245\347\250\213", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        pushButton_new->setStatusTip(QApplication::translate("HelloDialog", "\346\226\260\345\273\272\345\267\245\347\250\213", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        pushButton_new->setText(QString());
#ifndef QT_NO_SHORTCUT
        pushButton_new->setShortcut(QApplication::translate("HelloDialog", "Ctrl+S", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
    } // retranslateUi

};

namespace Ui {
    class HelloDialog: public Ui_HelloDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELLODIALOG_H
