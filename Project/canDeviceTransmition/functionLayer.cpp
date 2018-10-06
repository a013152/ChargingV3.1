#include "stdafx.h"
#include "functionLayer.h"
#include <regex>
#include <sstream>

static int iResult = 0;
static BYTE szTempCanID[2] = { 0 };
static UINT uintTempCanID = 0;
//ƥ��CANID
bool isGoodCANID(const char* szCANID)
{ 
	char pattern_[] = "[0-9a-fA-F]{3,3}";
	return regex_match(szCANID, std::regex(pattern_));
}
bool decideCANID(VT_STR  vtStrCommand ,char* resultString)
{
	printf("�ж�CAN ID%s �Ƿ���Ϲ���", vtStrCommand[2].c_str());
	if (isGoodCANID(vtStrCommand[2].c_str()))
	{
		printf("->����\n");
		return true;
	}
	else{
		printf("->������\n");
		sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDevieErrorCode::DetailError, "��֤����CAN_ID�����Ϲ��(3λʮ�����������磺201 ��7FE).\n");
		return false;
	}
	return true;
}
//CANID �ַ�ת��ʮ��������
void str_to_hex(const std::string& str , BYTE * szCanId)
{
	std::string strTemp;
	if (str.size() != str.size() / 2 * 2)//���� + ��0��
		strTemp = std::string("0") + str;
	std::vector<BYTE> vec;
	std::stringstream ss;
	ss << std::hex;
	for (std::string::size_type ix = 0; ix != strTemp.size() / 2; ++ix) {
		int val = 0;
		ss << strTemp.substr(ix * 2, 2);
		ss >> val;
		vec.push_back(val);
		ss.clear();
	}
	szCanId[0] = vec[1];				//����CANID ע�⣬LSB��ǰ��MSB�ں�,(����Ҫ���� 0x234����CanID[0]=0x34��CanID[1]=0x02)
	szCanId[1] = vec[0];
	return ;
}
void openCAN(char * resultString)
{
	//���豸
	int result = GET_T->openCanDev();
	if (result == 0){
		printf("���豸�ɹ���\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDevieErrorCode::Success, "���豸�ɹ���\n");


	}
	else if (result == -1)
	{
		printf("���豸ʧ�ܡ�\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDevieErrorCode::DetailError, "���豸ʧ�ܡ�\n");
	}
	else if (result == -2){
		printf("��ʼ���豸��ʧ�ܡ�\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDevieErrorCode::DetailError, "��ʼ���豸��ʧ�ܡ�\n");
	}
	else if (result != -3)
	{
		printf("����CAN�豸��ʧ�ܡ�\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDevieErrorCode::DetailError, "����CAN�豸��ʧ�ܡ�\n");
	}
}

void closeCAN(char * resultString)
{
	GET_T->closeCanDev();
	sprintf_s(resultString, 256, "%s,F2,%d,%s", S2C, enCANDevieErrorCode::Success, "CAN�豸�Ѿ��رա�\n");
}

bool readOrWriteCANID(VT_STR vtStrCommand, char* resultString)
{
	if (vtStrCommand[2].compare("R") == 0)
	{
		//��ȡcan id
		stCAN_DevData dataObj;
		GET_P->getCommandReadCanId(dataObj);
		iResult = GET_T->sendCanData(dataObj, Uint8ToUint16(g_CAN_ID_Common));
		if (iResult == -1)
		{
			sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDevieErrorCode::DetailError, "CAN�豸δ��.\n");
		}
	}
	else if (vtStrCommand[2].compare("W") == 0)
	{
		printf("�ж�CAN ID%s �Ƿ���Ϲ���", vtStrCommand[3].c_str());
		if (isGoodCANID(vtStrCommand[3].c_str() )){
			printf("->����\n");
			sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDevieErrorCode::Success, "д��CAN_ID�ɹ�.\n");

			
			str_to_hex(vtStrCommand[3], szTempCanID);	
			g_CAN_ID_Default[0] = szTempCanID[0]; g_CAN_ID_Default[1] = szTempCanID[1];
			printf("ת�����CAN ID:%X%02X\n", szTempCanID[1], szTempCanID[0]);
			stCAN_DevData dataObj;
			GET_P->getCommandWriteCanId(dataObj, szTempCanID);
			iResult = GET_T->sendCanData(dataObj, Uint8ToUint16(g_CAN_ID_Common));
		}
		else{ 
			printf("->������\n");
			sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDevieErrorCode::DetailError, "д��CAN_ID����CAN_ID�����Ϲ��(3λʮ�����������磺201 ��7FE).\n");
			return false;
		}

	}
	return true;
}



bool verifyDevice(VT_STR vtStrCommand, char* resultString)
{
	if (false == decideCANID(vtStrCommand, resultString))
	{
		return false;
	}
	else
	{
		str_to_hex(vtStrCommand[2], szTempCanID);
		g_CAN_ID_Default[0] = szTempCanID[0]; g_CAN_ID_Default[1] = szTempCanID[1];
		stCAN_DevData dataObj;
		GET_P->getCommandVerify(dataObj);
		uintTempCanID = Uint8ToUint16(szTempCanID);
		uintTempCanID |= 0x400;   //v1.3��İ汾 ��֤֮������� can id��Ҫ����0x400
		printf("ת�����CAN ID:%04X\n",uintTempCanID);
		GET_T->sendCanData(dataObj, uintTempCanID);
	}	
	return true;
}

bool readOrWriteBeginMode(VT_STR vtStrCommand, char* resultString)
{
	if (false == decideCANID(vtStrCommand, resultString))
	{
		return false;
	}
	else
	{
		str_to_hex(vtStrCommand[2], szTempCanID);g_CAN_ID_Default[0] = szTempCanID[0]; g_CAN_ID_Default[1] = szTempCanID[1];
		stCAN_DevData dataObj;		
		bool bReadOrWrite = vtStrCommand[3].compare("R") == 0 ? false : true;
		BYTE mode = 0;
		if (bReadOrWrite)
			mode = vtStrCommand[4].compare("0x01") == 0 ? 0x01 : 0xff;
		printf("mode%02X\n", (BYTE)mode);
		GET_P->getCommandBeginMode(dataObj,  bReadOrWrite, mode);
		uintTempCanID = Uint8ToUint16(szTempCanID);
		uintTempCanID |= 0x400;   //v1.3��İ汾 ��֤֮������� can id��Ҫ����0x400
		printf("ת�����CAN ID:%04X\n", uintTempCanID);
		GET_T->sendCanData(dataObj, uintTempCanID);
	}
	return true;
}

bool readBatteryInfo(VT_STR vtStrCommand, char* resultString)
{
	if (false == decideCANID(vtStrCommand, resultString))
	{
		return false;
	}
	else
	{
		str_to_hex(vtStrCommand[2], szTempCanID); g_CAN_ID_Default[0] = szTempCanID[0]; g_CAN_ID_Default[1] = szTempCanID[1];
		stCAN_DevData dataObj;		
		GET_P->getCommandDynaData(dataObj);
		uintTempCanID = Uint8ToUint16(szTempCanID);
		uintTempCanID |= 0x400;   //v1.3��İ汾 ��֤֮������� can id��Ҫ����0x400
		printf("ת�����CAN ID:%04X\n", uintTempCanID);
		GET_T->sendCanData(dataObj, uintTempCanID);
	}
	return true;
}
//��ȡ���״̬
bool readOrWriteChargingState(VT_STR vtStrCommand, char* resultString)
{
	if (false == decideCANID(vtStrCommand, resultString))
	{
		return false;
	}
	else
	{
		str_to_hex(vtStrCommand[2], szTempCanID); g_CAN_ID_Default[0] = szTempCanID[0]; g_CAN_ID_Default[1] = szTempCanID[1];
		stCAN_DevData dataObj;
		if (vtStrCommand[3].compare("R") == 0){
			GET_P->getCommandCharge(dataObj, false, 0, false);
		}
		else if (vtStrCommand[3].compare("W") == 0){
			int posBattery = atoi(vtStrCommand[4].c_str());
			bool bCharge = atoi(vtStrCommand[5].c_str()) == 1;  //1���
			GET_P->getCommandCharge(dataObj, true, posBattery, bCharge);
		}
		
		uintTempCanID = Uint8ToUint16(szTempCanID);
		uintTempCanID |= 0x400;   //v1.3��İ汾 ��֤֮������� can id��Ҫ����0x400
		printf("ת�����CAN ID:%04X\n", uintTempCanID);
		GET_T->sendCanData(dataObj, uintTempCanID);
	}
	return true;
}

//д����״̬
bool writeChargingState(VT_STR vtStrCommand, char* resultString)
{
	if (false == decideCANID(vtStrCommand, resultString))
	{
		return false;
	}
	else
	{
		str_to_hex(vtStrCommand[2], szTempCanID); g_CAN_ID_Default[0] = szTempCanID[0]; g_CAN_ID_Default[1] = szTempCanID[1];
		stCAN_DevData dataObj;
		int posBattery = atoi(vtStrCommand[4].c_str()); 
		bool bCharge = atoi(vtStrCommand[5].c_str()) == 1;  //1���
		GET_P->getCommandCharge(dataObj, true, posBattery, bCharge);
		uintTempCanID = Uint8ToUint16(szTempCanID);
		uintTempCanID |= 0x400;   //v1.3��İ汾 ��֤֮������� can id��Ҫ����0x400
		printf("ת�����CAN ID:%04X\n", uintTempCanID);
		GET_T->sendCanData(dataObj, uintTempCanID);
	}
	return true;
}