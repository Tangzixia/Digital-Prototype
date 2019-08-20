/********************************************************************************
** Form generated from reading UI file 'radarproperty.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RADARPROPERTY_H
#define UI_RADARPROPERTY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RadarProperty
{
public:

    void setupUi(QWidget *RadarProperty)
    {
        if (RadarProperty->objectName().isEmpty())
            RadarProperty->setObjectName(QStringLiteral("RadarProperty"));
        RadarProperty->resize(511, 348);

        retranslateUi(RadarProperty);

        QMetaObject::connectSlotsByName(RadarProperty);
    } // setupUi

    void retranslateUi(QWidget *RadarProperty)
    {
        RadarProperty->setWindowTitle(QApplication::translate("RadarProperty", "\351\233\267\350\276\276\345\261\236\346\200\247", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RadarProperty: public Ui_RadarProperty {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RADARPROPERTY_H
