#include "CanProcess.h" 
#include "common.h"
#include "CommonFunction.h"


static DWORD s_canDeviceProcessId = 0;  //进程id
static HANDLE s_hPipe = 0;//管道句柄
static std::wstring s_strTemp;
static CCanProcess* pThis = nullptr;
static char szPrintf[256] = { 0 };


CCanProcess::CCanProcess()
{
	
}


CCanProcess::~CCanProcess()
{
}



CCanProcess* CCanProcess::GetInstance()
{
	if (pThis == nullptr)
		pThis = new CCanProcess();
	return pThis;
}

//创建有名管道去连接


//启动can通讯进程
bool CCanProcess::startCanDeviceProcess(char * szPrintf)
{
	if (s_canDeviceProcessId)
		return true;
	char szServerPath[256] = { 0 };
	sprintf_s(szServerPath, 256, "%s/%s", (g_AppPath), CANDEVICETRANSMITION);
	s_canDeviceProcessId = COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());
	if (s_canDeviceProcessId){
		//先强制关闭进程
		do
		{
			if (COM_F::closeProcessFromId(s_canDeviceProcessId))
			{
				s_canDeviceProcessId = 0;
				s_hPipe = 0;
			}
			Sleep(10);
			s_canDeviceProcessId = COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());
		} while (s_canDeviceProcessId);
	}
	s_strTemp = COM_F::MBytesToWString(szServerPath);
	if (s_canDeviceProcessId == 0){

		//启动进程
		COM_F::startProcessFromPath(s_strTemp.c_str(), 1);
		::Sleep(1000);
		s_canDeviceProcessId = COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());

	}
	if (s_canDeviceProcessId)
	{
		//连接有名管道
		s_hPipe = COM_F::connectServerNamePipe(TEXT(PIPE_NAME));
		if (INVALID_HANDLE_VALUE == s_hPipe)
		{
			sprintf_s(szPrintf, 256, "连接通道失败!\n");
			return false;
		}
	}
	sprintf_s(szPrintf, 256, "连接通道成功!\n");
	return s_canDeviceProcessId != 0;
}

//关闭进程
bool CCanProcess::clossCanDeviceProcess(char * szPrintf)
{
	//判断进程id
	if (s_canDeviceProcessId > 0)
	{
		//判断有名通道句柄
		if (s_hPipe > 0)
		{
			//发送进程推出报文
			char szExit[256] = { 0 };
			sprintf_s(szExit, 256, "%s,FF", C2S);
			sendToCanDeviceProcess(szExit, strlen(szExit), szPrintf);
			char szTemp[MAX_BUF_SIZE] = { 0 };
			receiveFromCanDeviceProcess(szTemp, szPrintf);
			//s_canDeviceProcessId = 0;
			CloseHandle(s_hPipe);
			s_hPipe = 0;

		}
		else if (COM_F::closeProcessFromId(s_canDeviceProcessId))
		{
			s_canDeviceProcessId = 0;
			s_hPipe = 0;
		}
		while (s_canDeviceProcessId)
		{
			Sleep(100);
			s_canDeviceProcessId = COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());
		}
		sprintf_s(szPrintf, 256, "CAN通讯进程关闭!\n");
	}
	return 0;
}

//发送
int CCanProcess::sendToCanDeviceProcess(char * szData, int nLength, char * szPrintf)
{
	if (s_hPipe > 0)
	{
		DWORD wlen = 0;
		WriteFile(s_hPipe, szData, strlen(szData), &wlen, 0);//向CAN进程送内容
		
		sprintf_s(szPrintf, 256, "发送：%s %s\n", szData, (wlen==0)?"未返回":"");
		Sleep(10);
		return wlen;
	}
	sprintf_s(szPrintf, 256, "发送失败，管道未打开!\n");
	return 0;
}

//接收
int CCanProcess::receiveFromCanDeviceProcess(char * szData, char * szPrintf)
{
	if (s_hPipe > 0)
	{
		DWORD rlen = 0;
		ReadFile(s_hPipe, szData, MAX_BUF_SIZE, &rlen, NULL); //接受CAN进程发送过来的内容
		sprintf_s(szPrintf, 256, "接收：%s\n", szData);
		return rlen;
	}
	sprintf_s(szPrintf, 256, "接收失败，管道未打开!\n");
	return 0;
}
