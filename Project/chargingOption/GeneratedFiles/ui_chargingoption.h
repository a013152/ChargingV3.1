/********************************************************************************
** Form generated from reading UI file 'chargingoption.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARGINGOPTION_H
#define UI_CHARGINGOPTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_chargingOptionClass
{
public:
    QWidget *centralWidget;
    QTabWidget *tablePage;
    QWidget *tab_1;
    QTableWidget *tableBattery;
    QComboBox *cmbCloset;
    QLabel *label;
    QPushButton *btnRefresh_1;
    QPushButton *btnDel_1;
    QPushButton *btnSave_1;
    QPushButton *btnAdd_1;
    QWidget *tab_2;
    QTableWidget *tableBatteryModel;
    QPushButton *btnDel_2;
    QPushButton *btnAdd_2;
    QPushButton *btnSave_2;
    QPushButton *btnRefresh_2;
    QWidget *tab;
    QPushButton *btnQuery_1;
    QComboBox *cmbCharger;
    QLabel *label_8;
    QTableWidget *tableChargeRecord;
    QDateTimeEdit *dateTime_Begin;
    QCheckBox *checkBox_1;
    QCheckBox *checkBox_2;
    QLabel *label_9;
    QDateTimeEdit *dateTime_End;
    QCheckBox *checkBox_3;
    QLabel *label_10;
    QWidget *tab_3;
    QGroupBox *groupBox;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label_4;
    QLineEdit *lineEdit_3;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QPushButton *btnSave_3;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *chargingOptionClass)
    {
        if (chargingOptionClass->objectName().isEmpty())
            chargingOptionClass->setObjectName(QStringLiteral("chargingOptionClass"));
        chargingOptionClass->resize(790, 570);
        centralWidget = new QWidget(chargingOptionClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tablePage = new QTabWidget(centralWidget);
        tablePage->setObjectName(QStringLiteral("tablePage"));
        tablePage->setGeometry(QRect(5, 10, 785, 501));
        tab_1 = new QWidget();
        tab_1->setObjectName(QStringLiteral("tab_1"));
        tableBattery = new QTableWidget(tab_1);
        tableBattery->setObjectName(QStringLiteral("tableBattery"));
        tableBattery->setGeometry(QRect(0, 0, 651, 471));
        cmbCloset = new QComboBox(tab_1);
        cmbCloset->setObjectName(QStringLiteral("cmbCloset"));
        cmbCloset->setGeometry(QRect(705, 39, 69, 21));
        label = new QLabel(tab_1);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(660, 41, 48, 16));
        btnRefresh_1 = new QPushButton(tab_1);
        btnRefresh_1->setObjectName(QStringLiteral("btnRefresh_1"));
        btnRefresh_1->setGeometry(QRect(690, 120, 81, 41));
        btnDel_1 = new QPushButton(tab_1);
        btnDel_1->setObjectName(QStringLiteral("btnDel_1"));
        btnDel_1->setGeometry(QRect(690, 240, 81, 41));
        btnSave_1 = new QPushButton(tab_1);
        btnSave_1->setObjectName(QStringLiteral("btnSave_1"));
        btnSave_1->setGeometry(QRect(690, 300, 81, 41));
        btnAdd_1 = new QPushButton(tab_1);
        btnAdd_1->setObjectName(QStringLiteral("btnAdd_1"));
        btnAdd_1->setGeometry(QRect(690, 180, 81, 41));
        tablePage->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tableBatteryModel = new QTableWidget(tab_2);
        tableBatteryModel->setObjectName(QStringLiteral("tableBatteryModel"));
        tableBatteryModel->setGeometry(QRect(0, 0, 651, 471));
        btnDel_2 = new QPushButton(tab_2);
        btnDel_2->setObjectName(QStringLiteral("btnDel_2"));
        btnDel_2->setGeometry(QRect(690, 240, 81, 41));
        btnAdd_2 = new QPushButton(tab_2);
        btnAdd_2->setObjectName(QStringLiteral("btnAdd_2"));
        btnAdd_2->setGeometry(QRect(690, 180, 81, 41));
        btnSave_2 = new QPushButton(tab_2);
        btnSave_2->setObjectName(QStringLiteral("btnSave_2"));
        btnSave_2->setGeometry(QRect(690, 300, 81, 41));
        btnRefresh_2 = new QPushButton(tab_2);
        btnRefresh_2->setObjectName(QStringLiteral("btnRefresh_2"));
        btnRefresh_2->setGeometry(QRect(690, 120, 81, 41));
        tablePage->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        btnQuery_1 = new QPushButton(tab);
        btnQuery_1->setObjectName(QStringLiteral("btnQuery_1"));
        btnQuery_1->setGeometry(QRect(670, 280, 81, 41));
        cmbCharger = new QComboBox(tab);
        cmbCharger->setObjectName(QStringLiteral("cmbCharger"));
        cmbCharger->setGeometry(QRect(690, 60, 61, 21));
        label_8 = new QLabel(tab);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(655, 60, 48, 16));
        tableChargeRecord = new QTableWidget(tab);
        tableChargeRecord->setObjectName(QStringLiteral("tableChargeRecord"));
        tableChargeRecord->setGeometry(QRect(0, 0, 651, 471));
        dateTime_Begin = new QDateTimeEdit(tab);
        dateTime_Begin->setObjectName(QStringLiteral("dateTime_Begin"));
        dateTime_Begin->setGeometry(QRect(655, 130, 121, 22));
        checkBox_1 = new QCheckBox(tab);
        checkBox_1->setObjectName(QStringLiteral("checkBox_1"));
        checkBox_1->setGeometry(QRect(760, 63, 71, 16));
        checkBox_2 = new QCheckBox(tab);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(730, 110, 71, 16));
        label_9 = new QLabel(tab);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(655, 110, 61, 16));
        dateTime_End = new QDateTimeEdit(tab);
        dateTime_End->setObjectName(QStringLiteral("dateTime_End"));
        dateTime_End->setGeometry(QRect(655, 190, 121, 22));
        checkBox_3 = new QCheckBox(tab);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setGeometry(QRect(730, 170, 71, 16));
        label_10 = new QLabel(tab);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(655, 170, 61, 16));
        tablePage->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        groupBox = new QGroupBox(tab_3);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 20, 421, 181));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 20, 61, 16));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 50, 81, 16));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(110, 20, 113, 20));
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(110, 50, 113, 20));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 80, 81, 16));
        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(110, 80, 113, 20));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(230, 20, 61, 16));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(230, 50, 61, 16));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(230, 80, 61, 16));
        btnSave_3 = new QPushButton(tab_3);
        btnSave_3->setObjectName(QStringLiteral("btnSave_3"));
        btnSave_3->setGeometry(QRect(680, 330, 81, 41));
        tablePage->addTab(tab_3, QString());
        chargingOptionClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(chargingOptionClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 790, 23));
        chargingOptionClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(chargingOptionClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        chargingOptionClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(chargingOptionClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        chargingOptionClass->setStatusBar(statusBar);

        retranslateUi(chargingOptionClass);

        tablePage->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(chargingOptionClass);
    } // setupUi

    void retranslateUi(QMainWindow *chargingOptionClass)
    {
        chargingOptionClass->setWindowTitle(QApplication::translate("chargingOptionClass", "chargingOption", 0));
        label->setText(QApplication::translate("chargingOptionClass", "\345\205\205\347\224\265\346\237\234\357\274\232", 0));
        btnRefresh_1->setText(QApplication::translate("chargingOptionClass", "\345\210\267\346\226\260", 0));
        btnDel_1->setText(QApplication::translate("chargingOptionClass", "\345\210\240\351\231\244", 0));
        btnSave_1->setText(QApplication::translate("chargingOptionClass", "\344\277\235\345\255\230", 0));
        btnAdd_1->setText(QApplication::translate("chargingOptionClass", "\345\242\236\345\212\240", 0));
        tablePage->setTabText(tablePage->indexOf(tab_1), QApplication::translate("chargingOptionClass", "\347\224\265\346\261\240\344\277\241\346\201\257", 0));
        btnDel_2->setText(QApplication::translate("chargingOptionClass", "\345\210\240\351\231\244", 0));
        btnAdd_2->setText(QApplication::translate("chargingOptionClass", "\345\242\236\345\212\240", 0));
        btnSave_2->setText(QApplication::translate("chargingOptionClass", "\344\277\235\345\255\230", 0));
        btnRefresh_2->setText(QApplication::translate("chargingOptionClass", "\345\210\267\346\226\260", 0));
        tablePage->setTabText(tablePage->indexOf(tab_2), QApplication::translate("chargingOptionClass", "\347\224\265\346\261\240\345\236\213\345\217\267", 0));
        btnQuery_1->setText(QApplication::translate("chargingOptionClass", "\346\237\245\350\257\242", 0));
        label_8->setText(QApplication::translate("chargingOptionClass", "\347\224\265\346\261\240\357\274\232", 0));
        checkBox_1->setText(QApplication::translate("chargingOptionClass", "\344\275\277\350\203\275", 0));
        checkBox_2->setText(QString());
        label_9->setText(QApplication::translate("chargingOptionClass", "\345\274\200\345\247\213\346\227\266\351\227\264\357\274\232", 0));
        checkBox_3->setText(QString());
        label_10->setText(QApplication::translate("chargingOptionClass", "\347\273\223\346\235\237\346\227\266\351\227\264\357\274\232", 0));
        tablePage->setTabText(tablePage->indexOf(tab), QApplication::translate("chargingOptionClass", "\345\205\205\347\224\265\350\256\260\345\275\225", 0));
        groupBox->setTitle(QApplication::translate("chargingOptionClass", "\350\256\276\347\275\256", 0));
        label_2->setText(QApplication::translate("chargingOptionClass", "\345\205\205\347\224\265\346\227\266\351\231\220\357\274\232", 0));
        label_3->setText(QApplication::translate("chargingOptionClass", "\347\275\221\347\273\234\345\221\250\346\234\237\357\274\232", 0));
        label_4->setText(QApplication::translate("chargingOptionClass", "\350\277\207\347\203\255\346\270\251\345\272\246\357\274\232", 0));
        label_5->setText(QApplication::translate("chargingOptionClass", "\345\210\206\351\222\237", 0));
        label_6->setText(QApplication::translate("chargingOptionClass", "\346\257\253\347\247\222/\346\254\241", 0));
        label_7->setText(QApplication::translate("chargingOptionClass", "\345\272\246", 0));
        btnSave_3->setText(QApplication::translate("chargingOptionClass", "\344\277\235\345\255\230", 0));
        tablePage->setTabText(tablePage->indexOf(tab_3), QApplication::translate("chargingOptionClass", "\350\275\257\344\273\266\350\256\276\347\275\256", 0));
    } // retranslateUi

};

namespace Ui {
    class chargingOptionClass: public Ui_chargingOptionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARGINGOPTION_H
