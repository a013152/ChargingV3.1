//////////////////////////////////////////////////////////////////////////  
/// COPYRIGHT NOTICE  
/// Copyright (c) 2018, 广东能飞航空科技发展有限公司  （版权声明）  
/// All rights reserved.  
///   
/// @file    SerialPort.cpp    
/// @brief   串口通信类的实现文件  
///  
/// 本文件为串口通信类的实现代码  
///  
/// @version 1.0     
/// @author  chenlin    
/// @E-mail：cl@powerfly.com.cn  
/// @date    2018/01/06  
///   
///  
///  修订说明：  
//////////////////////////////////////////////////////////////////////////  

#include "StdAfx.h"  
#include "SerialPort.h"  
#include <process.h>  
#include <iostream>  

/** 线程退出标志 */
bool CSerialPort::s_bExit = false;
/** 当串口无数据时,sleep至下次查询间隔的时间,单位:秒 */
const UINT SLEEP_TIME_INTERVAL = 10;
static CSerialPort *pThis = NULL;
CSerialPort::CSerialPort(void)
: m_hListenThread(INVALID_HANDLE_VALUE)
{
	pThis = this;
	m_hComm = INVALID_HANDLE_VALUE;
	m_hListenThread = INVALID_HANDLE_VALUE;

	InitializeCriticalSection(&m_csCommunicationSync);


	memset(&m_ovRead, 0, sizeof(m_ovRead));
	memset(&m_ovWrite, 0, sizeof(m_ovWrite));

	memset(&m_CommTimeouts, 0, sizeof(m_CommTimeouts));
	memset(&m_DCB, 0, sizeof(m_DCB));

	//建立事件
	m_ovWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_ovRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_hReadShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hWriteShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//初始化事件数组
	m_hReadEventArray[0] = m_hReadShutdownEvent;
	m_hReadEventArray[1] = m_ovRead.hEvent;

	m_hWriteEventArray[0] = m_hWriteShutdownEvent;
	m_hWriteEventArray[1] = m_hWriteEvent;
}

CSerialPort::~CSerialPort(void)
{
	CloseListenTread();
	ClosePort();
	DeleteCriticalSection(&m_csCommunicationSync);

	//以下关闭各事件
	if (m_ovRead.hEvent)
	{
		CloseHandle(m_ovRead.hEvent);
		m_ovRead.hEvent = NULL;
	}
	if (m_ovWrite.hEvent)
	{
		CloseHandle(m_ovWrite.hEvent);
		m_ovWrite.hEvent = NULL;
	}
	if (m_hWriteEvent)
	{
		CloseHandle(m_hWriteEvent);
		m_hWriteEvent = NULL;
	}
	if (m_hReadShutdownEvent)
	{
		CloseHandle(m_hReadShutdownEvent);
		m_hReadShutdownEvent = NULL;
	}
	if (m_hWriteShutdownEvent)
	{
		CloseHandle(m_hWriteShutdownEvent);
		m_hWriteShutdownEvent = NULL;
	}
}

