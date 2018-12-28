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


//使用window消息 进行进程通信
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

//设置窗口置顶
void SetWindowTop(char * _szAppName);

//连接数据库线程类
class CConnectDBThread : public QThread
{
	Q_OBJECT
public:
	CConnectDBThread();
	
	void setMainProcess(charging* pthis){ m_pMainProcess = pthis; }

	void stop(){ stopped = true; }  //设置线程停止标志

	void doTestConnectDB(){ m_bTestConnectDB = true; } //测试连接数据库，是否通畅

	void doDetectServerDB(){ m_bDetectServerDB = true; } //连接数据库：提交电池数据，检测申请充电

signals:
	void connectDBResult(bool);  //连接数据库结果信号

protected:
	void run();

			
private:
	volatile bool stopped;

	bool m_bTestConnectDB = false;
	bool m_bDetectServerDB = false;
 

	charging * m_pMainProcess = NULL;
};


//隐藏调试信息UI线程类
class CThreadShowOrHideDebugInfo : public QThread
{
	Q_OBJECT
public:
	CThreadShowOrHideDebugInfo(){}
	void setBasePosition(QRect rect){
		baseRect = rect;
	}
	void setShowOrHide(bool flag2){ this->flag = flag2; }  //true 显示 false 隐藏

signals:
	void signalAdjustRect(QRect rect);  //移动位置

protected:
	bool flag = false;
	QRect baseRect;
	void run(){
		QRect rect = baseRect;
		for (int i = 1; i <= 10; i++)
		{
			if (flag){
				//显示 
				rect.setLeft(baseRect.left() - (baseRect.left() - 445) / 10 * i); 				
				}
			else
			{
				//隐藏
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

	//关闭软件
	void closeEvent(QCloseEvent *event);
	
	
	//初始化定时器
	void initTimer(); 
	

	//QTimer定时器，用于定时查询所有的电池信息
	QTimer * meTimer;
	//串口对象
	//MySerial my_Serial;

	//命令队列
	CCommandQueue m_CommandQueue;

	//连接数据库线程
	CConnectDBThread m_ConnectDBThread;

	CThreadShowOrHideDebugInfo m_ThreadHideOrShowDebugInfo;

	//开始扫描
	void beginScanBatteryState(bool reStart =false);
	//扫描所有柜子的电池数据
	void scanAllBatteryState();

	//扫描单柜子的电池数据，
	void scanOneBatteryState(unsigned int nClosetId, stCommand::enPriority enPriority = stCommand::normal);

	//增加扫描计数
	void addChargerScanTime();

	//检测判断需要数据库记录充电/停止充电
	void detectChargeRecord();

	//整合配置内容
	void combineConfig(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery, MAP_BATTERY_MODEL& mapBatteryModel, MAP_CHARGER& mapCharger, MAP_LEVEL& mapRelay);

	//拼装命令，加入发送队列
	void toSend(QString command, stCommand::enPriority enPriority = stCommand::normal);
	//去掉本地id前面的0
	QString no_zero(QString message);
	
	//获取返回的第n条信息
	QString get_back_message_at(QString message,int i = 0);	

	//显示各种数据
	void show_all_data_now();

	//显示一个电压
	void show_a_voltage(QString message, int i);
	void show_a_current(QString message, int i);
	void show_a_temperature(QString message, int i);
	void show_temperature(float ftemp, bool high  =false);
	void show_a_state(QString message, int indexArray, bool, QString chargerState , QString strVol); 

	bool nativeEvent(const QByteArray &eventType, void *message, long *result);//事件抓捕，处理进程通信
	 
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

	void OnReadyGetBatteryState(int);  //完成读取一个柜子编号 ，发送下一个柜子读取信息命令

	void OnAdjustDebugInfoRect(QRect rect); //调整调试信息区域的位置

signals:
	//退出事件
	void ExitApp();

	void RefreshState(enRefreshType type, int index);

	void AddCommamdIntoQueue(stCommand strCommand);

	void printfed(QString strPrintf, int nDebugLevel);

public:

	//初始化函数
	void init_now();

	//关联控件 
	void initConnectWidget(); 

	//读取配置
	void readConfig();
	
	QString packageCommand(QString command);//组装命令  调用例子:packageCommand("D,201,");

	//检测充电条件，返回true 表示能充, iResult返回不能充电的原因：
	bool detectChargingCondition(QString strBatteryId, int* iResult, bool showDebugInfo = true);
	//检测放电电条件，返回true 表示能充, iResult返回不能充电的原因：
	bool detectDisChargingCondition(QString strBatteryId, int* iResult, bool showDebugInfo = true);

	void detectServerBatteryState();    //检测服务器电池状态：申请充电或停止 本函数在线程中执行，不可操作UI

	void detectSubmitBatteryState();    //提交服务器电池状态: 电压、电流、温度 本函数在线程中执行，不可操作UI

	void setDischargeDay();		//设置自动放电天数

	void submitBatteryModel(MAP_BATTERY& mapNew, MAP_BATTERY& mapOld);  //提交电池型号

	void processApplyBatteryToCharging();  //处理申请电池 到 触发充电

	void appendChargingQueue(QString strBatteryId);  //添加到预充队列

	void removeChargingQueue(QString strBatteryId);  //取消待充电队列对应的id

	void writeApplyChargingQueueToFile();  //保存到文件

	void printfDebugInfo(QString strInfo, enDebugInfoPriority enLevel = DebugInfoLevelThree, bool bErrorInfo = false);

	void OnRefreshState(enRefreshType type, int index);

	bool getRelatedChargerOnline(QString strBatteryId);

	//窗口拖动
	void mousePressEvent(QMouseEvent * event);//按下事件
	void mouseMoveEvent(QMouseEvent * event);  //移动事件
	void mouseReleaseEvent(QMouseEvent * event); //松开按键
	//void paintEvent(QPaintEvent *);

	void extractNumber(QString strSrouce, int& iNumber);  //提取数字


	
private:
	bool m_LButtonPress = false; //按下标志
	QPoint m_MousePrePoint;  //鼠标位置
public slots :

	//改变柜子编号
	void OnClickClosetNo();

	void OnClickPrePage();

	void OnClickNextPage();

	
	//串口菜单点击事件
	void OnClickMenuCom(QAction * action);
	//打开配置窗口
	void onOpenOptionWin();
	//重新打开串口
	void OnClickMenuDebugInfoLeve(QAction * action);

	//打开数据窗口（阻塞)
	void onOpenDataDialog();


	//定时器超时
	void timer_out();

	//接收到的串口的内容param 1 命令类型，param 2 内容； 3错误码
	void readSerial(QString type, QString strContent, int iError);

	//解析接收到的CAN的内容param 1 内容。
	void onReadCAN(QString strContent);

	
	//接收到充电
	void OnBtnChargingOrStopCharging1();
	//接收到放电
	void OnBtnDisChargingOrStop1();

	//层级按钮： 切换层级
	void OnBtnLevel();


	void updateTextEdit(QString, int nDebugLevel);
	
	void detectTextEdit();  //测检textEdit 文本是否过长

	void detectHideDebugInfo();  //测检隐藏调试信息

	bool detectRecvXOR(QString strContent);  //测检收到的数据XOR

	//获取电池id 关联信息
	bool getBatteryIdRelatedInfo(QString strBatteryID, MAP_CLOSET_IT& itCloset, MAP_BATTERY_IT& itBattery, MAP_BATTERY_MODEL_IT& itBatteryModel, MAP_CHARGER_IT& itCharger, MAP_LEVEL_IT& itLevel);

	int getCanDJIBattery(int CANID, int pos);

	int chargerIDtoBatteryId(int chargerId);//充电器ID 查找对应电池ID

	int batteryIDtoArrayIndex(QString strID ); //转换 电池id对应 数组ID

	void onRefreshUI(QString strID);  //刷新相应 
	
	void releaseBatteryStateRefresh(); //刷新解锁 

	bool chargingByLocalID(QString strId, int *iResult , bool showDebugInfo = true); //根据电池ID，拼装充电命令，插入命令队列

	bool stopByLocalID(QString strId); //根据电池ID，拼装停止命令，插入命令队列
	
	void createClosetRadio();

	QAction * createMenus();//创建菜单 
	
	QAction * serial_scan(QAction *);//扫描串口创建菜单

	void createChargGrid(); //创建充电UI格+ 充电器层级

	void createDebugInfoUI(); //创建调试信息界面区域

	void adjustUI(); //调整ui： 设置位置、隐藏控件、设置图片等

	QString getBatteryState(int indexBattery); //获取电池的状态

	void showTipsMessagebox(int iType  , QString strMsg); //显示提示框 0提示、1警告、2错误 

	void refreshCurrentUI();

	void updateListviewBatteryModel(int indexMem = -1);  //更新详细电池数据

	void onOpenOrCloseCanDevice(bool bOpenOrClose);

	void onSetDebugInfo(QString strInfo);  //覆盖原有调试信息
public slots:
	void allClosetCharging();
	void oneClosetCharging();
	void onPauseScand(bool checked);
	void onPauseSubmit(bool checked);
	void onBtnCanDevice(bool checked);
	void onOneKeyCharger();
	
signals:
	void refreshUI(QString strID);  //刷新信号 ： 参数电池ID 
	void setDebugInfo(QString);  //定时器清理调试信息

	
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
	QVector<bool>battery_state_enable_refresh;  //能否刷新充电器状态
	QVector<bool>battery_charging_record; //充电记录记录， 当下位机返回充电成功后，设置true， 提交到数据库后设置false


	QVector<bool>battery_apply;// 电池申请情况 定时器里msg_get_now初始化
	QVector<QString> battery_server_state;  //服务器中的电池状态，目前用于取消充电 add 20180524
	QVector<stApplyInfo> battery_apply_charging;  //充电情况
	QVector<stApplyInfo> battery_cancel_charging_info;  //取消充电情况，临时用于执行停止充电后提交服务器 add20180524
	QVector<stApplyBatteryDontCharge> m_vtApplyDontCharge;  //申请还没充电的记录
	QVector<stApplyBatteryDontCharge> m_vtApplyDontChargeB;  //申请还没充电的记录B 用于比较

	data_send m_submitServer;  //提交服务器的类对象

	QMenu* m_menuSys;	//系统菜单
	QMenu* m_menuDebugInfoLevel;	//调试信息等级
	QMenu* m_menuCom; //串口菜单

	QAction * m_menuItemCan = nullptr; //can 设备 菜单项目

	CMessageBoxCus* m_msgDlg = nullptr; //消息提示框
	CDataDialog* m_dataDlg = nullptr; //电池数据框

	

	unsigned int m_count_recv = 0; //收到的电池计数
	 
	bool isOpenSerialPort;// 是否打开串口设备

	bool isOpenCANProcess;// 是否打开进程

	bool m_bContinueScan;// 是否继续扫描

	bool m_bContinueSubmit;//是否继续提交网络

	int iError;  //错误信息

	enDebugInfoPriority m_enShowDebugInfoLevel;//显示等级

	unsigned int m_SubmitInterval;//提交时间间隔

	unsigned int m_ScanDeviceInterval;//扫描设备时间间隔

	int m_nChargeLimitTime;  //充电时限 分钟为单位，add20180521

	bool m_bConnectServerIsSeccuss = false;//连接服务器数据库成功标志 

	float m_fOverHeatTemperature = DEFAUT_OVERHEATTEMPERATURE; //过热温度默认45

	int m_nDischargeDay = DISCHARGING_DAY; //自动放电天数


	unsigned int m_nCurrentLevel = 1; //当前层级
private:
	//读取的文件配置
	MAP_CLOSET m_mapCloset;  MAP_BATTERY_MODEL m_mapBatteryModel; MAP_BATTERY m_mapBattery;
	MAP_CHARGER m_mapCharger;  MAP_LEVEL m_mapLevel;
	int m_iError = 0;
	 
	//命令队列
	


	//UI
private:
	int m_iCurrentCloset = 1;  //当前的柜子编号  1~10
	int m_iCurrentClosetPage = 1;  //当前的柜子页  用于翻页判断
	QVector<QPushButton*> m_vtUiClosetRadio; //界面中的充电柜编号容器

	QGridLayout* m_gridLayout;  //格子布局
	QGroupBox* m_groupBox;		//格子组
	QVector< ui_charg_grid*> m_vtUiChargGrid;	//电池格子
	QVector<QPushButton*> m_vtUiLevelBtn; //层级ui按钮（导航栏） //add 20180920

	QGroupBox* m_GroupBox_DebugInfo = nullptr;	 //调试信息显示组
	QLabel* m_Label_DebugInfo = nullptr;	 //调试信息显示
	
	QCheckBox *m_CheckBox_DebugInfo = nullptr;   //调试信息自动隐藏
	QPushButton* m_BtnMin_DebugInfo = nullptr;   //调试信息隐藏按钮
	bool m_bAutoHideDebugInfo = false;	//自动隐藏
	bool m_bShowDebugInfo = false;		//显示调试信息区域
	bool m_bCanMoveWindow = true;		//允许移动窗口

	QTabWidget *tablePage_DebugInfo = nullptr;  //调试信息分页
	QWidget *tab_1 = nullptr;			//第一页
	QTextEdit *m_TextEdit_DebugInfo = nullptr;   //通讯信息信息文本框
	QWidget *tab_2 = nullptr;			//第二页
	 
	QListView * m_listview_Battery;			//UI显示部分电池： 温度、电压、电流
	QStringListModel* m_listview_Battery_model = nullptr;	//数据部分 电池： 温度、电压、电流
	 

private:
		QProcess *m_pProcess = NULL;
		COperatoreDBFile m_OperDB; // 操作数据库文件，记录充电/停止充电记录
};
class levelBtnData: public QObjectUserData{
public:
	unsigned int nlevel;
	~levelBtnData(){}
	levelBtnData(){ nlevel = 0; }
};
#endif // CHARGING_H
