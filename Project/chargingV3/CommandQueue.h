#pragma once
/*
///	��������ࣺ������������� �ͽ��մ��ڵ�����
//	@version 1.0
//	@author��chenlin
//	@date:2017/09/23

	�޶���*
*/

#include <QThread>
#include <QTime>
#include <QMutex>
#include <QQueue>
#include <QVector>
#include "common.h"
#include "typedef.h"
#include "errorCode.h"
//#include "MySerial.h"
#include "operatorfile.h"
#include "SerialPort.h"  
#define SERIAL_PORT CSerialPort::GetInstance()

class CCommandQueue : public QThread
{
	Q_OBJECT
public:
	CCommandQueue();
	~CCommandQueue(); 
	
//	void init(MySerial * serialObj, unsigned int iMaxLength);
	void init(unsigned int iMaxLength);
	void stop();
	void run();  //�߳�����

	bool isReadyAddCommand(); //�Ƿ�����������
	
	void addCommamd(stCommand );  //�������

	void addVtCommand(QVector<stCommand>& vtStCommand);  //���һ������

	void clearCommand();  //�������

	int countCommand(unsigned int nClosetId = 0) const; //������ڹ�����������; 0 ���������������� 1 ����һ�Ź������� ��2 ������Ź�����

	void setPause(bool flag){ m_bContinueRun = flag; }

	QString getReceiveContent() { return  m_strReceiveContent; }
	QString getStrPrintf() { return  m_strPrintf; }
protected: 
	void sendCommand(stCommand); //����������� 
	
	void detectFinishOneCloset(stCommand currentCommand); //����Ƿ����һ�����ӵ�ɨ�� 

private:
	QQueue<stCommand> m_queComman;  //A�������
	 
	unsigned int m_iSend = 0; //���ʹ���

	bool m_bContinueRun = true;  //��������

	QString m_strReceiveContent,m_strPrintf;

private:
	volatile bool stopped;  //�߳�ֹͣ��־
	QMutex m_MutexA;


	//MySerial* m_pSerial;

	unsigned int m_iMaxLength;  //���Ķ�������
signals:
	//param1  �������ͣ�����G ��D��O...)
	//param2  �յ������ݣ�*NF,Y��...)
	//param3  ������ 0 �ɹ�
	void readed(QString strType, QString recvContend ,int iError );  

	void printfed(QString writContend);
	

	void readyGetBatteryState( int nClosetId);  //�źţ���ɶ�ȡһ�����ӱ�� 

	//���յ�CAN�豸���ݵ��źš�
	//param1  �������ݣ�)
	void readedCAN( QString recvContend);

};