bool CSerialPort::InitPort(UINT portNo /*= 1*/, UINT baud /*= CBR_9600*/, char parity /*= 'N'*/,
	UINT databits /*= 8*/, UINT stopsbits /*= 1*/, DWORD dwCommEvents /*= EV_RXCHAR*/, HWND handleParent)
{
	m_handleParent = handleParent;
	/** 临时变量,将制定参数转化为字符串形式,以构造DCB结构 */
	char szDCBparam[50];
	sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);

	/** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */
	if (!openPort(portNo))
	{
		return false;
	}

	/** 进入临界段 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 是否有错误发生 */
	BOOL bIsSuccess = TRUE;

	/** 在此可以设置输入输出的缓冲区大小,如果不设置,则系统会设置默认值.
	*  自己设置缓冲区大小时,要注意设置稍大一些,避免缓冲区溢出
	*/
	if (bIsSuccess )
	{
	bIsSuccess = SetupComm(m_hComm,2*1024,2*1024);
	}

	/** 设置串口的超时时间,均设为0,表示不使用超时限制 */
	memset(&m_CommTimeouts, 0, sizeof(m_CommTimeouts));
	double dByteRate = (double)SP_SPECIAL_BAUDRATE / (10 * 1000);
	DWORD dwByteRate = (DWORD)(dByteRate + 0.5);
	m_CommTimeouts.ReadIntervalTimeout = 5;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = dwByteRate;
	m_CommTimeouts.ReadTotalTimeoutConstant = 10;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = dwByteRate;
	m_CommTimeouts.WriteTotalTimeoutConstant = 30;//可以考虑调整为10ms
	 
	if (bIsSuccess)
	{
		bIsSuccess = SetCommTimeouts(m_hComm, &m_CommTimeouts);
	}

	DCB  dcb;
	if (bIsSuccess)
	{
		// 将ANSI字符串转换为UNICODE字符串  
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
		wchar_t *pwText = new wchar_t[dwNum];
		if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
		{
			bIsSuccess = TRUE;
		}

		/** 获取当前串口配置参数,并且构造串口DCB参数 */
		bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCBW(pwText, &dcb);
		/** 开启RTS flow控制 */
		dcb.fRtsControl = RTS_CONTROL_ENABLE;

		/** 释放内存空间 */
		delete[] pwText;
	}

	if (bIsSuccess)
	{
		/** 使用DCB参数配置串口状态 */
		dcb.BaudRate = baud;
		bIsSuccess = SetCommState(m_hComm, &dcb);
	}

	/**  清空串口缓冲区 */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/* 设置事件掩模来监视指定通信端口上的事件*/
	DWORD dwMask = 0;
	GetCommMask(m_hComm, &dwMask);
	SetCommMask(m_hComm, dwMask | EV_RXCHAR);

	/** 离开临界段 */
	LeaveCriticalSection(&m_csCommunicationSync);

	return bIsSuccess == TRUE;
}

bool CSerialPort::InitPort(UINT portNo, const LPDCB& plDCB)
{
	/** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */
	if (!openPort(portNo))
	{
		return false;
	}

	/** 进入临界段 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 配置串口参数 */
	if (!SetCommState(m_hComm, plDCB))
	{
		return false;
	}

	/**  清空串口缓冲区 */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	

	/** 离开临界段 */
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

void CSerialPort::ClosePort()
{
	/** 如果有串口被打开，关闭它 */
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
	}
}

bool CSerialPort::openPort(UINT portNo)
{
	/** 进入临界段 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 把串口的编号转换为设备名 */
	char szPort[50];
	if (portNo<10)
		sprintf_s(szPort, "COM%d", portNo);
	else	
		sprintf_s(szPort, "\\\\.\\COM%d", portNo);

	/** 打开指定的串口 */
	m_hComm = CreateFileA(szPort,  /** 设备名,COM1,COM2等 */
		GENERIC_READ | GENERIC_WRITE, /** 访问模式,可同时读写 */
		0,                            /** 共享模式,0表示不共享 */
		NULL,                         /** 安全性设置,一般使用NULL */
		OPEN_EXISTING,                /** 该参数表示设备必须存在,否则创建失败 */
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,			/**尝试以同步方式通信*/
		0);

	/** 退出临界区 */
	LeaveCriticalSection(&m_csCommunicationSync);
	/** 如果打开失败，释放资源并返回 */
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	return true;
}

bool CSerialPort::OpenListenThread()
{
	/** 检测线程是否已经开启了 */
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		/** 线程已经开启 */
		return false;
	}

	s_bExit = false;
	m_bThreadRunning = true;
	/** 线程ID */
	UINT threadId;
	/** 开启串口数据监听线程 */
	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);
	if (!m_hListenThread)
	{
		return false;
	}
	/** 设置线程的优先级,高于普通线程 */
	if (!SetThreadPriority(m_hListenThread, THREAD_PRIORITY_ABOVE_NORMAL))
	{
		return false;
	}

	return true;
}

