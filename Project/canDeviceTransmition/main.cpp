// TransmitionTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <string>
#include "Common.h"
#include "../common/common.h" 
#include "functionLayer.h"
#include <cstdlib>  
static char rbuf[256] = "";
static char wbuf[MAX_BUF_SIZE] = "";
static bool s_sendFlg = false; //���͵����ý��̵ı�־����ִ�д�������̷߳��ؽ��Ϊtrue)
HANDLE hPipe = 0;	DWORD wlen = 0;	DWORD rlen = 0;

void displayOption(){
	printf("\n������ָ��: \n\t0 �˳�\t1 ��can�豸\t2 ��ȡcanidָ��\
		   \n\t3 ����canidָ��\t4 ��֤\t5 ��ȡ��ʼģʽ\
		   \n\t6 ��ȡ�������\t7 ��ȡ��翪��\t8 ��ȡ�汾��Ϣ\
		   \n\t9 ��ȡ�ܾ�̬����\t10 ��ȡ��̬����\t11 ��ȡ�ŵ翪��\
		   \n");
}
void sendToClint()
{
	//printf("�������ܵ����%d������:%s",hPipe, wbuf);
	WriteFile(hPipe, wbuf, MAX_BUF_SIZE, &wlen, 0);
	memset(wbuf, MAX_BUF_SIZE, 0);
}
//�ص���������ӡ�������
static void _callbackPrintf(int nType, bool bSend2Clint = true)
{
	if (nType == 1)
	{
		printf("%s\n", GET_P->getDebugData().c_str());
		sprintf_s(wbuf, MAX_BUF_SIZE, "%s\n", GET_P->getDebugData().c_str());
	}
	if (nType == 2)
	{
		printf("DebugData:%s\n", GET_T->getDebugData().c_str()); 
		if (bSend2Clint)
			sprintf_s(wbuf, MAX_BUF_SIZE, "%s,%s,%s,%d,%s\n", S2C, GET_T->getCurrentCommandType().c_str(),\
				GET_T->getCurrentCANID().c_str(), enCANDevieErrorCode::DetailError, GET_T->getDebugData().c_str()); 
	}
	if (bSend2Clint)
	{
		//displayOption();
		s_sendFlg = true;
	}
}

//�ָ��ַ�������1 ԭ�ַ��������룩�� ����2 �ָ��ַ������룩�� ����3 �ַ������������ã������
void split(std::string strtem, char a, VT_STR vtStrCommand)
{
	vtStrCommand.clear(); 
	std::string::size_type pos1, pos2;
	pos2 = strtem.find(a);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		vtStrCommand.push_back(strtem.substr(pos1, pos2 - pos1));
		pos1 = pos2 + 1;
		pos2 = strtem.find(a, pos1);
	}
	vtStrCommand.push_back(strtem.substr(pos1));
}

void readBeginMode()
{
	
}

void readMaxCharge()
{
	stCAN_DevData dataObj;
	GET_P->getCommandMaxCharge(dataObj);
	UINT canID = Uint8ToUint16(g_CAN_ID_Default);
	canID |= 0x400;   //v1.3can id��Ҫ����0x400
	GET_T->sendCanData(dataObj, canID);
}

