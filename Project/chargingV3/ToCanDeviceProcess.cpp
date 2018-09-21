#include "charging.h"
#include "CommandQueue.h"
#include <TLHELP32.H>
#pragma execution_character_set("utf-8")

static DWORD s_canDeviceProcessId = 0;  //进程id
static HANDLE s_hPipe = 0;//管道句柄
static std::wstring s_strTemp;
/*多字节转宽字节*/
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

/*宽字节转多字节*/
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


//查找can设备通讯进程:返回进程id
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

//强制关闭can通讯进程
bool closeProcessFromName( DWORD dwTH32ProcessID)
{
	bool bResult = FALSE;
	if (NULL == dwTH32ProcessID)
	{
		return bResult;
	}
	 // 已经知道进程ID,直接关闭
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwTH32ProcessID);
	bResult = TerminateProcess(hProcess, 0);
	if (bResult)
	{
		s_canDeviceProcessId = 0;
		s_hPipe = 0;
	}
	return bResult;
}

//创建有名管道去连接


//启动can通讯进程
bool charging::startCanDeviceProcess()
{
	if (s_canDeviceProcessId)
		return true;
	 
	char szServerPath[256] = { 0 };
	QByteArray ba = g_AppPath.toLocal8Bit();
	sprintf_s(szServerPath, 256, "%s/%s", ba.data(), CANDEVICETRANSMITION);
	s_canDeviceProcessId = GetProcessidFromName(MBytesToWString(CANDEVICETRANSMITION).c_str());
	if (s_canDeviceProcessId){
		//先强制关闭进程
		do
		{
			closeProcessFromName(s_canDeviceProcessId);
			Sleep(10);
			s_canDeviceProcessId = GetProcessidFromName(MBytesToWString(CANDEVICETRANSMITION).c_str());
		} while (s_canDeviceProcessId);
	}
	s_strTemp = MBytesToWString(szServerPath);
	if (s_canDeviceProcessId == 0){
		//启动进程
		SHELLEXECUTEINFO  ShExecInfo;
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = NULL;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = s_strTemp.c_str();        // 执行的程序名 
		ShExecInfo.lpParameters = NULL;
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_SHOWNORMAL;                // 显示这个程序 
		ShExecInfo.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo);
		::Sleep(1000);
		s_canDeviceProcessId = GetProcessidFromName(MBytesToWString(CANDEVICETRANSMITION).c_str());
	}
	if (s_canDeviceProcessId)
	{
		//连接有名管道
		if (!WaitNamedPipe(TEXT("\\\\.\\Pipe\\canDevicePipe"), NMPWAIT_WAIT_FOREVER))
		{ 
			printfDebugInfo("连接有名管道失败！", enDebugInfoPriority::DebugInfoLevelOne, true);
			return false;
		}

		s_hPipe = CreateFile(          //创建管道文件，即链接管道  
			TEXT("\\\\.\\Pipe\\canDevicePipe"),	//管道名称  
			GENERIC_READ | GENERIC_WRITE,   //文件模式  
			0,                              //是否共享  
			NULL,                           //指向一个SECURITY_ATTRIBUTES结构的指针  
			OPEN_EXISTING,                  //创建参数  
			FILE_ATTRIBUTE_NORMAL,          //文件属性，NORMAL为默认属性  
			NULL);                          //模板创建文件的句柄  
		if (INVALID_HANDLE_VALUE == s_hPipe)
		{
			printfDebugInfo("打开通道失败!", enDebugInfoPriority::DebugInfoLevelOne, true);
			return false;
		}
	}
	//
	return s_canDeviceProcessId != 0;
}

//关闭进程
bool charging::clossCanDeviceProcess()
{
	//判断进程id
	if (s_canDeviceProcessId > 0)
	{
		//判断有名通道句柄
		if (s_hPipe> 0)
		{
			//发送进程推出报文
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

//发送
int charging::sendToCanDeviceProcess(char * szData, int nLength)
{
	if (s_hPipe > 0)
	{
		DWORD wlen = 0;
		WriteFile(s_hPipe, szData, strlen(szData), &wlen, 0);//向CAN进程送内容
		QString strQ = "发送："; strQ += szData;
		printfDebugInfo(strQ, enDebugInfoPriority::DebugInfoLevelOne);
		Sleep(10);
		return wlen;
	}
	return 0;
	 
}

//接收
int charging::receiveFromCanDeviceProcess(char * szData)
{
	if (s_hPipe > 0)
	{
		DWORD rlen = 0;
		ReadFile(s_hPipe, szData, 256, &rlen, NULL); //接受CAN进程发送过来的内容
		QString strQ = "接收："; strQ += QString(szData);  
		printfDebugInfo(strQ, enDebugInfoPriority::DebugInfoLevelOne);
	}
	return 0;
}


//打开或者关闭CAN设备
void charging::onOpenCanDevice(bool checked)
{
	if (checked)	//打开 
	{
		//startCanDeviceProcess();
	}
	else{  //关闭
		
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