bool CSerialPort::CloseListenTread()
{
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		/** 通知线程退出 */
		s_bExit = true;
		m_bThreadRunning = false;
		SetEvent(m_hReadShutdownEvent);
		/** 等待线程退出 */
		Sleep(SLEEP_TIME_INTERVAL);

		/** 置线程句柄无效 */
		CloseHandle(m_hListenThread);
		m_hListenThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

UINT CSerialPort::GetBytesInCOM()
{
	DWORD dwError = 0;  /** 错误码 */
	COMSTAT  comstat;   /** COMSTAT结构体,记录通信设备的状态信息 */
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT BytesInQue = 0;
	/** 在调用ReadFile和WriteFile之前,通过本函数清除以前遗留的错误标志 */
	if (ClearCommError(m_hComm, &dwError, &comstat))
	{
		BytesInQue = comstat.cbInQue; /** 获取在输入缓冲区中的字节数 */
	} 
	return BytesInQue;
}

UINT WINAPI CSerialPort::ListenThread(void* pParam)
{
	/** 得到本类的指针 */
	CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);

	//方法1  异步方式
	pSerialPort->ListenThreadReceving();
	return 0;


	//方法2  同步方式
	// 线程循环,轮询方式读取串口数据  
	while (!pSerialPort->s_bExit)
	{		
		static UINT BytesInQue;
		BytesInQue  = pSerialPort->GetBytesInCOM();
		/** 如果串口输入缓冲区中无数据,则休息一会再查询 */
		if (BytesInQue == 0)
		{
			Sleep(SLEEP_TIME_INTERVAL);
			continue;
		}		
		
		//1代通讯方式， 只使用与能飞公司充电柜协议：判断终结符是\r\n
		/** 读取输入缓冲区中的数据并输出显示 */
		char cRecved = 0x00;
		static char s_strRecv[256] = { 0 };
		static unsigned char s_strRecv2[256] = { 0 };
		static unsigned char sub = 0;
		//::PostMessage(pSerialPort->GetParentHWND(), WM_COMSTAT_LEN, BytesInQue, 0);
		CString str; str.Format(L"尝试接收长度：%d\r\n", BytesInQue);
		TRACE(str);
		if (pSerialPort->ReadChar(s_strRecv, BytesInQue) == true)
		{
			for (UINT i = 0; i < BytesInQue; i++)
				s_strRecv2[sub++] = (unsigned char)s_strRecv[i];
			memset(s_strRecv, 0, 256);
			char sztemp1 = s_strRecv2[sub - 1];
			char sztemp2 = s_strRecv2[sub - 2];
			if (sztemp2 == 0x0d && sztemp1 == 0x0a) //判断\r\n 结束符
			{	

				memcpy(pSerialPort->GetMemBuf(), s_strRecv2, 256);
				 
				if (pSerialPort->GetParentHWND())
				{
					::PostMessage(pSerialPort->GetParentHWND(), WM_READYTOREAD, 0, 0);
				}				 
				sub = 0;
				memset(s_strRecv2, 0, 256); 
			} 
		} 
	}
	return 0;
}


//线程：读取函数
void CSerialPort::ListenThreadReceving()
{
	ResetEvent(m_hReadShutdownEvent);//设置退出读事件无信号
	DWORD dwEvent = 0;
	DWORD dwCommEvent = 0;
	DWORD dwError = 0;
	COMSTAT ComStat;
	BOOL  bResult = TRUE;
	//读线程主循环体
	for (;;)
	{
		if (m_hComm && m_hComm != INVALID_HANDLE_VALUE)
		{
			//等待端口信号
			bResult = WaitCommEvent(m_hComm, &dwEvent, &m_ovRead); //这里不阻塞？
			if (bResult) //接收到信号
			{
				//获取端口状态
				memset(&ComStat, 0, sizeof(ComStat));
				::ClearCommError(m_hComm, &dwError, &ComStat);
				if (ComStat.cbInQue == 0)//无数据继续等待
				{
					continue;
				}
			}
			else
			{	

				switch (dwError = GetLastError())
				{
				case ERROR_IO_PENDING:	//同步未确定				
				case 87:				//同步未确定	
					break;
				default:
					//串口通信中断
					wchar_t *szTemp = new wchar_t[256]; 
					swprintf_s(szTemp, 256, _T("串口通信中断，线程结束。"));
					::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
					return;
				}
			}
			
		} 
		//等待事件： 0关闭， 1读取
		dwEvent = WaitForMultipleObjects(2, m_hReadEventArray, FALSE, INFINITE);
		Sleep(10); 
		switch (dwEvent)
		{
			//关闭事件
			case 0:
			{
				  wchar_t *szTemp = new wchar_t[256];
				  swprintf(szTemp, 256, L"正常结束读取工作，线程退出。");
				  ::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
				  return ;
			}
			//读数据事件
			case 1:
			{
				if (m_hComm == NULL || m_hComm == INVALID_HANDLE_VALUE)
				{
					break;
				}
				//获取端口状态
				memset(&ComStat, 0, sizeof(ComStat));
				::ClearCommError(m_hComm, &dwError, &ComStat);

				COMM_TIME_DEF CurTime;
					 
				::GetLocalTime((LPSYSTEMTIME)&CurTime);
				//wchar_t *szTemp = new wchar_t[256];
				//swprintf(szTemp, 256, L"准备接收数据 %u t[%02u:%02u:%02u:%03u]", ComStat.cbInQue, CurTime.wHour, CurTime.wMinute, CurTime.wSecond, CurTime.wMilliseconds);
				//::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
				if (ComStat.cbInQue == 0)//无数据继续等待
				{
					continue;
				}
				ReceiveData(ComStat.cbInQue);//读数据
				break;
			}
		}
	}
}

