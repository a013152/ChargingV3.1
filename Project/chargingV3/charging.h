#ifndef CHARGING_H
#define CHARGING_H

#include <QtWidgets/QMainWindow>
#include "ui_charging.h"
#include <qdebug.h>
#include <qtimer.h>
#include <QThread>
#include <iostream>
#include <QTime>
#include <QCloseEvent>
#include <QQueue>
#include <QGridLayout>
#include <QMessageBox>
#include <QProcess>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QListView>
#include <QStringListModel>

#include "ui_charg_grid.h" 
#include "operatoreDBFile.h"

//#include "MySerial.h"

//
#include "typedef.h"
#include "common.h"
#include "errorCode.h"
#include "operatorfile.h"
#include "CommandQueue.h"
#include "data_send.h"
#include "ReadIniFile.h"
#include "MessageBoxCus.h"
#include "dataDialog.h"



#define GET_TIME (QDateTime::currentDateTime()).toString("hh:mm:ss ")
#define C_QSTR(szBuf) QString::fromLocal8Bit(szBuf)
#define DEBUG_LOG(str){char szBuf[256]={0};LOG3(szBuf);COperatorFile::GetInstance()->writeDebugLog(GET_TIME+C_QSTR(szBuf)+str);}


//ʹ��window��Ϣ ���н���ͨ��
#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif
#include "SerialPort.h" 
//#pragma comment(lib, "SerialPortDll.lib")
#define SERIAL_PORT CSerialPort::GetInstance()
const ULONG_PTR CUSTOM_TYPE = 10000;

extern QString g_winTitle;

extern char g_AppPath[256];



class charging;

//���ô����ö�
void SetWindowTop(char * _szAppName);

//�������ݿ��߳���
class CConnectDBThread : public QThread
{
	Q_OBJECT
public:
	CConnectDBThread();
	
	void setMainProcess(charging* pthis){ m_pMainProcess = pthis; }

	void stop(){ stopped = true; }  //�����߳�ֹͣ��־

	void doTestConnectDB(){ m_bTestConnectDB = true; } //�����������ݿ⣬�Ƿ�ͨ��

	void doDetectServerDB(){ m_bDetectServerDB = true; } //�������ݿ⣺�ύ������ݣ����������

signals:
	void connectDBResult(bool);  //�������ݿ����ź�

protected:
	void run();

			
private:
	volatile bool stopped;

	bool m_bTestConnectDB = false;
	bool m_bDetectServerDB = false;
 

	charging * m_pMainProcess = NULL;
};


//���ص�����ϢUI�߳���
class CThreadShowOrHideDebugInfo : public QThread
{
	Q_OBJECT
public:
	CThreadShowOrHideDebugInfo(){}
	void setBasePosition(QRect rect){
		baseRect = rect;
	}
	void setShowOrHide(bool flag2){ this->flag = flag2; }  //true ��ʾ false ����

signals:
	void signalAdjustRect(QRect rect);  //�ƶ�λ��

protected:
	bool flag = false;
	QRect baseRect;
	void run(){
		QRect rect = baseRect;
		for (int i = 1; i <= 10; i++)
		{
			if (flag){
				//��ʾ 
				rect.setLeft(baseRect.left() - (baseRect.left() - 445) / 10 * i); 				
				}
			else
			{
				//����
				rect.setLeft((800 - baseRect.left()) / 10 * i + (baseRect.left()));
			}
			emit signalAdjustRect(rect);
			Sleep(20);
		}  
	}
	 
};
  

class charging : public QMainWindow
{
	Q_OBJECT

public:
	charging(QWidget *parent = 0);
	~charging();

private:
	Ui::chargingClass ui;

	//�ر����
	void closeEvent(QCloseEvent *event);
	
	
	//��ʼ����ʱ��
	void initTimer(); 
	

	//QTimer��ʱ�������ڶ�ʱ��ѯ���еĵ����Ϣ
	QTimer * meTimer;
	//���ڶ���
	//MySerial my_Serial;

	//�������
	CCommandQueue m_CommandQueue;

	//�������ݿ��߳�
	CConnectDBThread m_ConnectDBThread;

	CThreadShowOrHideDebugInfo m_ThreadHideOrShowDebugInfo;

	//��ʼɨ��
	void beginScanBatteryState(bool reStart =false);
	//ɨ�����й��ӵĵ������
	void scanAllBatteryState();

	//ɨ�赥���ӵĵ�����ݣ�
	void scanOneBatteryState(unsigned int nClosetId, stCommand::enPriority enPriority = stCommand::normal);

