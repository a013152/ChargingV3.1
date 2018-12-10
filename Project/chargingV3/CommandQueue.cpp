#include "CommandQueue.h"
#include <QDebug>
#include "errorCode.h"
#include "CanProcess.h"
#pragma execution_character_set("utf-8")
#include "charging.h"
static char szPrintf[MAX_BUF_SIZE] = { 0 };

static int countQueueSize = 0;
char szW[256] = { 0 }, szR[MAX_BUF_SIZE] = { 0 };
QString command, ls_temp, strReceiveContent, strReceiveContent2;
CCommandQueue::CCommandQueue() :stopped(false) 
{
	//m_pSerial = nullptr;
	m_iMaxLength = MAX_CLOSET *MAX_BATTERY;  //默认 最长命令 = 最大电池 * 最大柜数量
}


CCommandQueue::~CCommandQueue()
{
}

//void CCommandQueue::init(MySerial * serialObj, unsigned int iMaxLength)
void CCommandQueue::init( unsigned int iMaxLength)
{
	//m_pSerial = serialObj;
	m_iMaxLength = iMaxLength * 2;  //两倍的电池数量
}

void CCommandQueue::run()  //处理队列 、并且发送数据
{
	stCommand stCurrentCommand; bool bLockRet = false; QString strPrint, strMsg;
	 
	//strPrint = "启动can进程";
	//emit printfed(strPrint);

	//// 打开can进程	
	//bool resultBool = GET_CAN->startCanDeviceProcess(szPrintf);
	//emit printfed(QString::fromLocal8Bit(szPrintf));
	//::Sleep(100);
	//char sztemp[256] = { 0 }, szReceive[MAX_BUF_SIZE] = { 0 };
	//sprintf_s(sztemp, 256, "%s,F1", C2S);
	//if (0 == GET_CAN->sendToCanDeviceProcess(sztemp, 256, szPrintf)){
	//	emit printfed(QString::fromLocal8Bit(szPrintf));
	//}
	//else{
	//	::Sleep(100);
	//	if (0 != GET_CAN->receiveFromCanDeviceProcess(szReceive, szPrintf))
	//	{
	//		strPrint = "打开can进程完成，接收：" + QString::fromLocal8Bit(szPrintf);
	//		emit printfed(strPrint);
	//		//emit printfed(QString::fromLocal8Bit(szPrintf));
	//		emit readedCAN(QString::fromLocal8Bit(szPrintf));
	//	}
	//}

	while (!stopped)
	{ 	
		while (!m_queComman.isEmpty() /*&& SERIAL_PORT->isOpen()*/)
		{ 
			if (!m_bContinueRun)
				break;
			bLockRet = m_MutexA.tryLock(1000);
			if (bLockRet){
				stCurrentCommand = m_queComman.first();
				m_queComman.pop_front();
				m_MutexA.unlock(); 
				sendCommand(stCurrentCommand);//发送命令
				detectFinishOneCloset(stCurrentCommand); //检测一个柜子的命令是否发送完毕
				m_iSend++; //发送累加 
				if (m_queComman.size() > m_iMaxLength)
				{ 
					strMsg.sprintf("命令队列数量超过%d", m_iMaxLength);
					strPrint = QString("<p><font size=\"%1\" color=\"%2\">%3</font></p>").arg(12).arg("red").arg(strMsg);
					emit printfed(strPrint,3);
				}
			}
			else{
				strMsg = QString::number(m_iSend+1) + "次发送命令，尝试锁临界区超时 ,发送失败。";
				strPrint = QString("<p><font size=\"%1\" color=\"%2\">%3</font></p>").arg(12).arg("red").arg(strMsg);
				emit printfed(strPrint, 3);
			}
		}    
		Sleep(30);
	}

	//尝试关闭can
	GET_CAN->clossCanDeviceProcess(szPrintf);
	SERIAL_PORT->ClosePort();


	stopped = false;
}
 
bool CCommandQueue::isReadyAddCommand()
{	
	return m_queComman.size() < m_iMaxLength;
}

void CCommandQueue::stop()
{
	stopped = true;
}

