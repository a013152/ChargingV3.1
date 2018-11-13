/********************************************************************************
** Form generated from reading UI file 'dataDialog01.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATADIALOG01_H
#define UI_DATADIALOG01_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_dataDialog01
{
public:

    void setupUi(QDialog *dataDialog01)
    {
        if (dataDialog01->objectName().isEmpty())
            dataDialog01->setObjectName(QStringLiteral("dataDialog01"));
        dataDialog01->resize(760, 540);

        retranslateUi(dataDialog01);

        QMetaObject::connectSlotsByName(dataDialog01);
    } // setupUi

    void retranslateUi(QDialog *dataDialog01)
    {
        dataDialog01->setWindowTitle(QApplication::translate("dataDialog01", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class dataDialog01: public Ui_dataDialog01 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATADIALOG01_H
