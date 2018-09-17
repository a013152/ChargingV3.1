//#include "stdafx.h"
#include "SerialPort.h"
CSerialPort* pthis = NULL;
/** 线程退出标志 */
bool CSerialPort::s_bExit = false;
int  SUM(int a, int b) { return a + b;  }
 

CSerialPort* CSerialPort::GetInstance()
{
	if (pthis == NULL)
		pthis = new CSerialPort();
	return pthis;
}

CSerialPort::CSerialPort()
{ 
	m_hComm = INVALID_HANDLE_VALUE;
	m_hListenThread = INVALID_HANDLE_VALUE;
	m_dwRecvlen = 0;

	InitializeCriticalSection(&m_csCommunicationSync);
	memset(m_btReceiveBuf, 0, 256);

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


CSerialPort::~CSerialPort()
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

void CSerialPort::getComInfo(char* szOut, int iError)
{
	HKEY hKey;
	//LPCWSTR subkey;
	char keyname[256]; //键名数组  
	char keyvalue[256];  //键值数组  
	DWORD keysize, type, valuesize, szOutlength = 0;
	int indexnum;

	//CString commresult;

	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hKey) != 0) {
		iError = -1;//无法打开注册表时返回error  
		return;
	} 

	//char keymessage[256] = { 0 };//键名  
	char message[256] = { 0 };
	char valuemessage[256] = { 0 };//键值  

	for (int i = 0; i < 20; i++)
	{
		indexnum = i;//要读取键值的索引号  
		keysize = sizeof(keyname);
		valuesize = sizeof(keyvalue);
		if (RegEnumValueA(hKey, indexnum, keyname, &keysize, 0, &type, (BYTE*)keyvalue, &valuesize) == 0) { //列举键名和值  
			//memset(keymessage, 0, 256);
			memset(valuemessage, 0, 256);
			//for (int i = 0; i < (int)keysize; i++) {
				//strcat(keymessage, &keyname[i]);//键名  
			//}

			//for (int j = 0; j < 256; j++) 
			{
				if (strlen(keyvalue) != 0x00) {
					strcat_s(valuemessage,256, keyvalue);//键值  
				}
				szOutlength += strlen(valuemessage);
				if (szOutlength < 256)
				{
					strcat_s(szOut, 256, valuemessage);
					szOutlength += strlen(valuemessage);
				}
				if (szOutlength < 256)
				{
					strcat_s(szOut, 256, ","); szOutlength += 1;
				} 			
			}
		}
	}	
	RegCloseKey(hKey);//关闭注册表  

}


bool CSerialPort::openPort(UINT portNo /*= 1*/, UINT baud /*= CBR_19200*/, char parity /*= 'N'*/,
	UINT databits /*= 8*/, UINT stopsbits /*= 1*/, DWORD dwCommEvents /*= EV_RXCHAR*/)
{
	/** 临时变量,将制定参数转化为字符串形式,以构造DCB结构 */
	char szDCBparam[50];
	sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);

	/** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */
	if (!openPortFile(portNo))
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
	if (bIsSuccess)
	{
		bIsSuccess = SetupComm(m_hComm, 2 * 1024, 2 * 1024);
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

	//开启线程接收
	if (!isThreadRunning()){
		if (!OpenListenThread())
			bIsSuccess = FALSE;
	}
	m_dwRecvlen = 0;
	m_bOpen = true;
	return bIsSuccess == TRUE;
}

void CSerialPort::ClosePort()
{
	int nCount = 0;
	while (isThreadRunning()){
		if(++nCount == 3)
			break;
		CloseListenTread();
	}
	
	/** 如果有串口被打开，关闭它 */
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
	}
	m_dwRecvlen = 0;
	m_bOpen = false;
}

