#pragma once
/*CAN 设备进程 通讯类， 处理与CAN进程的发送和读取*/

#include <string.h>
#include <string>
#include <windows.h> 

class CCanProcess;
#define GET_CAN CCanProcess::GetInstance()

class CCanProcess
{
	DWORD s_canDeviceProcessId = 0;  //进程id
	HANDLE s_hPipe = 0;//管道句柄
	std::wstring s_strTemp;
	
	
	char szPrintf[256] ;


	CCanProcess();

public:
	bool m_bOpenCanDevice = false; //打开CAN设备成功的标志, 只有在CAN进程返回“S2C,F1,0,打开设备成功。” 设置为true
	~CCanProcess();

	static CCanProcess* GetInstance();

	//是否准备好，发送、接收
	bool isPreareSendOrRead();

	//启动can通讯进程
	bool startCanDeviceProcess(char * szPrintf);


	//关闭进程
	bool clossCanDeviceProcess(char * szPrintf);


	//发送 成功返回发送的长度
	int sendToCanDeviceProcess(char * szData, int nLength, char * szPrintf);


	//接收 阻塞 ，成功则返回接收的长度
	int receiveFromCanDeviceProcess(char * szData, char * szPrintf);


};

