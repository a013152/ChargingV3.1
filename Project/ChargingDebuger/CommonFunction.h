#pragma once


/*常用的函数类
方法都是静态属性，


**********************
author： chenlin
date： 20180922
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
	/*多字节转宽字节*/
	static std::wstring MBytesToWString(const char* lpcszString);

	/*宽字节转多字节*/
	static std::string WStringToMBytes(const wchar_t* lpwcszWString);

	// ASCII与Unicode互转  
	static std::wstring AsciiToUnicode(const std::string& str);
	static std::string  UnicodeToAscii(const std::wstring& wstr);
	// UTF8与Unicode互转  
	static std::wstring Utf8ToUnicode(const std::string& str);
	static std::string  UnicodeToUtf8(const std::wstring& wstr);
	// ASCII与UTF8互转  
	static std::string  AsciiToUtf8(const std::string& str);
	static std::string  Utf8ToAscii(const std::string& str);


	//查找can设备通讯进程:返回进程id
	static DWORD GetProcessidFromName(LPCTSTR name);

	//强制关闭can通讯进程
	static bool closeProcessFromId(DWORD dwTH32ProcessID);
	
	//启动进程， 参数：推荐使用绝对路径
	static bool startProcessFromPath(LPCTSTR szPath, int showMode);

	//连接服务端的有名管道
	static HANDLE connectServerNamePipe(LPCTSTR pipeName);

	static void closeNamePipe(HANDLE hPipe);

	static std::wstring getAppDir();

	static void split(std::string strtem, char splitChar, std::vector<std::string>& vtStrCommand);
};