//添加命令
void CCommandQueue::addCommamd(stCommand command)
{ 
	bool bLockRet = false; QString strPrint, strMsg;
	bLockRet = m_MutexA.tryLock(1000);
	if (bLockRet){
		if (command.m_enPriority == stCommand::normal)
		{
			if (m_queComman.size()< m_iMaxLength)
				m_queComman.push_back(command);		
		}
		else if (command.m_enPriority == stCommand::front)
			m_queComman.push_front(command);   //前级的命令插入到队列前
		else if (command.m_enPriority == stCommand::hight)  
			m_queComman.push_front(command);   //高级的命令插入到队列前 
		m_MutexA.unlock();
	}
	else{
		strMsg = "添加" + command.m_strCommand.mid(3,6)+ " 单命令，尝试锁临界区超时 ,添加失败。";
		strPrint = QString("<p><font size=\"%1\" color=\"%2\">%3</font></p>").arg(12).arg("red").arg(strMsg);
		emit printfed(strPrint, 3);
	}
	
}
//添加一组命令
void CCommandQueue::addVtCommand(QVector<stCommand>& vtStCommand)
{
	if (vtStCommand.size())
	{
		bool bLockRet = false; QString strPrint, strMsg;
		bLockRet = m_MutexA.tryLock(1000);
		if (bLockRet){
			if (vtStCommand[0].m_enPriority == stCommand::normal)
			{
				for (int i = 0; i < vtStCommand.size(); i++){
					m_queComman.push_back(vtStCommand[i]);
				}
			}
			else{
				for (int i = vtStCommand.size() - 1; i >= 0; i--){
					m_queComman.push_front(vtStCommand[i]);  //逆序插入

					strMsg = "添加优先命令：" + vtStCommand[i].m_strCommand ;
					strPrint = QString("<p><font size=\"%1\" color=\"%2\">%3</font></p>").arg(5).arg("red").arg(strMsg);
					emit printfed(strPrint, 1);
				}
			}
			m_MutexA.unlock();
		}
		else{
			strMsg = "添加" + vtStCommand[0].m_strCommand.mid(5,1)+ "柜组命令，尝试锁临界区超时 ,添加失败。";
			strPrint = QString("<p><font size=\"%1\" color=\"%2\">%3</font></p>").arg(12).arg("red").arg(strMsg);
			emit printfed(strPrint, 3);
		} 
	} 
}

void CCommandQueue::clearCommand()
{
	bool bLockRet = false; QString strPrint, strMsg;
	bLockRet = m_MutexA.tryLock(1000);
	if (bLockRet){
		m_queComman.clear();
	}
	m_MutexA.unlock();

}

//检测属于柜子命令数量
int CCommandQueue::countCommand(unsigned int nClosetId) const
{
	int count = 0;
	if (nClosetId == 0)
		count = m_queComman.size();
	else{
		QStringList strlist;
		for (int i = 0; i < m_queComman.size(); i++)
		{
			strlist = m_queComman[i].m_strCommand.split(",");
			if (strlist.size()>3){
				if (nClosetId == (strlist[2].toInt() / 100))
					count++;
			}
		}
	} 
	return count;
}

