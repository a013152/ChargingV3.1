#pragma  once
/*
声明结构体
本文件命名规则 st+大写首字母 例如：stBatteryInfo 电池信息

	@version： 1.1
	@author：chenlin
	@date：20180122

	修订
	1.0  创建电池型号、电池信息结构体
	1.1	 添加命令结构体
	1.2  修改电池结构 添加关联充电器、关联继电器。
*/

#include <string.h>
#include <map>
#include <stdlib.h>
#include<cmath>
#include <QString>
#include <QTime>
#include <QDateTime>
#include "common.h"

//电池型号信息
struct stBatteryModelInfo
{
	unsigned int id;		//编号
	char droneModel[128];   //无人机型号	
	unsigned int capacity;  //容量	
	float chargingVoltage;//充电电压
	float chargingCurrent; //充电电流
	float storageVoltage; //储存电压
	float fullVoltage;		//满载电压	
	char connectType[20]; //链接种类？4S 6S
	bool balance;   //是否平衡（非智能电池false 智能电池true)
	

	stBatteryModelInfo() : id(0), capacity(0), chargingVoltage(0.0),\
		chargingCurrent(0.0), storageVoltage(0.0), \
		fullVoltage(0.0), balance(0.0) 
	{
			memset(droneModel, 0, 128);			
			memset(connectType, 0, 128);
	}
};


//充电记录结构体
struct stChargeRecord{
	bool beginChargeFlag; //开始充电记录标志， true表示要写入数据库，写入完成赋值false
	bool endChargeFlag;  //结束充电记录标志
	bool pendingEndFlag;  //需要判断结束充电的标志

	
	unsigned int batteryId; //电池id， 读取时用 
	QString beginTime;	//起始时间
	QString endTime;		//结束时间，读取时用
	QString pendingTime;	//充电过程时间，读取时用
	float beginVol;			//起始电压，读取时用
	float endVol;			//结束电压，读取时用
	QString strRemrk;  //备注 ：手动充电/远程申请充电+手动结束+远程结束

	stChargeRecord() :beginChargeFlag(false), endChargeFlag(false), pendingEndFlag(false), batteryId(0),beginVol(0), endVol(0){

	}
	stChargeRecord(const stChargeRecord& other)   //拷贝构造函数   
	{
		this->beginChargeFlag = other.beginChargeFlag;
		this->endChargeFlag = other.endChargeFlag;
		this->pendingEndFlag = other.pendingEndFlag;
		this->batteryId = other.batteryId;
		this->beginTime = other.beginTime;
		this->endTime = other.endTime;
		this->strRemrk = other.strRemrk;
		this->beginVol = other.beginVol;
		this->endVol = other.endVol;
		this->pendingTime = other.pendingTime;
	}
	stChargeRecord& operator=(const stChargeRecord& other)   //拷贝构造函数   
	{
		this->beginChargeFlag = other.beginChargeFlag;
		this->endChargeFlag = other.endChargeFlag;
		this->pendingEndFlag = other.pendingEndFlag;
		this->batteryId = other.batteryId;
		this->beginTime = other.beginTime;
		this->endTime = other.endTime;
		this->beginVol = other.beginVol;
		this->endVol = other.endVol;
		this->pendingTime = other.pendingTime;
		return *this;
	}
};

//电池信息
struct  stBatteryInfo
{
	unsigned int dbid;		  //数据库对应id
	char id[5];					//电池id	
	unsigned int modelId;     //电池型号Id	
	bool online;		      //是否使用  //false 的话跳过处理逻辑
	char user[256];				//申请使用者
	unsigned int relatedCharger;  //关联充电器 地址
	int state;   //状态 （针对DJI电池 的充电状态）电池状态	0x00 满电	0x01 充电中		0x02 放电中		0x03 静默
	int nlevel;   //所在层级
	//unsigned int relatedRelay;  //关联继电器控制板 地址
	//char relatedLoop;  //关联回路  
	bool isExisted;				//是否存在
	bool isChanged;				//是否被更换过电池//针对扫描间隔期间有电池更换现象
	bool isApplyCharging;       //是否需要预充（针对同组的电池正在充电，）
	QTime timeLockCloseLoop;	//禁止断开回路的计时器 
	QTime timeLockUI;   //禁止刷新对应ui的计时器
	QTime timeLockChargeRecord;   //禁止刷新充电记录标志计时器
	stChargeRecord stRecord;
	stBatteryInfo() : dbid(0), online(1), modelId(0), state(0),\
		relatedCharger(0), isExisted(false), isChanged(false), isApplyCharging(false), nlevel(0){
		memset(id, 0, 10);	
		memset(user, 0, 256);
		timeLockCloseLoop.start();
		timeLockUI.start();
		timeLockChargeRecord.start();
		//memset(gridNO, 0, 4);
	}
	stBatteryInfo(const stBatteryInfo& other)   //拷贝构造函数   
	{
		this->dbid = other.dbid;
		this->modelId = other.modelId;
		this->relatedCharger = other.relatedCharger;
		this->state = other.state; 
		this->online = other.online;
		//this->relatedLoop = other.relatedLoop;
		this->isExisted = other.isExisted;
		this->isChanged = other.isChanged;
		this->isApplyCharging = other.isApplyCharging;
		strcpy(this->id, other.id);
		strcpy(this->user, other.user);
		this->timeLockCloseLoop = other.timeLockCloseLoop;
		this->timeLockUI = other.timeLockUI;
		this->timeLockChargeRecord = other.timeLockChargeRecord;
		this->stRecord = other.stRecord;
		this->nlevel = other.nlevel;
	}
};
//充电器类型 add 20180920
enum enChargerType
{
	DJI_Charger = 0,   //大疆充电槽
	NF_Charger = 1		//能飞充电板
};

