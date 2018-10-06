#pragma once
/*
///	命令队列类：负责发送命令到串口 和接收串口的数据
//	@version 1.0
//	@author：chenlin
//	@date:2017/09/23

	修订：*
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
	void run();  //线程运行

	bool isReadyAddCommand(); //是否就绪添加命令
	
	void addCommamd(stCommand );  //添加命令

	void addVtCommand(QVector<stCommand>& vtStCommand);  //添加一组命令

	void clearCommand();  //清除命令

	int countCommand(unsigned int nClosetId = 0) const; //检测属于柜子命令数量; 0 代表所有命令数， 1 代表一号柜命令数 ，2 代表二号柜命令

	void setPause(bool flag){ m_bContinueRun = flag; }

	QString getReceiveContent() { return  m_strReceiveContent; }
	QString getStrPrintf() { return  m_strPrintf; }
protected: 
	void sendCommand(stCommand); //发送命令到串口 
	
	void detectFinishOneCloset(stCommand currentCommand); //检测是否完成一个柜子的扫描 

private:
	QQueue<stCommand> m_queComman;  //A负责接收
	 
	unsigned int m_iSend = 0; //发送次数

	bool m_bContinueRun = true;  //继续运行

	QString m_strReceiveContent,m_strPrintf;

private:
	volatile bool stopped;  //线程停止标志
	QMutex m_MutexA;


	//MySerial* m_pSerial;

	unsigned int m_iMaxLength;  //最大的队列数量
signals:
	//param1  命令类型（类似G 、D、O...)
	//param2  收到的内容（*NF,Y，...)
	//param3  错误码 0 成功
	void readed(QString strType, QString recvContend ,int iError );  

	void printfed(QString writContend);
	

	void readyGetBatteryState( int nClosetId);  //信号：完成读取一个柜子编号 

	//接收到CAN设备数据的信号。
	//param1  命令内容（)
	void readedCAN( QString recvContend);

};