bool CSerialPort::ReadChar_Ba(char *cRecved)
{
	/** 进入临界区保护 */
	EnterCriticalSection(&m_csCommunicationSync);
	memcpy(cRecved, m_btReceiveBuf, 256);
	memset(m_btReceiveBuf, 0, 256);
	/** 离开临界区保护 */
	LeaveCriticalSection(&m_csCommunicationSync);
	return true;
}

void CSerialPort::WriteChar_Ba(char *cRecved, int len)
{
	/** 进入临界区保护 */
	EnterCriticalSection(&m_csCommunicationSync);
	memcpy(m_btReceiveBuf, cRecved,len);
	/** 离开临界区保护 */
	LeaveCriticalSection(&m_csCommunicationSync);
}

bool CSerialPort::ReadChar(char *cRecved, unsigned int length)
{
	
	BOOL  bResult = TRUE;
	DWORD BytesRead = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** 临界区保护 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 从缓冲区读取length字节的数据 */
	bResult = ReadFile(m_hComm, cRecved, length, &BytesRead, NULL);
	if ((!bResult))
	{
		/** 获取错误码,可以根据该错误码查出错误原因 */
		DWORD dwError = GetLastError();

		/** 清空串口缓冲区 */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);

		return false;
	}

	/** 离开临界区 */
	LeaveCriticalSection(&m_csCommunicationSync);

	return (BytesRead == length);

}

