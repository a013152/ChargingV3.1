#ifndef OPERATORFILE_H
#define OPERATORFILE_H
/*
	�����ļ��ࣺ��Ҫ���� ��ȡtxt�ļ���ȡ��ص����ã������޸����ã���¼�쳣����
//	@version 1.0
//	@author��chenlin
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
	static COperatorFile* GetInstance(); //��ȡʵ��
	~COperatorFile();
	//��������Ŀ¼
	void setAppPath(QString strPath){
		m_strAppPath = strPath; 
	}
	//��ȡ��������
	void readAllConfig(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery, MAP_BATTERY_MODEL& mapBatteryModel,\
		MAP_CHARGER& mapCharger,  int* iError); //��ȡ�������ã����񡢵�ء�����ͺš��̵����������

	//д����������
	void writeAllConfig(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery, MAP_BATTERY_MODEL& mapBatteryModel, int* iError); //д���������ã����񡢵�ء�����ͺ�
	
	//д�������Ϣ
	void writeChargingClosetInfo(MAP_CLOSET& mapCloset, int* iError);

	//д������Ϣ
	void writeChargingInfo(MAP_BATTERY& mapBattery, int* iError);

	//д�����ͺ���Ϣ
	void writeChargingModelInfo(MAP_BATTERY_MODEL& mapBatteryModel, int* iError);
	//����log�ļ�
	void tryCreateLogFile(); 
	//дlog����
	void writeLog(QString str); 

	//дDebug log����
	void writeDebugLog(QString str);

	//��ȡ������δ������Ϣ
	void readApplyBatteryToCharging(QVector<stApplyBatteryDontCharge>& vtApplyBattery, int* iError);
	//д��������δ������Ϣ
	void writeApplyBatteryToCharging(QVector<stApplyBatteryDontCharge>& vtApplyBattery, int* iError);

	//�ر�
	void onCloseFile();

private:
	void readChargingClosetInfo(MAP_CLOSET& mapCloset, int* iError);  //��ȡ����

	void readBatteryInfo(MAP_BATTERY& mapBattery, int* iError);   //��ȡ�����Ϣ

	void readBatteryModel(MAP_BATTERY_MODEL& mapBatteryModel, int* iError); //��ȡ������ͺ���Ϣ

	void readChargerInfo(MAP_CHARGER& mapCharger, int* iError); //��ȡ�������Ϣ

	//void readRelayInfo(MAP_RELAY& mapRelay, int* iError); //��ȡ�̵�����Ϣ
	
	QString getContent(QString);

	void transitionContent1(QString strSource, QString strTemp1, QByteArray baTemp1, char* szTemp);	

	QString m_strAppPath;

	QTextStream *m_pTextLogin;   //��¼�ļ�
	QFile* m_pLogfile; 

	QTextStream *m_pTextApplyBatteryLog;   //������δ����¼�ļ�
	QFile* m_pLogfileApplyBattery;

	QTextStream *m_pTextDebug;   //�����ļ�
	QFile* m_pDebugLogfile;

	QMutex m_Mutex;
};

#endif // OPERATORFILE_H
