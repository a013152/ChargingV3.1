#include "charging.h"
#include "CommandQueue.h"
#include "CommonFunction.h"

#pragma execution_character_set("utf-8")

static DWORD s_canDeviceProcessId = 0;  //����id
static HANDLE s_hPipe = 0;//�ܵ����
static std::wstring s_strTemp;


 
//���������ܵ�ȥ����


//����canͨѶ����
bool charging::startCanDeviceProcess()
{
	if (s_canDeviceProcessId)
		return true;	 
	char szServerPath[256] = { 0 };	 
	sprintf_s(szServerPath, 256, "%s/%s", (g_AppPath), CANDEVICETRANSMITION);
	s_canDeviceProcessId =COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());
	if (s_canDeviceProcessId){
		//��ǿ�ƹرս���
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

		//��������
		COM_F::startProcessFromPath(s_strTemp.c_str());		
		::Sleep(1000);
		s_canDeviceProcessId = COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());

	}
	if (s_canDeviceProcessId)
	{
		//���������ܵ�
		s_hPipe = COM_F::connectServerNamePipe(TEXT(PIPE_NAME));
		if (INVALID_HANDLE_VALUE == s_hPipe)
		{
			printfDebugInfo("����ͨ��ʧ��!", enDebugInfoPriority::DebugInfoLevelOne, true);
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
		if (COM_F::closeProcessFromId(s_canDeviceProcessId))
		{
			s_canDeviceProcessId = 0;
			s_hPipe = 0;
		}
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