bool CSerialPort::WriteData(unsigned char* pData, unsigned int length)
{
	BOOL   bResult = TRUE;
	DWORD  BytesToSend = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/** 临界区保护 */
	EnterCriticalSection(&m_csCommunicationSync);

	/** 向缓冲区写入指定量的数据 */
	bResult = WriteFile(m_hComm, pData, length, &BytesToSend, &m_ovWrite);
	if (!bResult)
	{
		//DWORD dwError = GetLastError();
		///** 清空串口缓冲区 */
		//PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);

		DWORD dwError = GetLastError();
		switch (dwError)
		{
		case ERROR_IO_PENDING:
		case 87:				//同步未确定	
			BytesToSend = 0;
			bResult = GetOverlappedResult(m_hComm, &m_ovWrite, &BytesToSend, TRUE);


			 
			break;
		default:
			bResult = -3;
		}
		LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}

	/** 离开临界区 */
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

int CSerialPort::ReceiveData(DWORD dwBytes)
{
	int nRet = 0;
	DWORD dwError = 0;
	DWORD dwBytesRead = 0;//单次读取成功的字节数
	DWORD dwProtocolCommLen = 0;

	DWORD i = 0;
	int nCnt = 0;
	DWORD n = 0;
	int nInvalid = 0;
	int nRemain = 0;
	
	while (1)
	{
		if (dwBytes == 0)
		{
			//PRINT("ReceiveData 无数据返回\r\n");
			return -1;
		}
		//读取数据
		static int sRecvlen;  //历史接收长度
		static BYTE	btReceiveBuf[SP_MAX_COMM_DATA_LEN];//通信口缓冲
		nRet = ReadFile(m_hComm, &btReceiveBuf, dwBytes, &dwBytesRead, &m_ovRead);
		if (!nRet)
		{
			switch (dwError = GetLastError())
			{
			case ERROR_IO_PENDING:
			case 87:				//同步未确定 ERROR_INVALID_PARAMETER	
			{
					nRet = GetOverlappedResult(m_hComm, &m_ovRead, &dwBytesRead, TRUE);
					if (!nRet || dwBytesRead == 0)
					{
						//wchar_t *szTemp = new wchar_t[256];
						//swprintf(szTemp, 256, L"ReceiveData 接收错误返回 %d, %u, %u", nRet, dwBytesRead, dwBytes);
						//::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
						return -2;
					}
					break;
			}
			default:
				//PRINT("ReceiveData 接收返回 %u, %u\r\n", dwBytesRead, dwError);
				wchar_t *szTemp = new wchar_t[256];
				swprintf(szTemp, 256, L"ReceiveData 接收返回 %d, %u", dwBytesRead, dwError);
				::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
				return -3;
			}
		}//endif ReadFile
		//TRACE("receive data  %d, [%02X %02X %02X]\r\n", dwBytesRead, m_btReceiveBuf[0], m_btReceiveBuf[1], m_btReceiveBuf[2]);
		if (dwBytesRead == 0)
		{
			return 0;
		}
		else{
			bool bRecognize = false;  //识别通信结束标志
			
			static int recvRecallCount;
			//static BYTE	s_btReceiveBuf[SP_MAX_COMM_DATA_LEN];//通信口缓冲
			if (sRecvlen ==0)
				memcpy( m_btReceiveBuf, btReceiveBuf, SP_MAX_COMM_DATA_LEN);
			else{
				unsigned int lengthSub = 0;
				for (lengthSub = n; lengthSub < dwBytesRead; lengthSub++)
				{
					if (btReceiveBuf[lengthSub] == 0X0D && btReceiveBuf[lengthSub + 1] == 0X0A)
					{										
						break;
					}
				}
				memcpy(m_btReceiveBuf + sRecvlen, btReceiveBuf, lengthSub+2);

			}
			/*BYTE chTemp[256] = { 0x2A, 0x4E, 0x46, 0x2C, 0x59, 0x2C, 0x31, 0x32, \
				0x33, 0x34, 0x2C, 0x36, 0x53, 0x2C, 0x34, 0x2E, 0x30, 0x34, 0x2E, \
				0x30, 0x30, 0x2C, 0x30, 0x2E, 0x32, 0x39, 0x2C, 0x30, 0x2E, 0x30, \
				0x30, 0x2C, 0x1B, 0x2C, 0x0D, 0x0A };
			
			sRecvlen = strlen((char*)chTemp);
			memcpy(m_btReceiveBuf, chTemp, sRecvlen);*/
			sRecvlen += dwBytesRead;
			int nBegin = -1, nEnd = -1; 
			for (i = n; i < SP_MAX_COMM_DATA_LEN; i++)
			{
				if (m_btReceiveBuf[i] == '*' && 'N' == m_btReceiveBuf[i+1] && 'F' == m_btReceiveBuf[i+2]){
					nBegin = i;
				}
				if (m_btReceiveBuf[i] == 0X0D && m_btReceiveBuf[i + 1] == 0X0A)//找到0D0A
				{
					nEnd = i;
				}
				if (nBegin != -1 && nEnd != -1)
				{
					bRecognize = true;
					sRecvlen = 0;
					if (i + 2< SP_MAX_COMM_DATA_LEN)
					m_btReceiveBuf[i + 2] = 0x00;
					char szTemp[SP_MAX_COMM_DATA_LEN] = { 0 };
					memcpy(szTemp, m_btReceiveBuf+nBegin, nEnd+2-nBegin);
					memset(m_btReceiveBuf, 0, 256);
					memcpy(m_btReceiveBuf, szTemp, nEnd + 2 - nBegin);

					break;
				}
			}
			if (bRecognize)
			{
				//匹配前3个字符为*NF，通知窗口收数据，避免接收 “3.45，%，\r\n”这类不完整的数据。
				if (m_btReceiveBuf[0] == '*' && 'N' == m_btReceiveBuf[1] && 'F' == m_btReceiveBuf[2])
					::PostMessage(GetParentHWND(), WM_READYTOREAD, GetComObjID(), 0); //
				recvRecallCount = 0;
			}
			else{			
				if (++recvRecallCount > 10)   //每次10ms 尝试10次，一共100毫秒
				{
					recvRecallCount = 0;
					wchar_t *szTemp = new wchar_t[256], szTemp2, *szTemp3 = new wchar_t[256];
					CString strDes, strSource; strSource = m_btReceiveBuf;
					for (int i = 0; i < strSource.GetLength(); i++)
					{
						szTemp2 = strSource[i];
						swprintf_s(szTemp, 10, L"%02X ", szTemp2);
						strDes += szTemp;
					}
					swprintf(szTemp, 256, L"未匹配0D0A，HEX接收%s", strDes);
					::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
					strDes = m_btReceiveBuf;
					swprintf(szTemp3, 256, L"未匹配0D0A，ASCII接收%s", strDes);
					::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp3, 0);
					memset(m_btReceiveBuf, 0, 256);
					sRecvlen = 0;
					return -1;
				}				
			}
		}
	}
	return 0;
}