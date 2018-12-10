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
	m_iMaxLength = MAX_CLOSET *MAX_BATTERY;  //Ĭ�� ����� = ����� * ��������
}


CCommandQueue::~CCommandQueue()
{
}

//void CCommandQueue::init(MySerial * serialObj, unsigned int iMaxLength)
void CCommandQueue::init( unsigned int iMaxLength)
{
	//m_pSerial = serialObj;
	m_iMaxLength = iMaxLength * 2;  //�����ĵ������
}

void CCommandQueue::run()  //������� �����ҷ�������
{
	stCommand stCurrentCommand; bool bLockRet = false; QString strPrint, strMsg;
	 
	//strPrint = "����can����";
	//emit printfed(strPrint);

	//// ��can����	
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
	//		strPrint = "��can������ɣ����գ�" + QString::fromLocal8Bit(szPrintf);
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
				sendCommand(stCurrentCommand);//��������
				detectFinishOneCloset(stCurrentCommand); //���һ�����ӵ������Ƿ������
				m_iSend++; //�����ۼ� 
				if (m_queComman.size() > m_iMaxLength)
				{ 
					strMsg.sprintf("���������������%d", m_iMaxLength);
					strPrint = QString("<p><font size=\"%1\" color=\"%2\">%3</font></p>").arg(12).arg("red").arg(strMsg);
					emit printfed(strPrint,3);
				}
			}
			else{
				strMsg = QString::number(m_iSend+1) + "�η�������������ٽ�����ʱ ,����ʧ�ܡ�";
				strPrint = QString("<p><font size=\"%1\" color=\"%2\">%3</font></p>").arg(12).arg("red").arg(strMsg);
				emit printfed(strPrint, 3);
			}
		}    
		Sleep(30);
	}

	//���Թر�can
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

