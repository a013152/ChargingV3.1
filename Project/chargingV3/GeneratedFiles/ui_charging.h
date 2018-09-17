/********************************************************************************
** Form generated from reading UI file 'charging.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARGING_H
#define UI_CHARGING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_chargingClass
{
public:
    QWidget *centralWidget;
    QPushButton *btnMenuSys;
    QPushButton *btnMenuCom;
    QPushButton *btnSysMin;
    QPushButton *btnSysClose;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *btnPrePage;
    QPushButton *btnNextPage;
    QWidget *tab_2;
    QTextEdit *textEdit_show;
    QLabel *lab_temperature;
    QLabel *lab_temperature_title;
    QLabel *lab_temperature_imge;
    QLabel *lab_temperature_number;
    QLabel *lab_logo;
    QLabel *lab_title;

    void setupUi(QMainWindow *chargingClass)
    {
        if (chargingClass->objectName().isEmpty())
            chargingClass->setObjectName(QStringLiteral("chargingClass"));
        chargingClass->resize(1300, 700);
        centralWidget = new QWidget(chargingClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btnMenuSys = new QPushButton(centralWidget);
        btnMenuSys->setObjectName(QStringLiteral("btnMenuSys"));
        btnMenuSys->setGeometry(QRect(1030, 10, 75, 23));
        btnMenuCom = new QPushButton(centralWidget);
        btnMenuCom->setObjectName(QStringLiteral("btnMenuCom"));
        btnMenuCom->setGeometry(QRect(1120, 10, 75, 23));
        btnSysMin = new QPushButton(centralWidget);
        btnSysMin->setObjectName(QStringLiteral("btnSysMin"));
        btnSysMin->setGeometry(QRect(1220, 40, 75, 23));
        btnSysMin->setStyleSheet(QStringLiteral(""));
        btnSysClose = new QPushButton(centralWidget);
        btnSysClose->setObjectName(QStringLiteral("btnSysClose"));
        btnSysClose->setGeometry(QRect(1220, 10, 75, 23));
        btnSysClose->setStyleSheet(QStringLiteral(""));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(950, 140, 350, 520));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        btnPrePage = new QPushButton(tab);
        btnPrePage->setObjectName(QStringLiteral("btnPrePage"));
        btnPrePage->setGeometry(QRect(153, 0, 44, 82));
        btnNextPage = new QPushButton(tab);
        btnNextPage->setObjectName(QStringLiteral("btnNextPage"));
        btnNextPage->setGeometry(QRect(153, 410, 44, 82));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        textEdit_show = new QTextEdit(tab_2);
        textEdit_show->setObjectName(QStringLiteral("textEdit_show"));
        textEdit_show->setGeometry(QRect(2, 2, 342, 490));
        tabWidget->addTab(tab_2, QString());
        lab_temperature = new QLabel(centralWidget);
        lab_temperature->setObjectName(QStringLiteral("lab_temperature"));
        lab_temperature->setGeometry(QRect(930, 75, 350, 90));
        lab_temperature_title = new QLabel(centralWidget);
        lab_temperature_title->setObjectName(QStringLiteral("lab_temperature_title"));
        lab_temperature_title->setGeometry(QRect(1065, 75, 80, 20));
        lab_temperature_title->setMargin(0);
        lab_temperature_imge = new QLabel(centralWidget);
        lab_temperature_imge->setObjectName(QStringLiteral("lab_temperature_imge"));
        lab_temperature_imge->setGeometry(QRect(970, 100, 263, 57));
        lab_temperature_imge->setMargin(0);
        lab_temperature_number = new QLabel(centralWidget);
        lab_temperature_number->setObjectName(QStringLiteral("lab_temperature_number"));
        lab_temperature_number->setGeometry(QRect(1155, 110, 80, 40));
        lab_temperature_number->setMargin(0);
        lab_logo = new QLabel(centralWidget);
        lab_logo->setObjectName(QStringLiteral("lab_logo"));
        lab_logo->setGeometry(QRect(3, 3, 79, 62));
        lab_title = new QLabel(centralWidget);
        lab_title->setObjectName(QStringLiteral("lab_title"));
        lab_title->setGeometry(QRect(90, 3, 351, 62));
        chargingClass->setCentralWidget(centralWidget);

        retranslateUi(chargingClass);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(chargingClass);
    } // setupUi

    void retranslateUi(QMainWindow *chargingClass)
    {
        chargingClass->setWindowTitle(QApplication::translate("chargingClass", "charging", 0));
        btnMenuSys->setText(QApplication::translate("chargingClass", "\346\223\215\344\275\234", 0));
        btnMenuCom->setText(QApplication::translate("chargingClass", "\344\270\262\345\217\243", 0));
        btnSysMin->setText(QApplication::translate("chargingClass", "\346\234\200\345\260\217\345\214\226", 0));
        btnSysClose->setText(QApplication::translate("chargingClass", "\345\205\263\351\227\255", 0));
        btnPrePage->setText(QApplication::translate("chargingClass", "\344\270\212\344\270\200\351\241\265", 0));
        btnNextPage->setText(QApplication::translate("chargingClass", "\344\270\213\344\270\200\351\241\265", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("chargingClass", "\345\205\205\347\224\265\346\237\234\344\277\241\346\201\257", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("chargingClass", "\351\200\232\350\256\257\344\277\241\346\201\257", 0));
        lab_temperature->setText(QString());
        lab_temperature_title->setText(QApplication::translate("chargingClass", "\346\270\251\345\272\246\347\212\266\346\200\201", 0));
        lab_temperature_imge->setText(QApplication::translate("chargingClass", "\346\270\251\345\272\246\347\212\266\346\200\201\345\233\276", 0));
        lab_temperature_number->setText(QApplication::translate("chargingClass", "\346\270\251\345\272\246\347\212\266\346\200\201", 0));
        lab_logo->setText(QApplication::translate("chargingClass", "Logo", 0));
        lab_title->setText(QApplication::translate("chargingClass", "title", 0));
    } // retranslateUi

};

namespace Ui {
    class chargingClass: public Ui_chargingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARGING_H
