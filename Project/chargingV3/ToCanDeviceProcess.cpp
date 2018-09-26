#include "charging.h"
#include "CommandQueue.h"
#include "CommonFunction.h"

#pragma execution_character_set("utf-8")

static DWORD s_canDeviceProcessId = 0;  //进程id
static HANDLE s_hPipe = 0;//管道句柄
static std::wstring s_strTemp;


 
//创建有名管道去连接


//启动can通讯进程
bool charging::startCanDeviceProcess()
{
	if (s_canDeviceProcessId)
		return true;	 
	char szServerPath[256] = { 0 };	 
	sprintf_s(szServerPath, 256, "%s/%s", (g_AppPath), CANDEVICETRANSMITION);
	s_canDeviceProcessId =COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());
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
		COM_F::startProcessFromPath(s_strTemp.c_str(),1);		
		::Sleep(1000);
		s_canDeviceProcessId = COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());

	}
	if (s_canDeviceProcessId)
	{
		//连接有名管道
		s_hPipe = COM_F::connectServerNamePipe(TEXT(PIPE_NAME));
		if (INVALID_HANDLE_VALUE == s_hPipe)
		{
			printfDebugInfo("连接通道失败!", enDebugInfoPriority::DebugInfoLevelOne, true);
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
			char szExit[256] = { 0 };   
			sprintf_s(szExit, 256, "%s,FF", C2S);
			sendToCanDeviceProcess(szExit,strlen(szExit));
			char szTemp[MAX_BUF_SIZE] = { 0 }; 
			receiveFromCanDeviceProcess(szTemp);
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
		printfDebugInfo(QString("CAN通讯进程\n"), enDebugInfoPriority::DebugInfoLevelOne); 
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
		ReadFile(s_hPipe, szData, MAX_BUF_SIZE, &rlen, NULL); //接受CAN进程发送过来的内容
		QString strQ = "接收："; strQ += QString::fromLocal8Bit(szData);  
		printfDebugInfo(strQ, enDebugInfoPriority::DebugInfoLevelOne);
	}
	return 0;
}


//打开或者关闭CAN设备
void charging::onOpenCanDevice(bool checked)
{
	if (checked)	//打开 
	{
		startCanDeviceProcess();
		::Sleep(100);
		char sztemp[256] = { 0 };
		sprintf_s(sztemp, 256, "%s,F1", C2S);
		sendToCanDeviceProcess(sztemp, 256);
		::Sleep(100);
		char szReceive[MAX_BUF_SIZE] = { 0 };
		receiveFromCanDeviceProcess(szReceive);
	}
	else{  //关闭
		clossCanDeviceProcess(); 
	}
	
}