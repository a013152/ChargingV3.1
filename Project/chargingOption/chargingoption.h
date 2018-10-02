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


//ʹ��window��Ϣ ���н���ͨ��
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

	void initTabWidget(MAP_BATTERY &mapBattery, MAP_BATTERY_MODEL& mapBatteryModel);  //��ʼ��Tab�ؼ�

	void initComboBoxWidget();

	//��ʼ����
	void initConnectWidget();

	void resizeEvent(QResizeEvent* size);

	//���棨�����Ϣ��
	void OnBtnSave();

	//���棨����ͺ���Ϣ)
	void OnBtnSave2();
	//���� (�������)
	void OnBtnSave3();

	//��ѯ
	void OnBtnQuery_1();

	//��ӵ����Ϣ 
	void OnBtnAdd1();

	//��ӵ����Ϣ 
	void OnBtnAdd2();

	//ˢ�µ����Ϣ 
	void OnBtnRefresh1();

	void OnBtnRefresh2();
	//ɾ��ѡ�е�����Ϣ
	void OnBtnDel1();

	//ɾ��ѡ�е�����Ϣ
	void OnBtnDel2();

	//����ͨ��֪ͨ�����ܳ�籣���䡷�����������
	void SendChargingProgramToReadConfig();

	//�ر����
	void closeEvent(QCloseEvent *event);

	//�ռ�UI�����Ϣ ���浽 m_mapBattery
	bool collectUIInfoBattery();
	//�ռ�UI����ͺ���Ϣ ���浽 m_mapBatteryModel
	bool collectUIInfoBatteryModel();

	//��갴��	
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

	bool isChanged, isChanged2; //�޸Ĺ��ı�־������޸ĵ����������Ϊtrue�� ���������Ϊfalse
	//bool isChangedBatteryModel = false; //�޸Ĺ��ı�־������޸ĵ����������Ϊtrue�� ���������Ϊfalse

	int iError;  //������Ϣ

	MyQComboBox *m_ComboBoxDroneModel = NULL;
	MyQComboBox *m_ComboBoxBatteryModel = NULL;
	MyQComboBox *m_ComboBoxBalance = NULL;  //ƽ��
	QTableWidgetItem* m_pCurrentItemDrone = NULL;  //��ǰ�����˻��ͺ�
	QTableWidgetItem* m_pCurrentItemBattery = NULL; //��ǰ�ĵ���ͺ�
	QTableWidgetItem* m_pCurrentItemBalance = NULL; //��ǰ�ĵ��ƽ��

	 
};

#endif // CHARGINGOPTION_H