	//����ɨ�����
	void addChargerScanTime();

	//����ж���Ҫ���ݿ��¼���/ֹͣ���
	void detectChargeRecord();

	//������������
	void combineConfig(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery, MAP_BATTERY_MODEL& mapBatteryModel, MAP_CHARGER& mapCharger, MAP_LEVEL& mapRelay);

	//ƴװ������뷢�Ͷ���
	void toSend(QString command, stCommand::enPriority enPriority = stCommand::normal);
	//ȥ������idǰ���0
	QString no_zero(QString message);
	
	//��ȡ���صĵ�n����Ϣ
	QString get_back_message_at(QString message,int i = 0);	

	//��ʾ��������
	void show_all_data_now();

	//��ʾһ����ѹ
	void show_a_voltage(QString message, int i);
	void show_a_current(QString message, int i);
	void show_a_temperature(QString message, int i);
	void show_temperature(float ftemp, bool high  =false);
	void show_a_state(QString message, int indexArray, bool, QString chargerState , QString strVol); 

	bool nativeEvent(const QByteArray &eventType, void *message, long *result);//�¼�ץ�����������ͨ��
	 
	void OnConnectDBResult(bool);
	void OnBtnShowSysMenu();
	void OnBtnShowComMenu();
	void OnBtnSysMin();
	void OnExitApp();
public slots:
	void OnBtnSysClose();

	void OnBtnHideDebugInfo();
	void OnBtnShowDebugInfo();
	void OnCheckAutoShowDebugInfo(int nState);

	void OnAddCommamdIntoQueue(stCommand strCommand);

	void OnReadyGetBatteryState(int);  //��ɶ�ȡһ�����ӱ�� ��������һ�����Ӷ�ȡ��Ϣ����

	void OnAdjustDebugInfoRect(QRect rect); //����������Ϣ�����λ��

signals:
	//�˳��¼�
	void ExitApp();

	void RefreshState(enRefreshType type, int index);

	void AddCommamdIntoQueue(stCommand strCommand);

	void printfed(QString strPrintf, int nDebugLevel);

public:

	//��ʼ������
	void init_now();

	//�����ؼ� 
	void initConnectWidget(); 

	//��ȡ����
	void readConfig();
	
	QString packageCommand(QString command);//��װ����  ��������:packageCommand("D,201,");

	//���������������true ��ʾ�ܳ�, iResult���ز��ܳ���ԭ��
	bool detectChargingCondition(QString strBatteryId, int* iResult, bool showDebugInfo = true);
	//���ŵ������������true ��ʾ�ܳ�, iResult���ز��ܳ���ԭ��
	bool detectDisChargingCondition(QString strBatteryId, int* iResult, bool showDebugInfo = true);

	void detectServerBatteryState();    //�����������״̬���������ֹͣ ���������߳���ִ�У����ɲ���UI

	void detectSubmitBatteryState();    //�ύ���������״̬: ��ѹ���������¶� ���������߳���ִ�У����ɲ���UI

	void setDischargeDay();		//�����Զ��ŵ�����

	void submitBatteryModel(MAP_BATTERY& mapNew, MAP_BATTERY& mapOld);  //�ύ����ͺ�

	void processApplyBatteryToCharging();  //���������� �� �������

	void appendChargingQueue(QString strBatteryId);  //��ӵ�Ԥ�����

	void removeChargingQueue(QString strBatteryId);  //ȡ���������ж�Ӧ��id

	void writeApplyChargingQueueToFile();  //���浽�ļ�

	void printfDebugInfo(QString strInfo, enDebugInfoPriority enLevel = DebugInfoLevelThree, bool bErrorInfo = false);

	void OnRefreshState(enRefreshType type, int index);

	bool getRelatedChargerOnline(QString strBatteryId);

	//�����϶�
	void mousePressEvent(QMouseEvent * event);//�����¼�
	void mouseMoveEvent(QMouseEvent * event);  //�ƶ��¼�
	void mouseReleaseEvent(QMouseEvent * event); //�ɿ�����
	//void paintEvent(QPaintEvent *);

	void extractNumber(QString strSrouce, int& iNumber);  //��ȡ����


	
private:
	bool m_LButtonPress = false; //���±�־
	QPoint m_MousePrePoint;  //���λ��
public slots :

	//�ı���ӱ��
	void OnClickClosetNo();

	void OnClickPrePage();

