#include "stdafx.h"
#include <time.h>
#include "Transmit.h"
#include "Protocol.h"
#define GET_P CProtocol::GetInstance()
static CTransmit* pthis = nullptr;
// 接收线程
DWORD WINAPI _ReceiveThread(LPVOID lpParam)
{
	while (1)
	{
		//等待事件： 0关闭， 1读取
		DWORD dwEvent = 0;
		dwEvent = WaitForMultipleObjects(2, pthis->m_hReadEventArray, FALSE, INFINITE);
		Sleep(10);
		switch (dwEvent)
		{
			//关闭事件
		case 0:
		{
				  //正常结束读取工作，线程退出。
				  ResetEvent(pthis->m_hReadEventArray[0]);
				  return 0;
		}
			//读数据事件
		case 1:
		{
				  Sleep(10);
				  ResetEvent(pthis->m_hReadEventArray[1]);
				  pthis->CAN_Receive();
		}
		}
	}
	return 1;
}

CTransmit::CTransmit()
{
	//初始化事件数组
	m_hReadEventArray[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hReadEventArray[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_DevType = VCI_USBCAN2;
	m_DevIndex = 0;
}


CTransmit* CTransmit::GetInstance()
{
	if (nullptr == pthis){
		pthis = new CTransmit();
	}
	return pthis;
}

CTransmit::~CTransmit()
{
}

int CTransmit::openCanDev()
{
	DWORD Reserved = 0;
	//打开设备
	if (VCI_OpenDevice(m_DevType, m_DevIndex, Reserved) != 1)
	{		
		return -1;
	}
	VCI_INIT_CONFIG InitInfo[1];
	InitInfo->Timing0 = 0x00;
	InitInfo->Timing1 = 0x14;
	InitInfo->Filter = 0;
	InitInfo->AccCode = 0x80000008;
	InitInfo->AccMask = 0xFFFFFFFF;
	InitInfo->Mode = 0;
	//初始化通道0
	if (VCI_InitCAN(m_DevType, m_DevIndex, 0, InitInfo) != 1)	//can-0
	{
		return -2;
	}
	Sleep(100);
	//初始化通道0
	if (VCI_StartCAN(m_DevType, m_DevIndex, 0) != 1)	//can-0
	{
		return -3;
	}

	//开启线程
	if (m_hThread == NULL){
		DWORD threadID;
		m_hThread = CreateThread(NULL, 0, _ReceiveThread, NULL, 0, &threadID);	// 创建线程
	}

	m_bOpen = true;
	return 0;
}

int CTransmit::closeCanDev()
{
	SetEvent(m_hReadEventArray[0]); //设置退出线程事件有信号
	WaitForSingleObject(m_hThread, INFINITE); //等待线程关闭
	CloseHandle(m_hThread);	// 关闭内核对象
	m_hThread = NULL;

	if (VCI_CloseDevice(m_DevType, m_DevIndex) != 1) //关闭CAN设备
	{
		return -1;
	}
	return 0;
}

void CTransmit::CAN_Receive()
{
	int NumValue, i, num = 0, Len = 0,kCanIndex=0;
	VCI_CAN_OBJ pCanObj[200];
	memset(m_bufRecive, 0, MAX_BUF_SIZE);
	//for (int kCanIndex = 0; kCanIndex < 2; kCanIndex++) //两通道
	{
		//调用动态链接看接收函数
		NumValue = VCI_Receive(m_DevType, m_DevIndex, kCanIndex, pCanObj, 200, 0);
		//接收信息列表显示 
		/*CString strTime;
		SYSTEMTIME   systime;
		GetLocalTime(&systime);
		strTime.Format("%02d:%02d:%02d:%03d", systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
*/
		for (num = 0; num < NumValue; num++)
		{
			for (i = 0; i < (pCanObj[num].DataLen); i++)	//数据信息
			{
				m_bufRecive[Len++] = pCanObj[num].Data[i];
			}
		}
		str = "";
		for (i = 0; i < Len; i++)	//数据信息
		{
			sprintf_s(szTemp, 256, "%02X ", m_bufRecive[i]);
			str += szTemp;
		}
		//判断是否出现返回多次的情况
		/*time_t curtime = time(NULL);
		tm *ptm = localtime(&curtime);
		char buf[64];
		sprintf(buf, "%d/%02d/%02d %02d:%02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1,
			ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);*/

		static DWORD dwTimePre, dwTimeNow;
		static bool bShortInervert = false;
		dwTimeNow = GetTickCount();//从操作系统启动所经过（elapsed）的毫秒数，它的返回值是DWORD。
		if (dwTimeNow - dwTimePre > 30){

			dwTimePre = dwTimeNow;
			bShortInervert = true;
		}

		if (NumValue != 0)
			m_strDebugData = "收到数据：" + str + "\n";
		else
			m_strDebugData = "收到数据：空";
		if (m_pPrintfFun) m_pPrintfFun(2, ((NumValue == 0 && bShortInervert==true) ? true : false));//NumValue 等于0 ，则打印
		bShortInervert = false;

		//把数据转交协议类处理
		GET_P->analyzeReceiveData(m_bufRecive, Len);

		
	}
}


int CTransmit::sendCanData(stCAN_DevData &dataObj, UINT canID)
{
	int result_ = 0;
	if (m_bOpen == false)
	{
		m_strDebugData = "CAN设备未打开。\n";
		if (m_pPrintfFun) m_pPrintfFun(2, true);
		result_ = -1;
		return result_;
	}
	int length = dataObj.getLen();
	int loop = length / 8 + (length % 8 == 0 ? 0 : 1);
	BYTE * byteTemp = new BYTE[loop * 8 + 1];
	memset(byteTemp, 0, loop * 8 + 1);
	dataObj.packetDown(byteTemp);
	//memcpy(byteTemp, &dataObj,length);
	//"55 09 00 01 00 01 C3 03 14 "
	
	str = "";
	for (int i = 0; i < length; i++)	//数据信息
	{
		sprintf_s(szTemp, 256, "%02X ", byteTemp[i]);
		//str1.Format("%02X ", byteTemp[i]);
		str += szTemp;
	}
	
	m_strDebugData = "发送数据: " + str +"\n";
	if (m_pPrintfFun)m_pPrintfFun(2, false);

	VCI_CAN_OBJ* sendbuf = new VCI_CAN_OBJ[loop];
	for (int i = 0; i < loop; i++){
		sendbuf[i].TimeFlag = 0;
		sendbuf[i].TimeStamp = 0;
		sendbuf[i].SendType = 0; //重发送
		sendbuf[i].ExternFlag = 0;
		sendbuf[i].DataLen = 8;  //长度8
		sendbuf[i].RemoteFlag = 0;
		sendbuf[i].ID = canID;
		sendbuf[i].Data[0] = byteTemp[i * 8 + 0];
		sendbuf[i].Data[1] = byteTemp[i * 8 + 1];
		sendbuf[i].Data[2] = byteTemp[i * 8 + 2];
		sendbuf[i].Data[3] = byteTemp[i * 8 + 3];
		sendbuf[i].Data[4] = byteTemp[i * 8 + 4];
		sendbuf[i].Data[5] = byteTemp[i * 8 + 5];
		sendbuf[i].Data[6] = byteTemp[i * 8 + 6];
		sendbuf[i].Data[7] = byteTemp[i * 8 + 7];
	}
	int flag = VCI_Transmit(m_DevType, m_DevIndex, 0, sendbuf, loop);//CAN message send
	if (flag < 1){
		if (flag == -1)
			result_ = -1;
		else if (flag == 0)
			result_ = -2;
	}
	if (result_ == 0)
	{
		if (dataObj.CMD_ == 0x00 || dataObj.CMD_ == 0x05)
			Sleep(300);
		else //if ()
			Sleep(100);
		SetEvent(m_hReadEventArray[1]);
	}
	delete[] byteTemp;
	return result_;
}