#pragma once
/*CAN �豸���� ͨѶ�࣬ ������CAN���̵ķ��ͺͶ�ȡ*/

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

	//�Ƿ�׼���ã����͡�����
	bool isPreareSendOrRead();

	//����canͨѶ����
	bool startCanDeviceProcess(char * szPrintf);


	//�رս���
	bool clossCanDeviceProcess(char * szPrintf);


	//���� �ɹ����ط��͵ĳ���
	int sendToCanDeviceProcess(char * szData, int nLength, char * szPrintf);


	//���� ���� ���ɹ��򷵻ؽ��յĳ���
	int receiveFromCanDeviceProcess(char * szData, char * szPrintf);


};