	void OnClickNextPage();

	
	//���ڲ˵�����¼�
	void OnClickMenuCom(QAction * action);
	//�����ô���
	void onOpenOptionWin();
	//���´򿪴���
	void OnClickMenuDebugInfoLeve(QAction * action);

	//�����ݴ��ڣ�����)
	void onOpenDataDialog();


	//��ʱ����ʱ
	void timer_out();

	//���յ��Ĵ��ڵ�����param 1 �������ͣ�param 2 ���ݣ� 3������
	void readSerial(QString type, QString strContent, int iError);

	//�������յ���CAN������param 1 ���ݡ�
	void onReadCAN(QString strContent);

	
	//���յ����
	void OnBtnChargingOrStopCharging1();
	//���յ��ŵ�
	void OnBtnDisChargingOrStop1();

	//�㼶��ť�� �л��㼶
	void OnBtnLevel();


	void updateTextEdit(QString, int nDebugLevel);
	
	void detectTextEdit();  //���textEdit �ı��Ƿ����

	void detectHideDebugInfo();  //������ص�����Ϣ

	bool detectRecvXOR(QString strContent);  //����յ�������XOR

	//��ȡ���id ������Ϣ
	bool getBatteryIdRelatedInfo(QString strBatteryID, MAP_CLOSET_IT& itCloset, MAP_BATTERY_IT& itBattery, MAP_BATTERY_MODEL_IT& itBatteryModel, MAP_CHARGER_IT& itCharger, MAP_LEVEL_IT& itLevel);

	int getCanDJIBattery(int CANID, int pos);

	int chargerIDtoBatteryId(int chargerId);//�����ID ���Ҷ�Ӧ���ID

	int batteryIDtoArrayIndex(QString strID ); //ת�� ���id��Ӧ ����ID

	void onRefreshUI(QString strID);  //ˢ����Ӧ 
	
	void releaseBatteryStateRefresh(); //ˢ�½��� 

	bool chargingByLocalID(QString strId, int *iResult , bool showDebugInfo = true); //���ݵ��ID��ƴװ�����������������

	bool stopByLocalID(QString strId); //���ݵ��ID��ƴװֹͣ��������������
	
	void createClosetRadio();

	QAction * createMenus();//�����˵� 
	
	QAction * serial_scan(QAction *);//ɨ�贮�ڴ����˵�

	void createChargGrid(); //�������UI��+ ������㼶

	void createDebugInfoUI(); //����������Ϣ��������

	void adjustUI(); //����ui�� ����λ�á����ؿؼ�������ͼƬ��

	QString getBatteryState(int indexBattery); //��ȡ��ص�״̬

	void showTipsMessagebox(int iType  , QString strMsg); //��ʾ��ʾ�� 0��ʾ��1���桢2���� 

	void refreshCurrentUI();

	void updateListviewBatteryModel(int indexMem = -1);  //������ϸ�������

	void onOpenOrCloseCanDevice(bool bOpenOrClose);

	void onSetDebugInfo(QString strInfo);  //����ԭ�е�����Ϣ
public slots:
	void allClosetCharging();
	void oneClosetCharging();
	void onPauseScand(bool checked);
	void onPauseSubmit(bool checked);
	void onBtnCanDevice(bool checked);
	void onOneKeyCharger();
	
signals:
	void refreshUI(QString strID);  //ˢ���ź� �� �������ID 
	void setDebugInfo(QString);  //��ʱ�����������Ϣ

	
private:
	QVector<QString>battery_my_id;
	QVector<QString>battery_local_id;
	QVector<QString>battery_type;
	QVector<QString>battery_state;
	QVector<QString>charger_state;
	QVector<QString>battery_temperature;
	QVector<QString>battery_voltage;
	QVector<QString>battery_current;
	QVector<bool>battery_balance;
	QVector<bool>battery_state_enable_refresh;  //�ܷ�ˢ�³����״̬
	QVector<bool>battery_charging_record; //����¼��¼�� ����λ�����س��ɹ�������true�� �ύ�����ݿ������false


	QVector<bool>battery_apply;// ���������� ��ʱ����msg_get_now��ʼ��
	QVector<QString> battery_server_state;  //�������еĵ��״̬��Ŀǰ����ȡ����� add 20180524
	QVector<stApplyInfo> battery_apply_charging;  //������
	QVector<stApplyInfo> battery_cancel_charging_info;  //ȡ������������ʱ����ִ��ֹͣ�����ύ������ add20180524
	QVector<stApplyBatteryDontCharge> m_vtApplyDontCharge;  //���뻹û���ļ�¼
	QVector<stApplyBatteryDontCharge> m_vtApplyDontChargeB;  //���뻹û���ļ�¼B ���ڱȽ�

