// TransmitionTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <string>
#include "Common.h"
#include "Protocol.h"
#include "Transmit.h"
#include <cstdlib>     
#define GET_T CTransmit::GetInstance()
#define GET_P CProtocol::GetInstance()

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
		TEXT("\\\\.\\Pipe\\canDevicePipe"),						//�ܵ���
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
		printf("�����ܵ���ɣ��ȴ����������ӹܵ�\n");
		//�ȴ��ͻ������ӹܵ�
		ConnectNamedPipe(hPipe, NULL);
		printf("�����̽���ܵ�\n");
		while (true)
		{
			char rbuf[256] = "";
			char wbuf[256] = "";
			if (ReadFile(hPipe, rbuf, sizeof(rbuf), &rlen, 0) != FALSE)	//���ܷ����͹���������
			{
				printf("������������Ϣ: data = %s, size = %d\n", rbuf, rlen);
				
				if (strcmp(rbuf, "0xff") == 0)
				{
					sprintf_s(wbuf, 256 ,"���̼����˳���\n");
					
					WriteFile(hPipe, wbuf, strlen(wbuf), &wlen, 0);
					Sleep(1000);
					break;
				}
				strcpy_s(wbuf, 256, rbuf);

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

