#include "charging.h"
#include "CommandQueue.h"
#include <TLHELP32.H>
#pragma execution_character_set("utf-8")

static DWORD s_canDeviceProcessId = 0;  //����id
static HANDLE s_hPipe = 0;//�ܵ����
static std::wstring s_strTemp;
/*���ֽ�ת���ֽ�*/
std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, strlen(lpcszString), NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, strlen(lpcszString), (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}

/*���ֽ�ת���ֽ�*/
std::string WStringToMBytes(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen;
	// wide char to multi char
	iTextLen = ::WideCharToMultiByte(CP_ACP, 0, lpwcszWString, wcslen(lpwcszWString), NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_ACP, 0, lpwcszWString, wcslen(lpwcszWString), pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}


//����can�豸ͨѶ����:���ؽ���id
DWORD GetProcessidFromName(LPCTSTR name)
{
	PROCESSENTRY32 pe;
	DWORD id = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return 0;
	while (1)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (wcscmp(pe.szExeFile, name) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);
	return id;
}

//ǿ�ƹر�canͨѶ����
bool closeProcessFromName( DWORD dwTH32ProcessID)
{
	bool bResult = FALSE;
	if (NULL == dwTH32ProcessID)
	{
		return bResult;
	}
	 // �Ѿ�֪������ID,ֱ�ӹر�
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwTH32ProcessID);
	bResult = TerminateProcess(hProcess, 0);
	if (bResult)
	{
		s_canDeviceProcessId = 0;
		s_hPipe = 0;
	}
	return bResult;
}

//���������ܵ�ȥ����


//����canͨѶ����
bool charging::startCanDeviceProcess()
{
	if (s_canDeviceProcessId)
		return true;
	 
	char szServerPath[256] = { 0 };
	QByteArray ba = g_AppPath.toLocal8Bit();
	sprintf_s(szServerPath, 256, "%s/%s", ba.data(), CANDEVICETRANSMITION);
	s_canDeviceProcessId = GetProcessidFromName(MBytesToWString(CANDEVICETRANSMITION).c_str());
	if (s_canDeviceProcessId){
		//��ǿ�ƹرս���
		do
		{
			closeProcessFromName(s_canDeviceProcessId);
			Sleep(10);
			s_canDeviceProcessId = GetProcessidFromName(MBytesToWString(CANDEVICETRANSMITION).c_str());
		} while (s_canDeviceProcessId);
	}
	s_strTemp = MBytesToWString(szServerPath);
	if (s_canDeviceProcessId == 0){
		//��������
		SHELLEXECUTEINFO  ShExecInfo;
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = NULL;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = s_strTemp.c_str();        // ִ�еĳ����� 
		ShExecInfo.lpParameters = NULL;
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_SHOWNORMAL;                // ��ʾ������� 
		ShExecInfo.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo);
		::Sleep(1000);
		s_canDeviceProcessId = GetProcessidFromName(MBytesToWString(CANDEVICETRANSMITION).c_str());
	}
	if (s_canDeviceProcessId)
	{
		//���������ܵ�
		if (!WaitNamedPipe(TEXT("\\\\.\\Pipe\\canDevicePipe"), NMPWAIT_WAIT_FOREVER))
		{ 
			printfDebugInfo("���������ܵ�ʧ�ܣ�", enDebugInfoPriority::DebugInfoLevelOne, true);
			return false;
		}

		s_hPipe = CreateFile(          //�����ܵ��ļ��������ӹܵ�  
			TEXT("\\\\.\\Pipe\\canDevicePipe"),	//�ܵ�����  
			GENERIC_READ | GENERIC_WRITE,   //�ļ�ģʽ  
			0,                              //�Ƿ���  
			NULL,                           //ָ��һ��SECURITY_ATTRIBUTES�ṹ��ָ��  
			OPEN_EXISTING,                  //��������  
			FILE_ATTRIBUTE_NORMAL,          //�ļ����ԣ�NORMALΪĬ������  
			NULL);                          //ģ�崴���ļ��ľ��  
		if (INVALID_HANDLE_VALUE == s_hPipe)
		{
			printfDebugInfo("��ͨ��ʧ��!", enDebugInfoPriority::DebugInfoLevelOne, true);
			return false;
		}
	}
	//
	return s_canDeviceProcessId != 0;
}

//�رս���
bool charging::clossCanDeviceProcess()
{
	//�жϽ���id
	if (s_canDeviceProcessId > 0)
	{
		//�ж�����ͨ�����
		if (s_hPipe> 0)
		{
			//���ͽ����Ƴ�����
			char szExit[256] = { 0 };   sprintf_s(szExit, 256, "0xff");
			sendToCanDeviceProcess(szExit, 5);
			char szTemp[256] = { 0 }; 
			receiveFromCanDeviceProcess(szTemp);
			s_canDeviceProcessId = 0;
			CloseHandle(s_hPipe);
			s_hPipe = 0;
			return 0;
		}
		closeProcessFromName(s_canDeviceProcessId);

	}
	return 0;
}

//����
int charging::sendToCanDeviceProcess(char * szData, int nLength)
{
	if (s_hPipe > 0)
	{
		DWORD wlen = 0;
		WriteFile(s_hPipe, szData, strlen(szData), &wlen, 0);//��CAN����������
		QString strQ = "���ͣ�"; strQ += szData;
		printfDebugInfo(strQ, enDebugInfoPriority::DebugInfoLevelOne);
		Sleep(10);
		return wlen;
	}
	return 0;
	 
}

//����
int charging::receiveFromCanDeviceProcess(char * szData)
{
	if (s_hPipe > 0)
	{
		DWORD rlen = 0;
		ReadFile(s_hPipe, szData, 256, &rlen, NULL); //����CAN���̷��͹���������
		QString strQ = "���գ�"; strQ += QString(szData);  
		printfDebugInfo(strQ, enDebugInfoPriority::DebugInfoLevelOne);
	}
	return 0;
}


//�򿪻��߹ر�CAN�豸
void charging::onOpenCanDevice(bool checked)
{
	if (checked)	//�� 
	{
		//startCanDeviceProcess();
	}
	else{  //�ر�
		
	}
	static int i = 0;
	char szTemp[256] = { 0 };
	if (i == 0){
		startCanDeviceProcess();
	}
	if (++i > 2)
	{
		clossCanDeviceProcess();
		i = 0;

	}
	else{
		char sztemp[256] = { 0 };
		sprintf_s(sztemp, "%d", i-1);
		sendToCanDeviceProcess(sztemp, 256);
		receiveFromCanDeviceProcess(szTemp);

	}
}