	data_send m_submitServer;  //�ύ�������������

	QMenu* m_menuSys;	//ϵͳ�˵�
	QMenu* m_menuDebugInfoLevel;	//������Ϣ�ȼ�
	QMenu* m_menuCom; //���ڲ˵�

	QAction * m_menuItemCan = nullptr; //can �豸 �˵���Ŀ

	CMessageBoxCus* m_msgDlg = nullptr; //��Ϣ��ʾ��
	CDataDialog* m_dataDlg = nullptr; //������ݿ�

	

	unsigned int m_count_recv = 0; //�յ��ĵ�ؼ���
	 
	bool isOpenSerialPort;// �Ƿ�򿪴����豸

	bool isOpenCANProcess;// �Ƿ�򿪽���

	bool m_bContinueScan;// �Ƿ����ɨ��

	bool m_bContinueSubmit;//�Ƿ�����ύ����

	int iError;  //������Ϣ

	enDebugInfoPriority m_enShowDebugInfoLevel;//��ʾ�ȼ�

	unsigned int m_SubmitInterval;//�ύʱ����

	unsigned int m_ScanDeviceInterval;//ɨ���豸ʱ����

	int m_nChargeLimitTime;  //���ʱ�� ����Ϊ��λ��add20180521

	bool m_bConnectServerIsSeccuss = false;//���ӷ��������ݿ�ɹ���־ 

	float m_fOverHeatTemperature = DEFAUT_OVERHEATTEMPERATURE; //�����¶�Ĭ��45

	int m_nDischargeDay = DISCHARGING_DAY; //�Զ��ŵ�����


	unsigned int m_nCurrentLevel = 1; //��ǰ�㼶
private:
	//��ȡ���ļ�����
	MAP_CLOSET m_mapCloset;  MAP_BATTERY_MODEL m_mapBatteryModel; MAP_BATTERY m_mapBattery;
	MAP_CHARGER m_mapCharger;  MAP_LEVEL m_mapLevel;
	int m_iError = 0;
	 
	//�������
	


	//UI
private:
	int m_iCurrentCloset = 1;  //��ǰ�Ĺ��ӱ��  1~10
	int m_iCurrentClosetPage = 1;  //��ǰ�Ĺ���ҳ  ���ڷ�ҳ�ж�
	QVector<QPushButton*> m_vtUiClosetRadio; //�����еĳ���������

	QGridLayout* m_gridLayout;  //���Ӳ���
	QGroupBox* m_groupBox;		//������
	QVector< ui_charg_grid*> m_vtUiChargGrid;	//��ظ���
	QVector<QPushButton*> m_vtUiLevelBtn; //�㼶ui��ť���������� //add 20180920

	QGroupBox* m_GroupBox_DebugInfo = nullptr;	 //������Ϣ��ʾ��
	QLabel* m_Label_DebugInfo = nullptr;	 //������Ϣ��ʾ
	
	QCheckBox *m_CheckBox_DebugInfo = nullptr;   //������Ϣ�Զ�����
	QPushButton* m_BtnMin_DebugInfo = nullptr;   //������Ϣ���ذ�ť
	bool m_bAutoHideDebugInfo = false;	//�Զ�����
	bool m_bShowDebugInfo = false;		//��ʾ������Ϣ����
	bool m_bCanMoveWindow = true;		//�����ƶ�����

	QTabWidget *tablePage_DebugInfo = nullptr;  //������Ϣ��ҳ
	QWidget *tab_1 = nullptr;			//��һҳ
	QTextEdit *m_TextEdit_DebugInfo = nullptr;   //ͨѶ��Ϣ��Ϣ�ı���
	QWidget *tab_2 = nullptr;			//�ڶ�ҳ
	 
	QListView * m_listview_Battery;			//UI��ʾ���ֵ�أ� �¶ȡ���ѹ������
	QStringListModel* m_listview_Battery_model = nullptr;	//���ݲ��� ��أ� �¶ȡ���ѹ������
	 

private:
		QProcess *m_pProcess = NULL;
		COperatoreDBFile m_OperDB; // �������ݿ��ļ�����¼���/ֹͣ����¼
};
class levelBtnData: public QObjectUserData{
public:
	unsigned int nlevel;
	~levelBtnData(){}
	levelBtnData(){ nlevel = 0; }
};
#endif // CHARGING_H
