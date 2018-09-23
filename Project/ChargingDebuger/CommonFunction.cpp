#include "stdafx.h"
#include "CommonFunction.h"


CommonFunction::CommonFunction()
{
}


CommonFunction::~CommonFunction()
{
}

/*���ֽ�ת���ֽ�*/
std::wstring CommonFunction::MBytesToWString(const char* lpcszString)
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
std::string CommonFunction::WStringToMBytes(const wchar_t* lpwcszWString)
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
DWORD CommonFunction::GetProcessidFromName(LPCTSTR name)
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
bool CommonFunction::closeProcessFromId(DWORD dwTH32ProcessID)
{
	bool bResult = false;
	if (NULL == dwTH32ProcessID)
	{
		return bResult;
	}
	// �Ѿ�֪������ID,ֱ�ӹر�
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwTH32ProcessID);
	bResult = TerminateProcess(hProcess, 0)==TRUE;
	 
	return bResult;
}


//��������
bool CommonFunction::startProcessFromPath(LPCTSTR szPath)
{
	if (wcscmp(szPath,L"") == 0)
		return false;
	//��������
	SHELLEXECUTEINFO  ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = szPath;        // ִ�еĳ����� 
	ShExecInfo.lpParameters = L"test start process with param.";//����
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOWNORMAL;                // ��ʾ������� 
	ShExecInfo.hInstApp = NULL;
	return ShellExecuteEx(&ShExecInfo) == TRUE;
}

//���ӷ���˵������ܵ�
HANDLE CommonFunction::connectServerNamePipe(LPCTSTR pipeName)
{
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	//���������ܵ�
	if (!WaitNamedPipe(pipeName, NMPWAIT_WAIT_FOREVER))
	{
		//printfDebugInfo("���������ܵ�ʧ�ܣ�", enDebugInfoPriority::DebugInfoLevelOne, true);
		return hPipe;
	}

	hPipe = CreateFile(          //�����ܵ��ļ��������ӹܵ�  
		pipeName,						//�ܵ�����  
		GENERIC_READ | GENERIC_WRITE,   //�ļ�ģʽ  
		0,                              //�Ƿ���  
		NULL,                           //ָ��һ��SECURITY_ATTRIBUTES�ṹ��ָ��  
		OPEN_EXISTING,                  //��������  
		FILE_ATTRIBUTE_NORMAL,          //�ļ����ԣ�NORMALΪĬ������  
		NULL);                          //ģ�崴���ļ��ľ��  
	if (INVALID_HANDLE_VALUE == hPipe)
	{
		//printfDebugInfo("��ͨ��ʧ��!", enDebugInfoPriority::DebugInfoLevelOne, true);
		return hPipe;
	}
	return hPipe;
}

void CommonFunction::closeNamePipe(HANDLE hPipe)
{
	CloseHandle(hPipe);
}


//��ȡӦ�ó���Ŀ¼
std::wstring CommonFunction::getAppDir()
{
	int iResult = 1;
	wchar_t szapipath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szapipath, MAX_PATH);
	std::wstring strFileName = szapipath;
	int pos = strFileName.find_last_of(L"\\");
	if (pos != -1){
		iResult = 0;
		szapipath[pos + 1] = 0;
		strFileName = szapipath;
		//wcscpy_s(szDir, szapipath);
	}
	return strFileName;
} 


//�ָ��ַ�
void CommonFunction::split(std::string strtem, char a, std::vector<std::string>& vtStrCommand)
{ 
	std::vector<std::string> strvec;
	std::string::size_type pos1, pos2;
	pos2 = strtem.find(a);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		strvec.push_back(strtem.substr(pos1, pos2 - pos1));
		pos1 = pos2 + 1;
		pos2 = strtem.find(a, pos1);
	}
	vtStrCommand.push_back(strtem.substr(pos1));
}