//发送命令到串口
void CCommandQueue::sendCommand(stCommand stcommand)
{
	DEBUG_LOG(" 线程A-》 开始发送命令"+stcommand.m_strCommand+ "\n")
	QByteArray baTemp;
	//QString command, ls_temp, strReceiveContent, strReceiveContent2;
	int timeElapsed = 0, iPosBegin = 0, iPosEnd = 0, nnPos = 0, nrPos = 0, iLength = 0; int sendLoop = 0;
	bool bSendRet = false;
	//清空 数据
	//ls_temp  = m_pSerial->serialRead().data();
	char szWriteData[256] = { 0 }, szReadData[256] = { 0 };

	if (stcommand.chargerType == DJI_Charger)
	{
		if (GET_CAN->isPreareSendOrRead() == false || stcommand.m_strCommand.isEmpty()){
			//can进程未准备或者空命令，不处理。
		}
		else
		{
			//发送到大疆充电器设备
			static QByteArray ba; ba = stcommand.m_strCommand.toLocal8Bit();
			//static char szW[256] = { 0 }, szR[256] = { 0 };
			strcpy_s(szW, ba.data());
			GET_CAN->sendToCanDeviceProcess(szW, 256, szPrintf);
			m_strPrintf = "I write: "; m_strPrintf += stcommand.m_strCommand + "\r\n";
			emit printfed(m_strPrintf, 3);

			GET_CAN->receiveFromCanDeviceProcess(szR, szPrintf);
			m_strPrintf = "I read: "; m_strPrintf += QString::fromLocal8Bit(szR) + "\r\n";
			emit printfed(m_strPrintf, 3);
			 
			//发送到charing分析解码
			ls_temp = QString::fromLocal8Bit(szR);
			emit readedCAN(ls_temp);
		}

	}else if (stcommand.chargerType ==  NF_Charger)
	{		
		if (SERIAL_PORT->isOpen()/*m_pSerial*/)
		{ 
			
		
			command = stcommand.m_strCommand;

			//判断新扫描 *NF,G,101,,新的柜子扫描
			if (command.at(4) == QChar('G') && (command.at(7) == QChar('0') && command.at(8) == QChar('1')))
			{
				Sleep(1000);
				emit printfed("\r\n", 3);
			}


			//向串口发送数据 
		 
			//command.replace("%","0");
			//m_pSerial->serialWrite(command.toLatin1()); //写入 
			auto pSendFun = [&]()->bool{
				bool bSendSuccess = false;
				QTime me_time; int receiveTime = 0;
				SERIAL_PORT->ReadData(szReadData); ls_temp = QString("%1").arg(szReadData);
				if (ls_temp.size() != 0){
					ls_temp = "I discard: " + ls_temp;  //打印抛弃数据
					emit printfed(ls_temp, 3);
				}

				ls_temp = "I write: "; ls_temp += command.left(command.indexOf("\r\n"));
				emit printfed(ls_temp, 3);
				ls_temp.clear();
				baTemp = command.toLocal8Bit();
				iLength = baTemp.size();
				if (iLength < 256)			{
					strncpy(szWriteData, baTemp.data(), iLength);
				}
				else{
					strncpy(szWriteData, baTemp.data(), 256); iLength = 256;
				}
				SERIAL_PORT->WriteData((unsigned char*)szWriteData, iLength);//写入
				//接收返回的数据
				me_time.start();

				while (receiveTime++ < 15)
				{
					Sleep(10);
					//QApplication::processEvents();
					memset(szReadData, 0, 256);
					SERIAL_PORT->ReadData(szReadData);
					strReceiveContent += QString("%1").arg(szReadData);// m_pSerial->serialRead().data(); //读取数据
					iPosBegin = strReceiveContent.lastIndexOf("*NF");
					iPosEnd = strReceiveContent.lastIndexOf("\r\n");
					if (iPosBegin != -1 && iPosEnd != -1 && iPosBegin < iPosEnd)
					{
						strReceiveContent2 = strReceiveContent.mid(iPosBegin, iPosEnd + 2);
						bSendSuccess = true;
						break;
					}
				}				
			
				//正常判断
				//timeElapsed = me_time.elapsed();
				if (/*timeElapsed <= 100 &&*/ bSendSuccess)
				{
					//发送 收取数据测信号，通知readSerial() 处理
					int iError = ChargingClosetError::noError;
					emit readed(command.mid(4, 1), strReceiveContent2, iError);
				}			 
				return bSendSuccess;
			};
			//重发3次
			while (sendLoop < 3 && bSendRet == false)
			{
				sendLoop++;
				strReceiveContent.clear();
				bSendRet = pSendFun();
				
				
			}
			//bSendRet = pSendFun();
			//if (bSendRet == false && stcommand.m_enPriority == stCommand::hight) 
			//{ //高级命令需要从发 
			//	strReceiveContent.clear();
			//	bSendRet = pSendFun(); 
			//}
			if (bSendRet == false)
			{
				int iError = ChargingClosetError::recvTomeOut;
				if (strReceiveContent.isEmpty() == false){
					QString strInfo; strInfo = command.mid(4, 1) + " 接收超时,内存数据有：" + strReceiveContent;
					if (iPosBegin == -1) strInfo += ",未找到*NF,";
					if (iPosEnd == -1) strInfo += ",未找到\\r\\n,";

					emit printfed(strInfo + "\r\n", 3);
					//COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strInfo + "\r\n");
				}
				else{
					emit printfed("I read: " + command.mid(4, 1) + "接收超时,内存未收到数据\r\n", 3);
				}
			}
		}
	} 
	DEBUG_LOG(" 线程A-》 结束发送命令  \n")
}

//检测是否完成一个柜子的扫描
void CCommandQueue::detectFinishOneCloset(stCommand currentCommand)
{ 		
	if (currentCommand.lastCommandFlag == true ){		  
		DEBUG_LOG(" 线程A-》 一组命令命令发送完毕 发送信号，拼装下一组命令\n")
			emit printfed("\r\n", 3);

		Sleep(1000);
		
		emit readyGetBatteryState(1);  //当前改动：只有1个柜子		
	} 
}
