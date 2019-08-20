/********************************************************************************
** Form generated from reading UI file 'compproperty.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPPROPERTY_H
#define UI_COMPPROPERTY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CompProperty
{
public:
    QLabel *label;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *CompProperty)
    {
        if (CompProperty->objectName().isEmpty())
            CompProperty->setObjectName(QStringLiteral("CompProperty"));
        CompProperty->resize(292, 239);
        label = new QLabel(CompProperty);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(120, 70, 51, 31));
        QFont font;
        font.setPointSize(19);
        label->setFont(font);
        layoutWidget = new QWidget(CompProperty);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 200, 291, 25));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        retranslateUi(CompProperty);
        QObject::connect(buttonBox, SIGNAL(accepted()), CompProperty, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CompProperty, SLOT(reject()));

        QMetaObject::connectSlotsByName(CompProperty);
    } // setupUi

    void retranslateUi(QDialog *CompProperty)
    {
        CompProperty->setWindowTitle(QApplication::translate("CompProperty", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("CompProperty", "\345\261\236\346\200\247", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CompProperty: public Ui_CompProperty {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPPROPERTY_H
