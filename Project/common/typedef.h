#pragma  once
/*
�����ṹ��
���ļ��������� st+��д����ĸ ���磺stBatteryInfo �����Ϣ

	@version�� 1.1
	@author��chenlin
	@date��20180122

	�޶�
	1.0  ��������ͺš������Ϣ�ṹ��
	1.1	 �������ṹ��
	1.2  �޸ĵ�ؽṹ ��ӹ���������������̵�����
*/

#include <string.h>
#include <map>
#include <vector>
#include <stdlib.h>
#include<cmath>
#include <QString>
#include <QTime>
#include <QDateTime>
#include "common.h"

//����ͺ���Ϣ
struct stBatteryModelInfo
{
	unsigned int id;		//���
	char droneModel[128];   //���˻��ͺ�	
	unsigned int capacity;  //����	
	float chargingVoltage;//����ѹ
	float chargingCurrent; //������
	float storageVoltage; //�����ѹ
	float fullVoltage;		//���ص�ѹ	
	char connectType[20]; //�������ࣿ4S 6S
	bool balance;   //�Ƿ�ƽ�⣨�����ܵ��false ���ܵ��true)
	

	stBatteryModelInfo() : id(0), capacity(0), chargingVoltage(0.0),\
		chargingCurrent(0.0), storageVoltage(0.0), \
		fullVoltage(0.0), balance(0.0) 
	{
			memset(droneModel, 0, 128);			
			memset(connectType, 0, 128);
	}
};

//�����Ϣ
struct  stBatteryInfo
{
	unsigned int dbid;		  //���ݿ��Ӧid
	char id[5];					//���id	
	unsigned int modelId;     //����ͺ�Id	
	bool online;		      //�Ƿ�ʹ��  //false �Ļ����������߼�
	char user[256];				//����ʹ����
	unsigned int relatedCharger;  //��������� ��ַ
	//unsigned int relatedRelay;  //�����̵������ư� ��ַ
	//char relatedLoop;  //������·  
	bool isExisted;				//�Ƿ����
	bool isChanged;				//�Ƿ񱻸��������//���ɨ�����ڼ��е�ظ�������
	bool isApplyCharging;       //�Ƿ���ҪԤ�䣨���ͬ��ĵ�����ڳ�磬��
	QTime timeLockCloseLoop;	//��ֹ�Ͽ���·�ļ�ʱ�� 
	QTime timeLockUI;   //��ֹˢ�¶�Ӧui�ļ�ʱ��
	
	stBatteryInfo() : dbid(0), online(1), modelId(0), \
		relatedCharger(0), isExisted(false), isChanged(false), isApplyCharging(false){
		memset(id, 0, 10);	
		memset(user, 0, 256);
		timeLockCloseLoop.start();
		timeLockUI.start();
		//memset(gridNO, 0, 4);
	}
	stBatteryInfo(const stBatteryInfo& other)   //�������캯��   
	{
		this->dbid = other.dbid;
		this->modelId = other.modelId;
		this->relatedCharger = other.relatedCharger;
		//this->relatedRelay = other.relatedRelay; 
		this->online = other.online;
		//this->relatedLoop = other.relatedLoop;
		this->isExisted = other.isExisted;
		this->isChanged = other.isChanged;
		this->isApplyCharging = other.isApplyCharging;
		strcpy(this->id, other.id);
		strcpy(this->user, other.user);
		this->timeLockCloseLoop = other.timeLockCloseLoop;
	}
};



//��������� add 20180920
enum enChargerType
{
	DJI_Charger = 0,   //�󽮳���
	NF_Charger = 1		//�ܷɳ���
};

//�������Ϣ
struct stCharger
{
	unsigned int id;  //��������
	unsigned int nLevel;	//���ڲ���
	enChargerType chargerType;  //���������
	//std::map<unsigned int, stBatteryInfo> mapBattery;//����б�ӳ�䣩1������������������
	QTime timeLockChargingState;   //2���ڽ�ֹˢ�³����״̬
	QString beginChergeTime;       //��翪ʼʱ�� add20180521
	bool isChargingPre;
	bool isCharging;  //���ڳ���־
	bool isDisCharging;  //���ڷŵ��־
	bool bOnline;  //���������״̬
	bool isOverHeat;  //�Ƿ����
	char nSeekT;  //�¶����鸡��
	char nSeekV;  //��ѹ���鸡��
	float fTemperature[3];  //�¶�
	float fVoltage[3];			//��ѹ
	float fCurrent;			//����
	//bool bNeedReadVol;  //��Ҫ��ȡ��ѹ��־
	unsigned int nScanWatchDog;  //����״̬���Ź��������жϲ�����
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
	stCharger(const stCharger& other)   //�������캯��   
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
	float getAverage(float* arrayFloat){ //��ȡƽ��
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
		if (fabs(fTemp - averageTemp) < 50)//��ֵ������50�ȣ���Ϊ��Ч
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
				|| (averageTemp != 0 && fabs(fVol - averageTemp) < 0.9))//��ֵ������0.9V����Ϊ��Ч
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

