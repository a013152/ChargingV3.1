#include "stdafx.h"
#include "functionLayer.h"
#include <regex>


static int iResult = 0;
//ƥ��CANID
bool isGoodCANID(const char* szCANID)
{ 
	char pattern_[] = "[0-9a-fA-F]{3,4}";
	return regex_search(szCANID, std::regex(pattern_));
}


void openCAN(char * resultString)
{
	//���豸
	int result = GET_T->openCanDev();
	if (result == 0){
		printf("���豸�ɹ���\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDeviErrorCode::Success, "���豸�ɹ���\n");


	}
	else if (result == -1)
	{
		printf("���豸ʧ�ܡ�\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDeviErrorCode::DetailError, "���豸ʧ�ܡ�\n");
	}
	else if (result == -2){
		printf("��ʼ���豸��ʧ�ܡ�\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDeviErrorCode::DetailError, "��ʼ���豸��ʧ�ܡ�\n");
	}
	else if (result != -3)
	{
		printf("����CAN�豸��ʧ�ܡ�\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDeviErrorCode::DetailError, "����CAN�豸��ʧ�ܡ�\n");
	}
}

void closeCAN(char * resultString)
{
	GET_T->closeCanDev();
	sprintf_s(resultString, 256, "%s,F2,%d,%s", S2C, enCANDeviErrorCode::Success, "CAN�豸�Ѿ��رա�\n");
}

void readOrWriteCANID(VT_STR vtStrCommand, char* resultString)
{
	if (vtStrCommand[2].compare("R") == 0)
	{
		//��ȡcan id
		stCAN_DevData dataObj;
		GET_P->getCommandReadCanId(dataObj);
		iResult = GET_T->sendCanData(dataObj, Uint8ToUint16(g_CAN_ID_Common));
		if (iResult == -1)
		{
			sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDeviErrorCode::DetailError, "CAN�豸δ��.\n");
		}
	}
	else if (vtStrCommand[2].compare("W") == 0)
	{
		if (isGoodCANID(vtStrCommand[3].c_str() )){

			sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDeviErrorCode::Success, "д��CAN_ID�ɹ�.\n");
		}
		else{
			sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDeviErrorCode::DetailError, "д��CAN_ID����CAN_ID�����Ϲ��(3��4λʮ�����������磺201 ��7FE).\n");
		}
	}
}