//�������
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
			m_queComman.push_front(command);   //ǰ����������뵽����ǰ
		else if (command.m_enPriority == stCommand::hight)  
			m_queComman.push_front(command);   //�߼���������뵽����ǰ 
		m_MutexA.unlock();
	}
	else{
		strMsg = "���" + command.m_strCommand.mid(3,6)+ " ������������ٽ�����ʱ ,���ʧ�ܡ�";
		strPrint = QString("<p><font size=\"%1\" color=\"%2\">%3</font></p>").arg(12).arg("red").arg(strMsg);
		emit printfed(strPrint, 3);
	}
	
}
//���һ������
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
					m_queComman.push_front(vtStCommand[i]);  //�������

					strMsg = "����������" + vtStCommand[i].m_strCommand ;
					strPrint = QString("<p><font size=\"%1\" color=\"%2\">%3</font></p>").arg(5).arg("red").arg(strMsg);
					emit printfed(strPrint, 1);
				}
			}
			m_MutexA.unlock();
		}
		else{
			strMsg = "���" + vtStCommand[0].m_strCommand.mid(5,1)+ "��������������ٽ�����ʱ ,���ʧ�ܡ�";
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

//������ڹ�����������
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

//�����������
void CCommandQueue::sendCommand(stCommand stcommand)
{
	DEBUG_LOG(" �߳�A-�� ��ʼ��������"+stcommand.m_strCommand+ "\n")
	QByteArray baTemp;
	//QString command, ls_temp, strReceiveContent, strReceiveContent2;
	int timeElapsed = 0, iPosBegin = 0, iPosEnd = 0, nnPos = 0, nrPos = 0, iLength = 0; int sendLoop = 0;
	bool bSendRet = false;
	//��� ����
	//ls_temp  = m_pSerial->serialRead().data();
	char szWriteData[256] = { 0 }, szReadData[256] = { 0 };

	if (stcommand.chargerType == DJI_Charger)
	{
		if (GET_CAN->isPreareSendOrRead() == false || stcommand.m_strCommand.isEmpty()){
			//can����δ׼�����߿����������
		}
		else
		{
			//���͵��󽮳�����豸
			static QByteArray ba; ba = stcommand.m_strCommand.toLocal8Bit();
			//static char szW[256] = { 0 }, szR[256] = { 0 };
			strcpy_s(szW, ba.data());
			GET_CAN->sendToCanDeviceProcess(szW, 256, szPrintf);
			m_strPrintf = "I write: "; m_strPrintf += stcommand.m_strCommand + "\r\n";
			emit printfed(m_strPrintf, 3);

			GET_CAN->receiveFromCanDeviceProcess(szR, szPrintf);
			m_strPrintf = "I read: "; m_strPrintf += QString::fromLocal8Bit(szR) + "\r\n";
			emit printfed(m_strPrintf, 3);
			 
			//���͵�charing��������
			ls_temp = QString::fromLocal8Bit(szR);
			emit readedCAN(ls_temp);
		}

	}else if (stcommand.chargerType ==  NF_Charger)
	{		
		if (SERIAL_PORT->isOpen()/*m_pSerial*/)
		{ 
			
		
			command = stcommand.m_strCommand;

			//�ж���ɨ�� *NF,G,101,,�µĹ���ɨ��
			if (command.at(4) == QChar('G') && (command.at(7) == QChar('0') && command.at(8) == QChar('1')))
			{
				Sleep(1000);
				emit printfed("\r\n", 3);
			}


			//�򴮿ڷ������� 
		 
			//command.replace("%","0");
			//m_pSerial->serialWrite(command.toLatin1()); //д�� 
			auto pSendFun = [&]()->bool{
				bool bSendSuccess = false;
				QTime me_time; int receiveTime = 0;
				SERIAL_PORT->ReadData(szReadData); ls_temp = QString("%1").arg(szReadData);
				if (ls_temp.size() != 0){
					ls_temp = "I discard: " + ls_temp;  //��ӡ��������
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
				SERIAL_PORT->WriteData((unsigned char*)szWriteData, iLength);//д��
				//���շ��ص�����
				me_time.start();

				while (receiveTime++ < 15)
				{
					Sleep(10);
					//QApplication::processEvents();
					memset(szReadData, 0, 256);
					SERIAL_PORT->ReadData(szReadData);
					strReceiveContent += QString("%1").arg(szReadData);// m_pSerial->serialRead().data(); //��ȡ����
					iPosBegin = strReceiveContent.lastIndexOf("*NF");
					iPosEnd = strReceiveContent.lastIndexOf("\r\n");
					if (iPosBegin != -1 && iPosEnd != -1 && iPosBegin < iPosEnd)
					{
						strReceiveContent2 = strReceiveContent.mid(iPosBegin, iPosEnd + 2);
						bSendSuccess = true;
						break;
					}
				}				
			
				//�����ж�
				//timeElapsed = me_time.elapsed();
				if (/*timeElapsed <= 100 &&*/ bSendSuccess)
				{
					//���� ��ȡ���ݲ��źţ�֪ͨreadSerial() ����
					int iError = ChargingClosetError::noError;
					emit readed(command.mid(4, 1), strReceiveContent2, iError);
				}			 
				return bSendSuccess;
			};
			//�ط�3��
			while (sendLoop < 3 && bSendRet == false)
			{
				sendLoop++;
				strReceiveContent.clear();
				bSendRet = pSendFun();
				
				
			}
			//bSendRet = pSendFun();
			//if (bSendRet == false && stcommand.m_enPriority == stCommand::hight) 
			//{ //�߼�������Ҫ�ӷ� 
			//	strReceiveContent.clear();
			//	bSendRet = pSendFun(); 
			//}
			if (bSendRet == false)
			{
				int iError = ChargingClosetError::recvTomeOut;
				if (strReceiveContent.isEmpty() == false){
					QString strInfo; strInfo = command.mid(4, 1) + " ���ճ�ʱ,�ڴ������У�" + strReceiveContent;
					if (iPosBegin == -1) strInfo += ",δ�ҵ�*NF,";
					if (iPosEnd == -1) strInfo += ",δ�ҵ�\\r\\n,";

					emit printfed(strInfo + "\r\n", 3);
					//COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strInfo + "\r\n");
				}
				else{
					emit printfed("I read: " + command.mid(4, 1) + "���ճ�ʱ,�ڴ�δ�յ�����\r\n", 3);
				}
			}
		}
	} 
	DEBUG_LOG(" �߳�A-�� ������������  \n")
}

//����Ƿ����һ�����ӵ�ɨ��
void CCommandQueue::detectFinishOneCloset(stCommand currentCommand)
{ 		
	if (currentCommand.lastCommandFlag == true ){		  
		DEBUG_LOG(" �߳�A-�� һ�������������� �����źţ�ƴװ��һ������\n")
			emit printfed("\r\n", 3);

		Sleep(1000);
		
		emit readyGetBatteryState(1);  //��ǰ�Ķ���ֻ��1������		
	} 
}
