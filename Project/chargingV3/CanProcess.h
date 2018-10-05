#pragma once
/*CAN 设备进程 通讯类， 处理与CAN进程的发送和读取*/

#include <string.h>
#include <string>
#include <windows.h> 

class CCanProcess;
#define GET_CAN CCanProcess::GetInstance()

class CCanProcess
{

	CCanProcess();

public:
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

