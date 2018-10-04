#include "CanProcess.h" 
#include "common.h"
#include "CommonFunction.h"


static DWORD s_canDeviceProcessId = 0;  //����id
static HANDLE s_hPipe = 0;//�ܵ����
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

//���������ܵ�ȥ����


//����canͨѶ����
bool CCanProcess::startCanDeviceProcess(char * szPrintf)
{
	if (s_canDeviceProcessId)
		return true;
	char szServerPath[256] = { 0 };
	sprintf_s(szServerPath, 256, "%s/%s", (g_AppPath), CANDEVICETRANSMITION);
	s_canDeviceProcessId = COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());
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
		COM_F::startProcessFromPath(s_strTemp.c_str(), 1);
		::Sleep(1000);
		s_canDeviceProcessId = COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());

	}
	if (s_canDeviceProcessId)
	{
		//���������ܵ�
		s_hPipe = COM_F::connectServerNamePipe(TEXT(PIPE_NAME));
		if (INVALID_HANDLE_VALUE == s_hPipe)
		{
			sprintf_s(szPrintf, 256, "����ͨ��ʧ��!\n");
			return false;
		}
	}
	sprintf_s(szPrintf, 256, "����ͨ���ɹ�!\n");
	return s_canDeviceProcessId != 0;
}

//�رս���
bool CCanProcess::clossCanDeviceProcess(char * szPrintf)
{
	//�жϽ���id
	if (s_canDeviceProcessId > 0)
	{
		//�ж�����ͨ�����
		if (s_hPipe > 0)
		{
			//���ͽ����Ƴ�����
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
		sprintf_s(szPrintf, 256, "CANͨѶ���̹ر�!\n");
	}
	return 0;
}

//����
int CCanProcess::sendToCanDeviceProcess(char * szData, int nLength, char * szPrintf)
{
	if (s_hPipe > 0)
	{
		DWORD wlen = 0;
		WriteFile(s_hPipe, szData, strlen(szData), &wlen, 0);//��CAN����������
		
		sprintf_s(szPrintf, 256, "���ͣ�%s %s\n", szData, (wlen==0)?"δ����":"");
		Sleep(10);
		return wlen;
	}
	sprintf_s(szPrintf, 256, "����ʧ�ܣ��ܵ�δ��!\n");
	return 0;
}

//����
int CCanProcess::receiveFromCanDeviceProcess(char * szData, char * szPrintf)
{
	if (s_hPipe > 0)
	{
		DWORD rlen = 0;
		ReadFile(s_hPipe, szData, MAX_BUF_SIZE, &rlen, NULL); //����CAN���̷��͹���������
		sprintf_s(szPrintf, 256, "���գ�%s\n", szData);
		return rlen;
	}
	sprintf_s(szPrintf, 256, "����ʧ�ܣ��ܵ�δ��!\n");
	return 0;
}
