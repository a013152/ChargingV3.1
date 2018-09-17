/********************************************************************************
** Form generated from reading UI file 'messageBoxCus.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEBOXCUS_H
#define UI_MESSAGEBOXCUS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_MessageBoxCus
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *lab_logo;
    QLabel *lab_message;

    void setupUi(QDialog *MessageBoxCus)
    {
        if (MessageBoxCus->objectName().isEmpty())
            MessageBoxCus->setObjectName(QStringLiteral("MessageBoxCus"));
        MessageBoxCus->resize(320, 105);
        pushButton = new QPushButton(MessageBoxCus);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(123, 62, 75, 30));
        pushButton_2 = new QPushButton(MessageBoxCus);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(210, 62, 75, 30));
        lab_logo = new QLabel(MessageBoxCus);
        lab_logo->setObjectName(QStringLiteral("lab_logo"));
        lab_logo->setGeometry(QRect(10, 10, 61, 61));
        lab_message = new QLabel(MessageBoxCus);
        lab_message->setObjectName(QStringLiteral("lab_message"));
        lab_message->setGeometry(QRect(80, 10, 240, 50));
        lab_message->setMargin(1);

        retranslateUi(MessageBoxCus);

        QMetaObject::connectSlotsByName(MessageBoxCus);
    } // setupUi

    void retranslateUi(QDialog *MessageBoxCus)
    {
        MessageBoxCus->setWindowTitle(QApplication::translate("MessageBoxCus", "MessageBoxCus", 0));
        pushButton->setText(QApplication::translate("MessageBoxCus", "\347\241\256\345\256\232", 0));
        pushButton_2->setText(QApplication::translate("MessageBoxCus", "\345\217\226\346\266\210", 0));
        lab_logo->setText(QApplication::translate("MessageBoxCus", "logo", 0));
        lab_message->setText(QApplication::translate("MessageBoxCus", "title", 0));
    } // retranslateUi

};

namespace Ui {
    class MessageBoxCus: public Ui_MessageBoxCus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEBOXCUS_H
