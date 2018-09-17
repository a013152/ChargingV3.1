//#include "stdafx.h"
#include "SerialPort.h"
CSerialPort* pthis = NULL;
/** �߳��˳���־ */
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

	//�����¼�
	m_ovWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_ovRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_hReadShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hWriteShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//��ʼ���¼�����
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

	//���¹رո��¼�
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
	/** ����߳��Ƿ��Ѿ������� */
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		/** �߳��Ѿ����� */
		return false;
	}

	s_bExit = false;
	m_bThreadRunning = true;
	/** �߳�ID */
	UINT threadId;
	/** �����������ݼ����߳� */
	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);
	if (!m_hListenThread)
	{
		return false;
	}
	/** �����̵߳����ȼ�,������ͨ�߳� */
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
		/** ֪ͨ�߳��˳� */
		s_bExit = true;
		m_bThreadRunning = false;
		SetEvent(m_hReadShutdownEvent);
		/** �ȴ��߳��˳� */
		Sleep(SLEEP_TIME_INTERVAL);

		/** ���߳̾����Ч */
		CloseHandle(m_hListenThread);
		m_hListenThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

void CSerialPort::getComInfo(char* szOut, int iError)
{
	HKEY hKey;
	//LPCWSTR subkey;
	char keyname[256]; //��������  
	char keyvalue[256];  //��ֵ����  
	DWORD keysize, type, valuesize, szOutlength = 0;
	int indexnum;

	//CString commresult;

	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hKey) != 0) {
		iError = -1;//�޷���ע���ʱ����error  
		return;
	} 

	//char keymessage[256] = { 0 };//����  
	char message[256] = { 0 };
	char valuemessage[256] = { 0 };//��ֵ  

	for (int i = 0; i < 20; i++)
	{
		indexnum = i;//Ҫ��ȡ��ֵ��������  
		keysize = sizeof(keyname);
		valuesize = sizeof(keyvalue);
		if (RegEnumValueA(hKey, indexnum, keyname, &keysize, 0, &type, (BYTE*)keyvalue, &valuesize) == 0) { //�оټ�����ֵ  
			//memset(keymessage, 0, 256);
			memset(valuemessage, 0, 256);
			//for (int i = 0; i < (int)keysize; i++) {
				//strcat(keymessage, &keyname[i]);//����  
			//}

			//for (int j = 0; j < 256; j++) 
			{
				if (strlen(keyvalue) != 0x00) {
					strcat_s(valuemessage,256, keyvalue);//��ֵ  
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
	RegCloseKey(hKey);//�ر�ע���  

}


bool CSerialPort::openPort(UINT portNo /*= 1*/, UINT baud /*= CBR_19200*/, char parity /*= 'N'*/,
	UINT databits /*= 8*/, UINT stopsbits /*= 1*/, DWORD dwCommEvents /*= EV_RXCHAR*/)
{
	/** ��ʱ����,���ƶ�����ת��Ϊ�ַ�����ʽ,�Թ���DCB�ṹ */
	char szDCBparam[50];
	sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);

	/** ��ָ������,�ú����ڲ��Ѿ����ٽ�������,�����벻Ҫ�ӱ��� */
	if (!openPortFile(portNo))
	{
		return false;
	}

	/** �����ٽ�� */
	EnterCriticalSection(&m_csCommunicationSync);

	/** �Ƿ��д����� */
	BOOL bIsSuccess = TRUE;

	/** �ڴ˿���������������Ļ�������С,���������,��ϵͳ������Ĭ��ֵ.
	*  �Լ����û�������Сʱ,Ҫע�������Դ�һЩ,���⻺�������
	*/
	if (bIsSuccess)
	{
		bIsSuccess = SetupComm(m_hComm, 2 * 1024, 2 * 1024);
	}

	/** ���ô��ڵĳ�ʱʱ��,����Ϊ0,��ʾ��ʹ�ó�ʱ���� */
	memset(&m_CommTimeouts, 0, sizeof(m_CommTimeouts));
	double dByteRate = (double)SP_SPECIAL_BAUDRATE / (10 * 1000);
	DWORD dwByteRate = (DWORD)(dByteRate + 0.5);
	m_CommTimeouts.ReadIntervalTimeout = 5;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = dwByteRate;
	m_CommTimeouts.ReadTotalTimeoutConstant = 10;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = dwByteRate;
	m_CommTimeouts.WriteTotalTimeoutConstant = 30;//���Կ��ǵ���Ϊ10ms

	if (bIsSuccess)
	{
		bIsSuccess = SetCommTimeouts(m_hComm, &m_CommTimeouts);
	}

	DCB  dcb;
	if (bIsSuccess)
	{
		// ��ANSI�ַ���ת��ΪUNICODE�ַ���  
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
		wchar_t *pwText = new wchar_t[dwNum];
		if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
		{
			bIsSuccess = TRUE;
		}

		/** ��ȡ��ǰ�������ò���,���ҹ��촮��DCB���� */
		bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCBW(pwText, &dcb);
		/** ����RTS flow���� */
		dcb.fRtsControl = RTS_CONTROL_ENABLE;

		/** �ͷ��ڴ�ռ� */
		delete[] pwText;
	}

	if (bIsSuccess)
	{
		/** ʹ��DCB�������ô���״̬ */
		dcb.BaudRate = baud;
		bIsSuccess = SetCommState(m_hComm, &dcb);
	}

	/**  ��մ��ڻ����� */
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/* �����¼���ģ������ָ��ͨ�Ŷ˿��ϵ��¼�*/
	DWORD dwMask = 0;
	GetCommMask(m_hComm, &dwMask);
	SetCommMask(m_hComm, dwMask | EV_RXCHAR);

	/** �뿪�ٽ�� */
	LeaveCriticalSection(&m_csCommunicationSync);

	//�����߳̽���
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
	
	/** ����д��ڱ��򿪣��ر��� */
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
	/** �Ѵ��ڵı��ת��Ϊ�豸�� */
	char szPort[50];
	if (portNo<10)
		sprintf_s(szPort, "COM%d", portNo);
	else
		sprintf_s(szPort, "\\\\.\\COM%d", portNo);

	/** ��ָ���Ĵ��� */
	m_hComm = CreateFileA(szPort,  /** �豸��,COM1,COM2�� */
		GENERIC_READ | GENERIC_WRITE, /** ����ģʽ,��ͬʱ��д */
		0,                            /** ����ģʽ,0��ʾ������ */
		NULL,                         /** ��ȫ������,һ��ʹ��NULL */
		OPEN_EXISTING,                /** �ò�����ʾ�豸�������,���򴴽�ʧ�� */
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,			/**������ͬ����ʽͨ��*/
		0);
	
	/** �����ʧ�ܣ��ͷ���Դ������ */
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	return true;
}


UINT CSerialPort::GetBytesInCOM()
{
	DWORD dwError = 0;  /** ������ */
	COMSTAT  comstat;   /** COMSTAT�ṹ��,��¼ͨ���豸��״̬��Ϣ */
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT BytesInQue = 0;
	/** �ڵ���ReadFile��WriteFile֮ǰ,ͨ�������������ǰ�����Ĵ����־ */
	if (ClearCommError(m_hComm, &dwError, &comstat))
	{
		BytesInQue = comstat.cbInQue; /** ��ȡ�����뻺�����е��ֽ��� */
	}
	return BytesInQue;
}

UINT WINAPI CSerialPort::ListenThread(void* pParam)
{
	/** �õ������ָ�� */
	CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);

	//����  �첽��ʽ
	pSerialPort->ListenThreadReceving();
	return 0;
	 
}


//�̣߳���ȡ����
void CSerialPort::ListenThreadReceving()
{
	ResetEvent(m_hReadShutdownEvent);//�����˳����¼����ź�
	DWORD dwEvent = 0;
	DWORD dwCommEvent = 0;
	DWORD dwError = 0;
	COMSTAT ComStat;
	BOOL  bResult = TRUE;
	//���߳���ѭ����
	for (;;)
	{
		if (m_hComm && m_hComm != INVALID_HANDLE_VALUE)
		{
			//�ȴ��˿��ź�
			bResult = WaitCommEvent(m_hComm, &dwEvent, &m_ovRead); //���ﲻ������
			if (bResult) //���յ��ź�
			{
				//��ȡ�˿�״̬
				memset(&ComStat, 0, sizeof(ComStat));
				::ClearCommError(m_hComm, &dwError, &ComStat);
				if (ComStat.cbInQue == 0)//�����ݼ����ȴ�
				{
					continue;
				}
			}
			else
			{

				switch (dwError = GetLastError())
				{
				case ERROR_IO_PENDING:	//ͬ��δȷ��				
				case 87:				//ͬ��δȷ��	
					break;
				default:
					//����ͨ���ж�
					//wchar_t *szTemp = new wchar_t[256];
					//swprintf_s(szTemp, 256, _T("����ͨ���жϣ��߳̽�����"));
					//::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
					return;
				}
			}

		}
		//�ȴ��¼��� 0�رգ� 1��ȡ
		dwEvent = WaitForMultipleObjects(2, m_hReadEventArray, FALSE, INFINITE);
		Sleep(10);
		switch (dwEvent)
		{
			//�ر��¼�
			case 0:
			{
				//wchar_t *szTemp = new wchar_t[256];
				//swprintf(szTemp, 256, L"����������ȡ�������߳��˳���");
				//::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
				return;
			}
				//�������¼�
			case 1:
			{
				if (m_hComm == NULL || m_hComm == INVALID_HANDLE_VALUE)
				{
					break;
				}
				//��ȡ�˿�״̬
				memset(&ComStat, 0, sizeof(ComStat));
				::ClearCommError(m_hComm, &dwError, &ComStat);

				// COMM_TIME_DEF CurTime;

				//::GetLocalTime((LPSYSTEMTIME)&CurTime);
				//wchar_t *szTemp = new wchar_t[256];
				//swprintf(szTemp, 256, L"׼���������� %u t[%02u:%02u:%02u:%03u]", ComStat.cbInQue, CurTime.wHour, CurTime.wMinute, CurTime.wSecond, CurTime.wMilliseconds);
				//::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
				if (ComStat.cbInQue == 0)//�����ݼ����ȴ�
				{
					continue;
				}
				ReceiveData(ComStat.cbInQue);//������
				break;
			}
		}
	}
}

bool CSerialPort::ReadData(char *cRecved)
{
	/** �����ٽ������� */
	EnterCriticalSection(&m_csCommunicationSync);
	memcpy(cRecved, m_btReceiveBuf, 256);
	memset(m_btReceiveBuf, 0, 256);
	m_dwRecvlen = 0;
	/** �뿪�ٽ������� */
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

	/** �ٽ������� */
	//EnterCriticalSection(&m_csCommunicationSync);

	/** �򻺳���д��ָ���������� */
	bResult = WriteFile(m_hComm, pData, length, &BytesToSend, &m_ovWrite);
	if (!bResult)
	{
		//DWORD dwError = GetLastError();
		///** ��մ��ڻ����� */
		//PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);

		DWORD dwError = GetLastError();
		switch (dwError)
		{
		case ERROR_IO_PENDING:
		case 87:				//ͬ��δȷ��	
			BytesToSend = 0;
			bResult = GetOverlappedResult(m_hComm, &m_ovWrite, &BytesToSend, TRUE); 

			break;
		default:
			bResult = -3;
		}
		//LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}

	/** �뿪�ٽ��� */
	//LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

int CSerialPort::ReceiveData(DWORD dwBytes)
{
	int nRet = 0;
	DWORD dwError = 0;
	DWORD dwBytesRead = 0;//���ζ�ȡ�ɹ����ֽ���
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
			//PRINT("ReceiveData �����ݷ���\r\n");
			return -1;
		}
		//��ȡ����
		
		static BYTE	btReceiveBuf[SP_MAX_COMM_DATA_LEN];//ͨ�ſڻ���
		memset(btReceiveBuf, 0, SP_MAX_COMM_DATA_LEN);
		if (dwBytes > SP_MAX_COMM_DATA_LEN){
			dwBytes = SP_MAX_COMM_DATA_LEN;//ֻ������߳���
		}
		nRet = ReadFile(m_hComm, &btReceiveBuf, dwBytes, &dwBytesRead, &m_ovRead);
		if (!nRet)
		{
			switch (dwError = GetLastError())
			{
			case ERROR_IO_PENDING:
			case 87:				//ͬ��δȷ�� ERROR_INVALID_PARAMETER	
			{
				nRet = GetOverlappedResult(m_hComm, &m_ovRead, &dwBytesRead, TRUE);
				if (!nRet || dwBytesRead == 0)
				{
					//wchar_t *szTemp = new wchar_t[256];
					//swprintf(szTemp, 256, L"ReceiveData ���մ��󷵻� %d, %u, %u", nRet, dwBytesRead, dwBytes);
					//::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
					return -2;
				}
				break;
			}
			default:
				//PRINT("ReceiveData ���շ��� %u, %u\r\n", dwBytesRead, dwError);
				//wchar_t *szTemp = new wchar_t[256];
				//swprintf(szTemp, 256, L"ReceiveData ���շ��� %d, %u", dwBytesRead, dwError);
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
			bool bRecognize = false;  //ʶ��ͨ�Ž�����־

			static int recvRecallCount;
			//static BYTE	s_btReceiveBuf[SP_MAX_COMM_DATA_LEN];//ͨ�ſڻ���
			/** �����ٽ������� */
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
			/** �뿪�ٽ������� */
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
			//		&& 'F' == m_btReceiveBuf[i + 2]){ //�ҵ���ʼ��
			//		nBegin = i;
			//	}
			//	if (m_btReceiveBuf[i] == 0X0d && m_btReceiveBuf[i + 1] == 0X0a)//�ҵ�������
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
			//	//ƥ��ǰ3���ַ�Ϊ*NF��֪ͨ���������ݣ�������� ��3.45��%��\r\n�����಻���������ݡ�
			//	if (m_btReceiveBuf[0] == '*' && 'N' == m_btReceiveBuf[1] && 'F' == m_btReceiveBuf[2])
			//	{
			//		//���ûص�������֪ͨ���ò㣬���Խ��������ˡ�
			//		//::PostMessage(GetParentHWND(), WM_READYTOREAD, GetComObjID(), 0); //
			//	}
			//	recvRecallCount = 0;
			//}
			//else{
			//	if (++recvRecallCount > 10)   //ÿ��10ms ����10�Σ�һ��100����
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
			//		//swprintf(szTemp, 256, L"δƥ��0D0A��HEX����%s", strDes);
			//		////::PostMessage(GetParentHWND(), WM_COMSTAT_LEN, (WPARAM)szTemp, 0);
			//		//strDes = m_btReceiveBuf;
			//		//swprintf(szTemp3, 256, L"δƥ��0D0A��ASCII����%s", strDes);
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