//充电器信息
struct stCharger
{
	unsigned int id;  //充电器编号
	unsigned int nLevel;	//所属层级
	enChargerType chargerType;  //充电器类型
	//std::map<unsigned int, stBatteryInfo> mapBattery;//电池列表（映射）1个充电器最多带三个电池
	QTime timeLockChargingState;   //2秒内禁止刷新充电器状态
	QString beginChergeTime;       //充电开始时间 add20180521
	bool isChargingPre;
	bool isCharging;  //正在充电标志
	bool isDisCharging;  //正在放电标志
	bool bOnline;  //充电器在线状态
	bool isOverHeat;  //是否过热
	char nSeekT;  //温度数组浮标
	char nSeekV;  //电压数组浮标
	float fTemperature[3];  //温度
	float fVoltage[3];			//电压
	float fCurrent;			//电流
	//bool bNeedReadVol;  //需要读取电压标志
	unsigned int nScanWatchDog;  //在线状态看门狗，用于判断不在线
	stCharger() :id(0), nLevel(0), chargerType(NF_Charger), isChargingPre(false), isCharging(false), bOnline(false),
		isDisCharging(false), /*bNeedReadVol(false),*/ nScanWatchDog(0), 
		isOverHeat(false), nSeekT(0), nSeekV(0), fCurrent(0)
	{
		for (int i = 0; i < 3; i++){
			fTemperature[i] = 0; fVoltage[i] = 0;
		}
		timeLockChargingState.start();
		beginChergeTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
	}
	stCharger(const stCharger& other)   //拷贝构造函数   
	{
		this->id = other.id;
		this->nLevel = other.nLevel;
		this->chargerType = other.chargerType;
		this->isChargingPre = other.isChargingPre;
		this->isCharging = other.isCharging;
		this->isDisCharging = other.isDisCharging;
		this->bOnline = other.bOnline;
		//this->bNeedReadVol = other.bNeedReadVol;
		this->nScanWatchDog = other.nScanWatchDog; 
		this->timeLockChargingState = other.timeLockChargingState;
		this->beginChergeTime = other.beginChergeTime;
		this->isOverHeat = other.isOverHeat;
		for (int i = 0; i < 3; i++){
			this->fTemperature[i] = other.fTemperature[i];
			this->fVoltage[i] = other.fVoltage[i];
		}
		this->nSeekT = other.nSeekT;
		this->nSeekV = other.nSeekV;
		this->fCurrent = other.fCurrent;
	}
	float getAverage(float* arrayFloat){ //获取平均
		float  averageTemp = 0; char count_ = 0;
		for (int i = 0; i < 3; i++){
			if (arrayFloat[i] != 0){
				count_++; averageTemp += arrayFloat[i];
			}
		}
		if(count_!=0)
			averageTemp = averageTemp / count_; 
		return averageTemp;
	}
	float saveTemperature(float fTemp){
		float  averageTemp = getAverage(fTemperature);
		if (fabs(fTemp - averageTemp) < 50)//差值不超过50度，视为有效
		{
			if (nSeekT < 3){
				fTemperature[nSeekT++] = fTemp;
				if (nSeekT == 3)
					nSeekT = 0;
			}
			averageTemp = getAverage(fTemperature);
		} 
		return averageTemp; 
	}
	float saveVoltage(float fVol){
		float  averageTemp = getAverage(fVoltage); 
		if (fVol >= 3.3 && fVol <= 4.4){
			if (averageTemp  == 0
				|| (averageTemp != 0 && fabs(fVol - averageTemp) < 0.9))//差值不超过0.9V，视为有效
			{
				if (nSeekV < 3){
					fVoltage[nSeekV++] = fVol;
					if (nSeekV == 3)
						nSeekV = 0;
				}
				averageTemp = getAverage(fVoltage);
			}
		}
		return averageTemp;
	}

