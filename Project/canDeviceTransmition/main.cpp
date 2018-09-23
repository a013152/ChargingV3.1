// TransmitionTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <string>
#include "Common.h"
#include "../common/common.h"
#include "Protocol.h"
#include "Transmit.h"
#include <cstdlib>     
#define GET_T CTransmit::GetInstance()
#define GET_P CProtocol::GetInstance()
#include <string>
#include <vector>
void displayOption(){
	printf("\n������ָ��: \n\t0 �˳�\t1 ��can�豸\t2 ��ȡcanidָ��\
		   \n\t3 ����canidָ��\t4 ��֤\t5 ��ȡ��ʼģʽ\
		   \n\t6 ��ȡ�������\t7 ��ȡ��翪��\t8 ��ȡ�汾��Ϣ\
		   \n\t9 ��ȡ�ܾ�̬����\t10 ��ȡ��̬����\t11 ��ȡ�ŵ翪��\
		   \n");
}
//�ص���������ӡ�������
static void _callbackPrintf(int nType, bool bDisplayOption)
{
	if (nType == 1)
		printf("%s\n", GET_P->getDebugData().c_str());
	if (nType == 2)
		printf("%s\n", GET_T->getDebugData().c_str());
	if (bDisplayOption)
		displayOption();
}

//�ָ��ַ�
void split(std::string strtem, char a, std::vector<std::string>& vtStrCommand)
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

void openCAN()
{
	//���豸
	int result = GET_T->openCanDev();
	if (result == 0){
		printf("���豸�ɹ���\n");
		displayOption();
	}
	else if (result == -1)
	{
		printf("open failed\n");
	}
	else if (result == -2){
		printf("Init-CAN failed!\n");
	}
	else if (result != -3)
	{
		printf("Start-CAN failed!\n");
	}
}

void readCanId()
{
	stCAN_DevData dataObj;
	GET_P->getCommandReadCanId(dataObj);
	GET_T->sendCanData(dataObj, Uint8ToUint16(g_CAN_ID_Common));
}

void writeCanId()
{
	stCAN_DevData dataObj;
	GET_P->getCommandWriteCanId(dataObj, g_CAN_ID_Default);
	GET_T->sendCanData(dataObj, Uint8ToUint16(g_CAN_ID_Common));
}

void sendVerify()
{
	stCAN_DevData dataObj;
	GET_P->getCommandVerify(dataObj);
	UINT canID = Uint8ToUint16(g_CAN_ID_Default); 
	canID |= 0x400;   //v1.3can id��Ҫ����0x400
	GET_T->sendCanData(dataObj, canID);
}


void readBeginMode()
{
	stCAN_DevData dataObj;
	GET_P->getCommandBeginMode(dataObj);
	UINT canID = Uint8ToUint16(g_CAN_ID_Default);
	canID |= 0x400;   //v1.3can id��Ҫ����0x400
	GET_T->sendCanData(dataObj, canID);
}

void readMaxCharge()
{
	stCAN_DevData dataObj;
	GET_P->getCommandMaxCharge(dataObj);
	UINT canID = Uint8ToUint16(g_CAN_ID_Default);
	canID |= 0x400;   //v1.3can id��Ҫ����0x400
	GET_T->sendCanData(dataObj, canID);
}

void readOrWriteCharge()
{
	printf("������(0��ȡ���/1���ó��):\n");
	int nReadOrWrite = 0, chargeId = 0;
	stCAN_DevData dataObj;
	scanf_s("%d", &nReadOrWrite);
	switch (nReadOrWrite)
	{
	case 0:
		GET_P->getCommandCharge(dataObj, false, 0);
		break;
	case 1:
	{
			  printf("���������(1-15):");
			  scanf_s("%d", &chargeId);
			  if (chargeId>0 && chargeId <=15  )
			  {
				  GET_P->getCommandCharge(dataObj, true, chargeId);
				  break;
			  }
			  else
			  {
				  printf("��������\n"); 
				  displayOption();
				  return;
			  }

	}
	default:
		printf("δ֪����\n");
		displayOption();
		return;
		break;
	}
	
	
	UINT canID = Uint8ToUint16(g_CAN_ID_Default);
	canID |= 0x400;   //v1.3can id��Ҫ����0x400
	GET_T->sendCanData(dataObj, canID);
}

void readLoadAppInfo()
{
	stCAN_DevData dataObj;
	GET_P->getCommandLoadAppInfo(dataObj);
	UINT canID = Uint8ToUint16(g_CAN_ID_Default);
	canID |= 0x400;   //v1.3can id��Ҫ����0x400
	GET_T->sendCanData(dataObj, canID);
}

void readStaticData()
{
	stCAN_DevData dataObj;
	GET_P->getCommandStaticData(dataObj);
	UINT canID = Uint8ToUint16(g_CAN_ID_Default);
	canID |= 0x400;   //v1.3can id��Ҫ����0x400
	GET_T->sendCanData(dataObj, canID);
}

void readDynaData()
{
	stCAN_DevData dataObj;
	GET_P->getCommandDynaData(dataObj);
	UINT canID = Uint8ToUint16(g_CAN_ID_Default);
	canID |= 0x400;   //v1.3can id��Ҫ����0x400
	GET_T->sendCanData(dataObj, canID);
}

