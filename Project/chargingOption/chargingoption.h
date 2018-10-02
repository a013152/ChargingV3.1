#ifndef CHARGINGOPTION_H
#define CHARGINGOPTION_H

#include <QtWidgets/QMainWindow>

#include <QtCore/QString>
#include <QVBoxLayout> 
#include <QResizeEvent>
#include <QMessageBox>
#include <QtGui/QEnterEvent>

#include ".\GeneratedFiles\ui_chargingoption.h"
#include "operatorfile.h"
#include "ReadIniFile.h"
#include "common.h"
#include "typedef.h"
#include "MyQComboBox.h"
#include "operatoreDBFile.h"


//使用window消息 进行进程通信
#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

const ULONG_PTR CUSTOM_TYPE = 10000;
extern QString g_winTitle ;


extern char g_AppPath[256];
 
class QTabWidget;
class QVBoxLayout;
class CBatteryPage;
class CBatteryModelPage;


class chargingOption : public QMainWindow
{
	Q_OBJECT

public:
	chargingOption(QWidget *parent = 0);
	~chargingOption();

	void initWidget();

	void initTabWidget(MAP_BATTERY &mapBattery, MAP_BATTERY_MODEL& mapBatteryModel);  //初始化Tab控件

	void initComboBoxWidget();

	//初始化绑定
	void initConnectWidget();

	void resizeEvent(QResizeEvent* size);

	//保存（电池信息）
	void OnBtnSave();

	//保存（电池型号信息)
	void OnBtnSave2();
	//保存 (软件配置)
	void OnBtnSave3();

	//查询
	void OnBtnQuery_1();

	//添加电池信息 
	void OnBtnAdd1();

	//添加电池信息 
	void OnBtnAdd2();

	//刷新电池信息 
	void OnBtnRefresh1();

	void OnBtnRefresh2();
	//删除选中的行信息
	void OnBtnDel1();

	//删除选中的行信息
	void OnBtnDel2();

	//进程通信通知《智能充电保护箱》程序更新数据
	void SendChargingProgramToReadConfig();

	//关闭软件
	void closeEvent(QCloseEvent *event);

	//收集UI电池信息 保存到 m_mapBattery
	bool collectUIInfoBattery();
	//收集UI电池型号信息 保存到 m_mapBatteryModel
	bool collectUIInfoBatteryModel();

	//鼠标按下	
	void mousePressEvent(QMouseEvent *event){
		if (event->button() == Qt::LeftButton)
		{
			QPoint po = event->pos(); 
			if (m_ComboBoxDroneModel){
				if (m_ComboBoxDroneModel->geometry().contains(po) == false){
					m_ComboBoxDroneModel->setVisible(false);
				}
			}
			if (m_ComboBoxBatteryModel){
				if (m_ComboBoxBatteryModel->geometry().contains(po) == false){
					m_ComboBoxBatteryModel->setVisible(false);
				}
			}
		}
	}

public slots:
	void cellchangeBattery(int, int, int, int);
	void cellchangeBatteryModel(int, int, int, int);
	void cellChangedBattery(int, int);
	void cellChangedBatteryModel(int, int);
	void srocllBattery(int);
	void srocllBatteryModel(int);
	void textChangeBattery(const QString &);
	void textChangeDrone(const QString &);
	void textChangeBalance(const QString &);
	void textChangeCloset(const QString &);
	void textChangeScanTime(const QString &);
	void textChangeSubmitTime(const QString &);
private:
	Ui::chargingOptionClass ui;

	MAP_CLOSET m_mapCloset;  MAP_BATTERY_MODEL m_mapBatteryModel; MAP_BATTERY m_mapBattery; 
	MAP_CHARGER m_mapCharger;
	unsigned int m_SubmitInterval;
	unsigned int m_ChargeLimitTime;
	float m_fOverHeatTemperature;
	int m_iError = 0;
	
	QTabWidget *tabWidget; 
	QVBoxLayout *mainLayout; 
	COperatoreDBFile m_OperatoreDB;

	bool isChanged, isChanged2; //修改过的标志、如果修改电池内容设置为true， 保存后设置为false
	//bool isChangedBatteryModel = false; //修改过的标志、如果修改电池内容设置为true， 保存后设置为false

	int iError;  //错误信息

	MyQComboBox *m_ComboBoxDroneModel = NULL;
	MyQComboBox *m_ComboBoxBatteryModel = NULL;
	MyQComboBox *m_ComboBoxBalance = NULL;  //平衡
	QTableWidgetItem* m_pCurrentItemDrone = NULL;  //当前的无人机型号
	QTableWidgetItem* m_pCurrentItemBattery = NULL; //当前的电池型号
	QTableWidgetItem* m_pCurrentItemBalance = NULL; //当前的电池平衡

	 
};

#endif // CHARGINGOPTION_H