	void updateBeginChargeTime(){  //更新充电时间
		beginChergeTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
	}
	int calculateChargeTime()
	{
		if (isChargingPre == false && isCharging == true){
			updateBeginChargeTime(); //开始充电 记录开始充电时间。
		}
		isChargingPre = isCharging;
		static QDate dateCurrent, dateBeginCharge;
		static QTime timeCurrent, timeBeginCharge;
		static int beginYear, beginMonth, beginDay, beginHour, beginMinite;
		int nInterval; nInterval = 0;
		dateCurrent = QDateTime::currentDateTime().date();
		timeCurrent = QDateTime::currentDateTime().time();
		beginYear = beginChergeTime.left(4).toInt();
		beginMonth = beginChergeTime.mid(5, 2).toInt();
		beginDay = beginChergeTime.mid(8, 2).toInt();
		beginHour = beginChergeTime.mid(11, 2).toInt();
		beginMinite = beginChergeTime.mid(14, 2).toInt();

		nInterval = (dateCurrent.year() - beginYear) * 365 * 24 * 60;
		nInterval += (dateCurrent.month() - beginMonth) * 30 * 24 * 60;
		nInterval += (dateCurrent.day() - beginDay) * 24 * 60;
		nInterval += (timeCurrent.hour() - beginHour) * 60;
		nInterval += (timeCurrent.minute() - beginMinite);
		return nInterval; //返回分钟
	}
};

// 层级 20180920
struct stLevel
{
	unsigned int nLevel;
	std::map<unsigned int, stBatteryInfo> mapBattery;
	std::map<unsigned int, stCharger> mapCharger;
	stLevel() :nLevel(0){

	}
};
//继电器信息
struct stRelay
{
	//unsigned int id;			//继电器编号 
	//std::map<unsigned int, stCharger> mapCharger;//充电器列表（映射）1个继电器器最多关联二个充电器
	//std::map<unsigned int, stBatteryInfo> mapBattery;//电池列表（映射）1个继电器器最多关联六个电池
	//char szLoopInfo[7];			//回路闭合信息
	//char szBatteryExistInfo[7]; //电池存在信息
	//stRelay() :id(0){
	//	memset(szLoopInfo, 0, 7);
	//	memset(szBatteryExistInfo, 0, 7);
	//}
	//stRelay(const stRelay& other){
	//	this->id = other.id;
	//	this->mapCharger = other.mapCharger;
	//	this->mapBattery = other.mapBattery;
	//	strcpy(this->szLoopInfo, other.szLoopInfo);
	//	strcpy(this->szBatteryExistInfo, other.szBatteryExistInfo);
	//}
	//void getCurrentOnLoop(int* loopA, int* loopB){  //获取两组当前闭合的回路，A; B, -1代表所有回路未闭合
	//	*loopA = -1;
	//	*loopB = -1;
	//	for (int i = 0; i < MAX_RELAY_TO_LOOP; i++){
	//		if (i < 3 && szLoopInfo[i] == '1'){
	//			*loopA = i;
	//		}
	//		if (i >= 3 && szLoopInfo[i] == '1'){
	//			*loopB = i;
	//		}
	//	}
	//}
	////根据闭合的回路查找电池编号
	//int getBattery(int nloop){
	//	for (std::map<unsigned int, stBatteryInfo>::iterator itBattery = mapBattery.begin(); \
	//		itBattery != mapBattery.end(); itBattery++){
	//		if (itBattery->second.relatedLoop == nloop)
	//		{
	//			return atoi(itBattery->second.id);
	//		}
	//	}
	//	return -1;
	//}
};

//电池id与电池存在关系
struct stBatteryIsExist{
	unsigned int nBatteryID;
	bool bExisted;
	stBatteryIsExist(){
		nBatteryID = 0;
		bExisted = false;
	}
};


//充电柜信息
struct stChargingCloset
{
	char id[10];  //充电柜编号
	
	//std::map<unsigned int, stRelay> mapRelay;//继电器信息

	std::map<unsigned int, stCharger> mapCharger;//充电器信息

	std::map<unsigned int, stBatteryInfo> mapBattery;//电池列表（映射） 通过id关联（柜子的ID 后两位与电池ID 前两位相同
	stChargingCloset(){
		memset(id, 0, 10);
		
	}
};