void readOrWriteDisC()
{
	printf("������(0��ȡ�ŵ�/1���÷ŵ�):\n");
	int nReadOrWrite = 0, chargeId = 0;
	stCAN_DevData dataObj;
	scanf_s("%d", &nReadOrWrite);
	switch (nReadOrWrite)
	{
	case 0:
		GET_P->getCommandDisCharge(dataObj, false, 0);
		break;
	case 1:
	{
			  printf("������ŵ��(1-15):");
			  scanf_s("%d", &chargeId);
			  if (chargeId > 0 && chargeId <= 15)
			  {
				  GET_P->getCommandDisCharge(dataObj, true, chargeId);
				  break;
			  }
			  else
			  {
				  printf("��������\n");
				  displayOption();
				  return;
			  }

	}
	default:
		printf("��������\n");
		displayOption();
		return;
		break;
	}

	UINT canID = Uint8ToUint16(g_CAN_ID_Default);
	canID |= 0x400;   //v1.3can id��Ҫ����0x400
	GET_T->sendCanData(dataObj, canID); 
}

int _tmain(int argc, _TCHAR* argv[])
{
	system("mode con cols=100 ");    //�������ڴ�С
	GET_P->m_pPrintfFun = _callbackPrintf;
	GET_T->m_pPrintfFun = _callbackPrintf;
	//���������ܵ�
	HANDLE hPipe = 0;
	DWORD wlen = 0;
	DWORD rlen = 0;
	//�����ܵ�
	hPipe = CreateNamedPipe(
		TEXT(PIPE_NAME),						//�ܵ���
		PIPE_ACCESS_DUPLEX,									//�ܵ����� 
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,	//�ܵ�������˫��ͨ�� 
		PIPE_UNLIMITED_INSTANCES,							//�ܵ��ܴ��������ʵ������
		0,													//������������� 0��ʾĬ��
		0,													//���뻺�������� 0��ʾĬ��
		NMPWAIT_WAIT_FOREVER,								//��ʱʱ��,NMPWAIT_WAIT_FOREVERΪ����ʱ�ȴ�
		NULL);
	if (INVALID_HANDLE_VALUE == hPipe)
	{
		printf("�����ܵ�ʧ��:%d ", GetLastError());
		system("PAUSE");
	}
	else{
		printf("�����ܵ���ɣ��ȴ����ý������ӹܵ�\n");
		//�ȴ��ͻ������ӹܵ�
		ConnectNamedPipe(hPipe, NULL);
		printf("�����̽���ܵ�\n");

		std::vector<std::string> vtStrCommand;
		while (true)
		{
			char rbuf[256] = "";
			char wbuf[256] = "";
			if (ReadFile(hPipe, rbuf, sizeof(rbuf), &rlen, 0) != FALSE)	//���ܷ����͹���������
			{
				printf("������������Ϣ: data = %s, size = %d\n", rbuf, rlen);

				vtStrCommand.clear();
				//ת�����ݵ�����ṹ��
				printf("������������Ϣ: data = %s, size = %d\n", rbuf, rlen);
				split(rbuf, ',', vtStrCommand);


				if (strcmp(vtStrCommand[1].c_str(), "ff") == 0)
				{
					GET_T->closeCanDev();
					sprintf_s(wbuf, 256 ,"CANͨѶ�����Ƴ���\n");					
					WriteFile(hPipe, wbuf, strlen(wbuf), &wlen, 0);
					Sleep(1000);
					break;
				}
				else if (strcmp(vtStrCommand[1].c_str(), "0xf1") == 0)
				{
					//���豸
					int result = GET_T->openCanDev();
					if (result == 0){
						printf("���豸�ɹ���\n");
						strcpy_s(wbuf, 256, "���豸�ɹ���\n"); 
					}
					else if (result == -1)
					{
						printf("open failed\n");
						strcpy_s(wbuf, 256, "���豸ʧ�ܡ�\n");
					}
					else if (result == -2){
						printf("Init-CAN failed!\n");
						strcpy_s(wbuf, 256, "��ʼ���豸��ʧ�ܡ�\n");
					}
					else if (result != -3)
					{
						printf("Start-CAN failed!\n");
						strcpy_s(wbuf, 256, "����CAN�豸��ʧ�ܡ�\n");
					}
				}
				else if (strcmp(rbuf, "0xf2") == 0)
				{
					GET_T->closeCanDev();
					strcpy_s(wbuf, 256, "CAN�豸�Ѿ��رա�\n");
				}
				

				WriteFile(hPipe, wbuf, strlen(wbuf), &wlen, 0);

				 
			}
		}
	}
	CloseHandle(hPipe);

	return 0;



	int a = 0;
	displayOption();
	while (true)
	{
		scanf_s("%d", &a);
		switch (a)
		{
		case  0:
			return 0;
			break;
		case  1:
			openCAN();
			break;
		case  2:
			readCanId();
			break;
		case  3:
			writeCanId();
			break; 
		case  4:
				sendVerify();
				break;
		case  5:
			readBeginMode();
			break;
		case  6:
			readMaxCharge();
			break;
		case  7:
			readOrWriteCharge();
			break;
		case  8:
			readLoadAppInfo();
			break;
		case  9:
			readStaticData();
			break;
		case  10:
			readDynaData();
			break;
		case  11:
			readOrWriteDisC();
			break;
		default:
			printf("δ֪�������������\n");
			break;
		}
	}
	
	
	 
}