bool CSerialPort::openPortFile(UINT portNo)
{
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
	
	/** 如果打开失败，释放资源并返回 */
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
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

	//方法  异步方式
	pSerialPort->ListenThreadReceving();
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
					//wchar_t *szTemp = new wchar_t[256];
					//swprintf_s(szTemp, 256, _T("串口通信中断，线程结束。"));
					//::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
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
				//wchar_t *szTemp = new wchar_t[256];
				//swprintf(szTemp, 256, L"正常结束读取工作，线程退出。");
				//::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
				return;
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

				// COMM_TIME_DEF CurTime;

				//::GetLocalTime((LPSYSTEMTIME)&CurTime);
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

bool CSerialPort::ReadData(char *cRecved)
{
	/** 进入临界区保护 */
	EnterCriticalSection(&m_csCommunicationSync);
	memcpy(cRecved, m_btReceiveBuf, 256);
	memset(m_btReceiveBuf, 0, 256);
	m_dwRecvlen = 0;
	/** 离开临界区保护 */
	LeaveCriticalSection(&m_csCommunicationSync);
	return true;
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
	//EnterCriticalSection(&m_csCommunicationSync);

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
		//LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}

	/** 离开临界区 */
	//LeaveCriticalSection(&m_csCommunicationSync);

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
		
		static BYTE	btReceiveBuf[SP_MAX_COMM_DATA_LEN];//通信口缓冲
		memset(btReceiveBuf, 0, SP_MAX_COMM_DATA_LEN);
		if (dwBytes > SP_MAX_COMM_DATA_LEN){
			dwBytes = SP_MAX_COMM_DATA_LEN;//只接受最高长度
		}
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
				//wchar_t *szTemp = new wchar_t[256];
				//swprintf(szTemp, 256, L"ReceiveData 接收返回 %d, %u", dwBytesRead, dwError);
				//::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
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
			/** 进入临界区保护 */
			EnterCriticalSection(&m_csCommunicationSync);
			if (m_dwRecvlen == 0)
				memcpy(m_btReceiveBuf, btReceiveBuf, SP_MAX_COMM_DATA_LEN);
			else{
				/*unsigned int lengthSub = 0;
				for (lengthSub = n; lengthSub < dwBytesRead; lengthSub++)
				{
					if (btReceiveBuf[lengthSub] == 0X0D && btReceiveBuf[lengthSub + 1] == 0X0A)
					{
						break;
					}
				}
				memcpy(m_btReceiveBuf + sRecvlen, btReceiveBuf, lengthSub + 2);*/
				if (SP_MAX_COMM_DATA_LEN > (m_dwRecvlen + dwBytesRead))
				{
					memcpy(m_btReceiveBuf + m_dwRecvlen, btReceiveBuf, dwBytesRead);
					m_dwRecvlen += dwBytesRead;
				}

			}
			/** 离开临界区保护 */
			LeaveCriticalSection(&m_csCommunicationSync);

			/*BYTE chTemp[256] = { 0x2A, 0x4E, 0x46, 0x2C, 0x59, 0x2C, 0x31, 0x32, \
			0x33, 0x34, 0x2C, 0x36, 0x53, 0x2C, 0x34, 0x2E, 0x30, 0x34, 0x2E, \
			0x30, 0x30, 0x2C, 0x30, 0x2E, 0x32, 0x39, 0x2C, 0x30, 0x2E, 0x30, \
			0x30, 0x2C, 0x1B, 0x2C, 0x0D, 0x0A };

			sRecvlen = strlen((char*)chTemp);
			memcpy(m_btReceiveBuf, chTemp, sRecvlen);*/
			//m_dwRecvlen += dwBytesRead;
			//int nBegin = -1, nEnd = -1;
			//for (i = n; i < SP_MAX_COMM_DATA_LEN; i++)
			//{
			//	if (m_btReceiveBuf[i] == '*' && 'N' == m_btReceiveBuf[i + 1] 
			//		&& 'F' == m_btReceiveBuf[i + 2]){ //找到开始符
			//		nBegin = i;
			//	}
			//	if (m_btReceiveBuf[i] == 0X0d && m_btReceiveBuf[i + 1] == 0X0a)//找到结束符
			//	{
			//		nEnd = i;
			//		//sRecvlen = 0;
			//	}
			//	if (nBegin != -1 && nEnd != -1)
			//	{
			//		bRecognize = true;
			//		//sRecvlen = 0;
			//		//if (i + 2< SP_MAX_COMM_DATA_LEN)
			//		//	m_btReceiveBuf[i + 2] = 0x00;
			//		//char szTemp[SP_MAX_COMM_DATA_LEN] = { 0 };
			//		//memcpy(szTemp, m_btReceiveBuf + nBegin, nEnd + 2 - nBegin);
			//		//memset(m_btReceiveBuf, 0, 256);
			//		//memcpy(m_btReceiveBuf, szTemp, nEnd + 2 - nBegin);

			//		break;
			//	}
			//}
			//if (bRecognize)
			//{
			//	//匹配前3个字符为*NF，通知窗口收数据，避免接收 “3.45，%，\r\n”这类不完整的数据。
			//	if (m_btReceiveBuf[0] == '*' && 'N' == m_btReceiveBuf[1] && 'F' == m_btReceiveBuf[2])
			//	{
			//		//调用回调函数，通知调用层，可以接收数据了。
			//		//::PostMessage(GetParentHWND(), WM_READYTOREAD, GetComObjID(), 0); //
			//	}
			//	recvRecallCount = 0;
			//}
			//else{
			//	if (++recvRecallCount > 10)   //每次10ms 尝试10次，一共100毫秒
			//	{
			//		recvRecallCount = 0;
			//		//wchar_t *szTemp = new wchar_t[256], szTemp2, *szTemp3 = new wchar_t[256];
			//		//CString strDes, strSource; strSource = m_btReceiveBuf;
			//		//for (int i = 0; i < strSource.GetLength(); i++)
			//		//{
			//		//	szTemp2 = strSource[i];
			//		//	swprintf_s(szTemp, 10, L"%02X ", szTemp2);
			//		//	strDes += szTemp;
			//		//}
			//		//swprintf(szTemp, 256, L"未匹配0D0A，HEX接收%s", strDes);
			//		////::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
			//		//strDes = m_btReceiveBuf;
			//		//swprintf(szTemp3, 256, L"未匹配0D0A，ASCII接收%s", strDes);
			//		////::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp3, 0);
			//		//memset(m_btReceiveBuf, 0, 256);
			//		 
			//		return -1;
			//	}
			//}
		}
	}
	return 0;
}