typedef std::map<unsigned int, stChargingCloset> MAP_CLOSET;   //充电柜列表类型
typedef std::map<unsigned int, stChargingCloset>::iterator MAP_CLOSET_IT;  //充电柜列表迭代器
typedef std::map<unsigned int, stBatteryInfo> MAP_BATTERY;  //电池列表
typedef std::map<unsigned int, stBatteryInfo>::iterator MAP_BATTERY_IT;  //电池列表迭代器
typedef std::map<unsigned int, stBatteryModelInfo> MAP_BATTERY_MODEL; //电池型号
typedef std::map<unsigned int, stBatteryModelInfo>::iterator MAP_BATTERY_MODEL_IT; //电池型号

typedef std::map<unsigned int, stCharger> MAP_CHARGER; //充电器
typedef std::map<unsigned int, stCharger>::iterator MAP_CHARGER_IT;

typedef std::map<unsigned int, stLevel> MAP_LEVEL; //继电器
typedef std::map<unsigned int, stLevel>::iterator MAP_LEVEL_IT;
 


//命令 结构体
struct stCommand
{
	enum enPriority{		//优先级
		normal = 0,   
		front,		//前级命令 插入队列前。
		hight,		//高级命令 插入队列前，并且检测失败重发2次。
	};
	enPriority m_enPriority;
	enChargerType chargerType; //充电器类型， 根据充电器类型区分命令发到串口或者CAN
	QString m_strCommand;  //命令
	bool lastCommandFlag;   //单次循环最后的命令，如果true，则抛出下次循环的命令。

	stCommand(QString strCommand="", enPriority pri = normal) :\
		m_strCommand(strCommand), m_enPriority(pri), chargerType(NF_Charger),lastCommandFlag(false){

		}
	stCommand operator=(const stCommand & other){
		this->m_enPriority = other.m_enPriority;
		this->m_strCommand = other.m_strCommand;
		this->chargerType = other.chargerType;
		this->lastCommandFlag = other.lastCommandFlag;
		return *this;
	}

};

//电池申请情况
struct stApplyInfo
{
	unsigned int myID;  //数据库的ID
	bool bApply;	//被申请的表示
	char iCount;   //充电次数
	bool bNeedCharge; //尝试充电的标志	
	QTime timeLockUI;	//禁止刷新UI的计时器 
	bool bNeedStopCharge;  //停止充电
	stApplyInfo()
	{
		myID = 0; bApply = false; iCount = 0; bNeedCharge = false;	bNeedStopCharge = false; timeLockUI.start();
	}
};

//申请电池且未充电的结构体
struct stApplyBatteryDontCharge{
	unsigned int nBatteryID;
	QString strTime;
	stApplyBatteryDontCharge(){
		nBatteryID = 0;
	}
	stApplyBatteryDontCharge(const stApplyBatteryDontCharge& other){
		nBatteryID = other.nBatteryID;
		strTime = other.strTime;
	}
	stApplyBatteryDontCharge& operator= (const stApplyBatteryDontCharge& other){
		nBatteryID = other.nBatteryID;
		strTime = other.strTime;
		return *this;
	}
	bool compare(const stApplyBatteryDontCharge& other){
		return nBatteryID == other.nBatteryID;
	}
};

//调试信息显示等级
enum enDebugInfoPriority{
	DebugInfoClose = 0,	//0关闭显示
	DebugInfoLevelOne,  //1显示结果
	DebugInfoLevelTwo,  //2显示结果、过程
	DebugInfoLevelThree,  //3显示结果、过程、细节
};

//刷新类型
enum enRefreshType{
	ChargerState = 0,  //刷新充电器状态
	ChargerOnlineState,  //充电器在线状态
	BatteryState,		//电池状态
	BatteryVol,		//电池电压
};

//进程通信简易数据包
struct stProcessSimpltData 
{
	char strInfo[256];
	int nTime1;
	int nSubmitTime ;
	float fOverHeatTemperature;
	int nDischargeDay;
	stProcessSimpltData() :nTime1(0), nSubmitTime(0), fOverHeatTemperature(45.0), nDischargeDay(10){
		memset(strInfo, 0, 256);
	}
};

//充电记录类型
enum enChargeRecordStateType{
	StateBegin = 0,  //开始充电
	StateEnd,		 //停止充电
	 
};

//数据结构体，更新电池数据用
struct stBatteryData{
	int nBatteryId;
	QString strData;  //保存电池 SN、寿命百分比
	bool bNeedRefresh;  //跟新标志
	stBatteryData() :nBatteryId(0), bNeedRefresh(0){

	} 
};
typedef std::map<int, stBatteryData> MAP_BtData; //数据
typedef std::map<int, stBatteryData>::iterator MAP_BtData_IT;
