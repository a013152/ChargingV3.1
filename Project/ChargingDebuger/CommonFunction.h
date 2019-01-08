#pragma once


/*���õĺ�����
�������Ǿ�̬���ԣ�


**********************
author�� chenlin
date�� 20180922
*/


#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <TLHELP32.H>

#define COM_F CommonFunction 

class CommonFunction
{
	CommonFunction();
	~CommonFunction();
public:
	/*���ֽ�ת���ֽ�*/
	static std::wstring MBytesToWString(const char* lpcszString);

	/*���ֽ�ת���ֽ�*/
	static std::string WStringToMBytes(const wchar_t* lpwcszWString);

	// ASCII��Unicode��ת  
	static std::wstring AsciiToUnicode(const std::string& str);
	static std::string  UnicodeToAscii(const std::wstring& wstr);
	// UTF8��Unicode��ת  
	static std::wstring Utf8ToUnicode(const std::string& str);
	static std::string  UnicodeToUtf8(const std::wstring& wstr);
	// ASCII��UTF8��ת  
	static std::string  AsciiToUtf8(const std::string& str);
	static std::string  Utf8ToAscii(const std::string& str);


	//����can�豸ͨѶ����:���ؽ���id
	static DWORD GetProcessidFromName(LPCTSTR name);

	//ǿ�ƹر�canͨѶ����
	static bool closeProcessFromId(DWORD dwTH32ProcessID);
	
	//�������̣� �������Ƽ�ʹ�þ���·��
	static bool startProcessFromPath(LPCTSTR szPath, int showMode);

	//���ӷ���˵������ܵ�
	static HANDLE connectServerNamePipe(LPCTSTR pipeName);

	static void closeNamePipe(HANDLE hPipe);

	static std::wstring getAppDir();

	static void split(std::string strtem, char splitChar, std::vector<std::string>& vtStrCommand);
};