//void readOrWriteCharge()
//{
//	printf("������(0��ȡ���/1���ó��):\n");
//	int nReadOrWrite = 0, chargeId = 0;
//	stCAN_DevData dataObj;
//	scanf_s("%d", &nReadOrWrite);
//	switch (nReadOrWrite)
//	{
//	case 0:
//		GET_P->getCommandCharge(dataObj, false, 0,1);
//		break;
//	case 1:
//	{
//			  printf("���������(1-15):");
//			  scanf_s("%d", &chargeId);
//			  if (chargeId>0 && chargeId <=15  )
//			  {
//				  GET_P->getCommandCharge(dataObj, true, chargeId,false);
//				  break;
//			  }
//			  else
//			  {
//				  printf("��������\n"); 
//				  displayOption();
//				  return;
//			  }
//
//	}
//	default:
//		printf("δ֪����\n");
//		displayOption();
//		return;
//		break;
//	}
//	
//	
//	UINT canID = Uint8ToUint16(g_CAN_ID_Default);
//	canID |= 0x400;   //v1.3can id��Ҫ����0x400
//	GET_T->sendCanData(dataObj, canID);
//}

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
	//system("mode con cols=100 ");    //�������ڴ�С
	GET_P->m_pPrintfFun = _callbackPrintf;
	GET_T->m_pPrintfFun = _callbackPrintf;	
	
	//�����ܵ�
	hPipe = CreateNamedPipe(
		TEXT(PIPE_NAME),						//�ܵ���
		PIPE_ACCESS_DUPLEX /*| FILE_FLAG_OVERLAPPED*/,			//�ܵ����� ��ʹ���ص�IO
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,	//�ܵ�������˫��ͨ�� 
		PIPE_UNLIMITED_INSTANCES,							//�ܵ��ܴ��������ʵ������
		0,													//������������� 0��ʾĬ��
		0,													//���뻺�������� 0��ʾĬ��
		1000,												//��ʱʱ��,����1�룬NMPWAIT_WAIT_FOREVERΪ����ʱ�ȴ�
		NULL);
	if (INVALID_HANDLE_VALUE == hPipe)
	{
		printf("�����ܵ�ʧ�ܣ�������:%d �������˳���", GetLastError());
		Sleep(3000);
		return 0;
	}
	else{
		printf("�����ܵ���ɣ��ȴ������ý��̡����ӹܵ���\n");
		//�ȴ��ͻ������ӹܵ�


		//OVERLAPPED tagOver;		memset(&tagOver, 0x0, sizeof(tagOver)); 		
		//tagOver.hEvent = CreateEvent(NULL,
		//	TRUE,//�ֹ�reset		
		//	TRUE,//��ʼ״̬signaled		
		//	NULL);//δ����
		BOOL nRet = ConnectNamedPipe(hPipe, NULL);
		if (nRet == FALSE)
		{
			printf("���������ܵ�ʧ�ܣ�������:%d �������˳���", GetLastError());
			Sleep(3000);
			return 0;
		}
		printf("�����ý��̡�����ܵ���\n");

		std::vector<std::string> vtStrCommand;
		
		while (true)
		{
			if (ReadFile(hPipe, rbuf, sizeof(rbuf), &rlen, 0) != FALSE)	//���ܷ����͹���������
			{
				//ת�����ݵ�����ṹ��
				split(rbuf, ',', vtStrCommand);
				printf("������������Ϣ: data = %s  length = %d\n", rbuf, rlen);
				
				//��ջ�����				
				memset(rbuf, 0, 256);

				//�жϵ�һԪ���Ƿ�ΪC2S
				if (vtStrCommand[0].compare(C2S))
				{
					printf("��һԪ�ز��ܽ�������������\n");
					sprintf_s(wbuf, 256, "���ݣ�%s Ԫ�ز��ܽ�������������", rbuf);
					sendToClint();
					continue;
				}
				//����������ɷ�����
				if (strcmp(vtStrCommand[1].c_str(), "FF") == 0)
				{
					GET_T->closeCanDev();
					sprintf_s(wbuf, 256, "%s,%d,%s", S2C, enCANDevieErrorCode::Success, "CANͨѶ���̼����˳���\n");
					sendToClint();
					Sleep(1000);
					break;
				}
				else if (vtStrCommand[1].compare("F1") == 0)
				{
					static int count = 0;
					printf("openCAN ��������%d��\n", ++count);
					//���豸
					openCAN(wbuf);
					s_sendFlg = true;
					
				}
				else if (GET_T->isOpenCanDev() == false)
				{
					s_sendFlg = true;
					sprintf_s(wbuf, 256, "%s,%d,%s", S2C, enCANDevieErrorCode::DetailError, "CAN�豸δ��.\n");
				}
				
				else if (strcmp(vtStrCommand[1].c_str(), "F2") == 0)
				{
					//�ر��豸
					closeCAN(wbuf);
					s_sendFlg = true;
				}
				else if (strcmp(vtStrCommand[1].c_str(), "F3") == 0)
				{
					//��ȡ/����CAN ID
					if(readOrWriteCANID(vtStrCommand, wbuf))
						s_sendFlg = false;
					else
						s_sendFlg = true;
				}
				else if (strcmp(vtStrCommand[1].c_str(), "F4") == 0)
				{
					GET_P->setCurrentCANID(vtStrCommand[2]);
					GET_T->setCurrentCANID(vtStrCommand[2]);
					GET_P->setCurrentCommandType(vtStrCommand[1]);
					GET_T->setCurrentCommandType(vtStrCommand[1]);
					//��֤
					if (verifyDevice(vtStrCommand, wbuf))
						s_sendFlg = false;
					else
						s_sendFlg = true;
				}
				else if (strcmp(vtStrCommand[1].c_str(), "F5") == 0)
				{
					GET_P->setCurrentCANID(vtStrCommand[2]);
					GET_T->setCurrentCANID(vtStrCommand[2]);
					GET_P->setCurrentCommandType(vtStrCommand[1]);
					GET_T->setCurrentCommandType(vtStrCommand[1]);
					//��ȡ/������ʼ���״̬
					if (readOrWriteBeginMode(vtStrCommand, wbuf))
						s_sendFlg = false;
					else
						s_sendFlg = true;
					
				}
				else if (strcmp(vtStrCommand[1].c_str(), "F6") == 0)
				{

				}
				else if (strcmp(vtStrCommand[1].c_str(), "F6") == 0)
				{

				}
				else if (strcmp(vtStrCommand[1].c_str(), "F8") == 0)
				{
					GET_P->setCurrentCANID(vtStrCommand[2]);
					GET_T->setCurrentCANID(vtStrCommand[2]);
					GET_P->setCurrentCommandType(vtStrCommand[1]);
					GET_T->setCurrentCommandType(vtStrCommand[1]);
					//��ȡ�����Ϣ
					if (readBatteryInfo(vtStrCommand, wbuf))
						s_sendFlg = false;
					else
						s_sendFlg = true;

					GET_P->setCurrentCANID(vtStrCommand[2]);
				}
				else if (strcmp(vtStrCommand[1].c_str(), "F9") == 0)
				{
					GET_P->setCurrentCANID(vtStrCommand[2]);
					GET_T->setCurrentCANID(vtStrCommand[2]);
					GET_P->setCurrentCommandType(vtStrCommand[1]);
					GET_T->setCurrentCommandType(vtStrCommand[1]);
					//��� /ֹͣ���
					if (readOrWriteChargingState(vtStrCommand, wbuf))
						s_sendFlg = false;
					else
						s_sendFlg = true;

				}
				else if (strcmp(vtStrCommand[1].c_str(), "F10") == 0)
				{
					GET_P->setCurrentCANID(vtStrCommand[2]);

				}
				static char waitTime = 0; waitTime = 0;
				bool flag = true;
				while (flag )
				{
					waitTime++;
					Sleep(10);
					flag = (s_sendFlg == false && waitTime < 30);  //�����߳�����Ӧ�����߳�ʱ300ms�˳�ѭ����					
				}  
				if (waitTime >=30)
					sprintf_s(wbuf, 256, "%s,%d,%s%s", S2C, enCANDevieErrorCode::DetailError, vtStrCommand[1].c_str(),"����ȴ���ʱ��\n");
				 
				sendToClint(); 
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
			//openCAN();
			break;
		case  2:
			//readCanId();
			break;
		case  3:
			//writeCanId();
			break; 
		case  4:
				//sendVerify();
				break;
		case  5:
			readBeginMode();
			break;
		case  6:
			readMaxCharge();
			break;
		case  7:
			//readOrWriteCharge();
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