	void updateBeginChargeTime(){  //���³��ʱ��
		beginChergeTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
	}
	int calculateChargeTime()
	{
		if (isChargingPre == false && isCharging == true){
			updateBeginChargeTime(); //��ʼ��� ��¼��ʼ���ʱ�䡣
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
		return nInterval; //���ط���
	}
};

// ��ṹ�� 20180920
struct stLevel
{
	unsigned int nLevel;
	std::map<unsigned int, stBatteryInfo> mapBattery;
	std::map<unsigned int, stCharger> mapCharger;
	stLevel() :nLevel(0){

	}
};
//�̵�����Ϣ
struct stRelay
{
	//unsigned int id;			//�̵������ 
	//std::map<unsigned int, stCharger> mapCharger;//������б�ӳ�䣩1���̵��������������������
	//std::map<unsigned int, stBatteryInfo> mapBattery;//����б�ӳ�䣩1���̵������������������
	//char szLoopInfo[7];			//��·�պ���Ϣ
	//char szBatteryExistInfo[7]; //��ش�����Ϣ
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
	//void getCurrentOnLoop(int* loopA, int* loopB){  //��ȡ���鵱ǰ�պϵĻ�·��A; B, -1�������л�·δ�պ�
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
	////���ݱպϵĻ�·���ҵ�ر��
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

//���id���ش��ڹ�ϵ
struct stBatteryIsExist{
	unsigned int nBatteryID;
	bool bExisted;
	stBatteryIsExist(){
		nBatteryID = 0;
		bExisted = false;
	}
};


//������Ϣ
struct stChargingCloset
{
	char id[10];  //������
	
	//std::map<unsigned int, stRelay> mapRelay;//�̵�����Ϣ

	std::map<unsigned int, stCharger> mapCharger;//�������Ϣ

	std::map<unsigned int, stBatteryInfo> mapBattery;//����б�ӳ�䣩 ͨ��id���������ӵ�ID ����λ����ID ǰ��λ��ͬ
	stChargingCloset(){
		memset(id, 0, 10);
		
	}
};



typedef std::map<unsigned int, stChargingCloset> MAP_CLOSET;   //�����б�����
typedef std::map<unsigned int, stChargingCloset>::iterator MAP_CLOSET_IT;  //�����б������
typedef std::map<unsigned int, stBatteryInfo> MAP_BATTERY;  //����б�
typedef std::map<unsigned int, stBatteryInfo>::iterator MAP_BATTERY_IT;  //����б������
typedef std::map<unsigned int, stBatteryModelInfo> MAP_BATTERY_MODEL; //����ͺ�
typedef std::map<unsigned int, stBatteryModelInfo>::iterator MAP_BATTERY_MODEL_IT; //����ͺ�

typedef std::map<unsigned int, stCharger> MAP_CHARGER; //�����
typedef std::map<unsigned int, stCharger>::iterator MAP_CHARGER_IT;

typedef std::map<unsigned int, stLevel> MAP_LEVEL; //��
typedef std::map<unsigned int, stLevel>::iterator MAP_LEVEL_IT; 


//typedef std::map<unsigned int, stRelay> MAP_RELAY; //�̵���
//typedef std::map<unsigned int, stRelay>::iterator MAP_RELAY_IT;
 


//���� �ṹ��
struct stCommand
{
	enum enPriority{		//���ȼ�
		normal = 0,   
		front,		//ǰ������ �������ǰ��
		hight,		//�߼����� �������ǰ�����Ҽ��ʧ���ط�2�Ρ�
	};
	enPriority m_enPriority;
	QString m_strCommand;  //����
	stCommand(QString strCommand="", enPriority pri = normal) :\
		m_strCommand(strCommand), m_enPriority(pri){

		}
	stCommand operator=(const stCommand & other){
		this->m_enPriority = other.m_enPriority;
		this->m_strCommand = other.m_strCommand;
		return *this;
	}

};

//����������
struct stApplyInfo
{
	unsigned int myID;  //���ݿ��ID
	bool bApply;	//������ı�ʾ
	char iCount;   //������
	bool bNeedCharge; //���Գ��ı�־	
	QTime timeLockUI;	//��ֹˢ��UI�ļ�ʱ�� 
	bool bNeedStopCharge;  //ֹͣ���
	stApplyInfo()
	{
		myID = 0; bApply = false; iCount = 0; bNeedCharge = false;	bNeedStopCharge = false; timeLockUI.start();
	}
};

//��������δ���Ľṹ��
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

//������Ϣ��ʾ�ȼ�
enum enDebugInfoPriority{
	DebugInfoClose = 0,	//0�ر���ʾ
	DebugInfoLevelOne,  //1��ʾ���
	DebugInfoLevelTwo,  //2��ʾ���������
	DebugInfoLevelThree,  //3��ʾ��������̡�ϸ��
};

//ˢ������
enum enRefreshType{
	ChargerState = 0,  //ˢ�³����״̬
	ChargerOnlineState,  //���������״̬
	BatteryState,		//���״̬
	BatteryVol,		//��ص�ѹ
};

//����ͨ�ż������ݰ�
struct stProcessSimpltData 
{
	char strInfo[256];
	int nTime1;
	int nSubmitTime ;
	float fOverHeatTemperature;
	stProcessSimpltData() :nTime1(0), nSubmitTime(0), fOverHeatTemperature(45.0){
		memset(strInfo, 0, 256);
	}
};

//����¼����
enum enChargeRecordStateType{
	StateBegin = 0,  //��ʼ���
	StateEnd,		 //ֹͣ���
	 
};