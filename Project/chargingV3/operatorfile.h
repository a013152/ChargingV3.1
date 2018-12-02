#ifndef OPERATORFILE_H
#define OPERATORFILE_H
/*
	操作文件类：主要功能 读取txt文件获取电池等配置，保存修改配置，记录异常操作
//	@version 1.0
//	@author：chenlin
//	@date:2017/09/21

*/ 


#include"typedef.h"
#include "common.h"
#include "errorCode.h"
#include <QString>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>
#include <QMutex>
//OPERATORFILE_EXPORT
class COperatorFile
{
	COperatorFile();
public:
	static COperatorFile* GetInstance(); //获取实例
	~COperatorFile();
	//设置运行目录
	void setAppPath(QString strPath){
		m_strAppPath = strPath; 
	}
	//读取所有配置
	void readAllConfig(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery, MAP_BATTERY_MODEL& mapBatteryModel,\
		MAP_CHARGER& mapCharger,  int* iError); //读取所有配置：充电柜、电池、电池型号、继电器、充电器

	//写入所有配置
	void writeAllConfig(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery, MAP_BATTERY_MODEL& mapBatteryModel, int* iError); //写入所有配置：充电柜、电池、电池型号
	
	//写入充电柜信息
	void writeChargingClosetInfo(MAP_CLOSET& mapCloset, int* iError);

	//写入电池信息
	void writeChargingInfo(MAP_BATTERY& mapBattery, int* iError);

	//写入电池型号信息
	void writeChargingModelInfo(MAP_BATTERY_MODEL& mapBatteryModel, int* iError);
	//创建log文件
	void tryCreateLogFile(); 
	//写log内容
	void writeLog(QString str); 

	//写Debug log内容
	void writeDebugLog(QString str);

	//读取申请电池未充电的信息
	void readApplyBatteryToCharging(QVector<stApplyBatteryDontCharge>& vtApplyBattery, int* iError);
	//写入申请电池未充电的信息
	void writeApplyBatteryToCharging(QVector<stApplyBatteryDontCharge>& vtApplyBattery, int* iError);

	//关闭
	void onCloseFile();

private:
	void readChargingClosetInfo(MAP_CLOSET& mapCloset, int* iError);  //获取充电柜

	void readBatteryInfo(MAP_BATTERY& mapBattery, int* iError);   //读取电池信息

	void readBatteryModel(MAP_BATTERY_MODEL& mapBatteryModel, int* iError); //读取电池新型号信息

	void readChargerInfo(MAP_CHARGER& mapCharger, int* iError); //读取充电器信息

	//void readRelayInfo(MAP_RELAY& mapRelay, int* iError); //读取继电器信息
	
	QString getContent(QString);

	void transitionContent1(QString strSource, QString strTemp1, QByteArray baTemp1, char* szTemp);	

	QString m_strAppPath;

	QTextStream *m_pTextLogin;   //记录文件
	QFile* m_pLogfile; 

	QTextStream *m_pTextApplyBatteryLog;   //申请电池未充电记录文件
	QFile* m_pLogfileApplyBattery;

	QTextStream *m_pTextDebug;   //调试文件
	QFile* m_pDebugLogfile;

	QMutex m_Mutex;
};

#endif // OPERATORFILE_H
