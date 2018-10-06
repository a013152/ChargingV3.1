#include "stdafx.h"
#include "functionLayer.h"
#include <regex>
#include <sstream>

static int iResult = 0;
static BYTE szTempCanID[2] = { 0 };
static UINT uintTempCanID = 0;
//匹配CANID
bool isGoodCANID(const char* szCANID)
{ 
	char pattern_[] = "[0-9a-fA-F]{3,3}";
	return regex_match(szCANID, std::regex(pattern_));
}
bool decideCANID(VT_STR  vtStrCommand ,char* resultString)
{
	printf("判断CAN ID%s 是否符合规律", vtStrCommand[2].c_str());
	if (isGoodCANID(vtStrCommand[2].c_str()))
	{
		printf("->符合\n");
		return true;
	}
	else{
		printf("->不符合\n");
		sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDevieErrorCode::DetailError, "认证错误，CAN_ID不符合规格(3位十六进制数例如：201 、7FE).\n");
		return false;
	}
	return true;
}
//CANID 字符转换十六进制数
void str_to_hex(const std::string& str , BYTE * szCanId)
{
	std::string strTemp;
	if (str.size() != str.size() / 2 * 2)//奇数 + “0”
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
	szCanId[0] = vec[1];				//设置CANID 注意，LSB在前，MSB在后,(例如要设置 0x234，则CanID[0]=0x34，CanID[1]=0x02)
	szCanId[1] = vec[0];
	return ;
}
void openCAN(char * resultString)
{
	//打开设备
	int result = GET_T->openCanDev();
	if (result == 0){
		printf("打开设备成功。\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDevieErrorCode::Success, "打开设备成功。\n");


	}
	else if (result == -1)
	{
		printf("打开设备失败。\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDevieErrorCode::DetailError, "打开设备失败。\n");
	}
	else if (result == -2){
		printf("初始化设备库失败。\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDevieErrorCode::DetailError, "初始化设备库失败。\n");
	}
	else if (result != -3)
	{
		printf("启动CAN设备库失败。\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDevieErrorCode::DetailError, "启动CAN设备库失败。\n");
	}
}

void closeCAN(char * resultString)
{
	GET_T->closeCanDev();
	sprintf_s(resultString, 256, "%s,F2,%d,%s", S2C, enCANDevieErrorCode::Success, "CAN设备已经关闭。\n");
}

bool readOrWriteCANID(VT_STR vtStrCommand, char* resultString)
{
	if (vtStrCommand[2].compare("R") == 0)
	{
		//读取can id
		stCAN_DevData dataObj;
		GET_P->getCommandReadCanId(dataObj);
		iResult = GET_T->sendCanData(dataObj, Uint8ToUint16(g_CAN_ID_Common));
		if (iResult == -1)
		{
			sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDevieErrorCode::DetailError, "CAN设备未打开.\n");
		}
	}
	else if (vtStrCommand[2].compare("W") == 0)
	{
		printf("判断CAN ID%s 是否符合规律", vtStrCommand[3].c_str());
		if (isGoodCANID(vtStrCommand[3].c_str() )){
			printf("->符合\n");
			sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDevieErrorCode::Success, "写入CAN_ID成功.\n");

			
			str_to_hex(vtStrCommand[3], szTempCanID);	
			g_CAN_ID_Default[0] = szTempCanID[0]; g_CAN_ID_Default[1] = szTempCanID[1];
			printf("转换后的CAN ID:%X%02X\n", szTempCanID[1], szTempCanID[0]);
			stCAN_DevData dataObj;
			GET_P->getCommandWriteCanId(dataObj, szTempCanID);
			iResult = GET_T->sendCanData(dataObj, Uint8ToUint16(g_CAN_ID_Common));
		}
		else{ 
			printf("->不符合\n");
			sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDevieErrorCode::DetailError, "写入CAN_ID错误，CAN_ID不符合规格(3位十六进制数例如：201 、7FE).\n");
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
		uintTempCanID |= 0x400;   //v1.3后的版本 认证之后的命令 can id需要或上0x400
		printf("转换后的CAN ID:%04X\n",uintTempCanID);
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
		uintTempCanID |= 0x400;   //v1.3后的版本 认证之后的命令 can id需要或上0x400
		printf("转换后的CAN ID:%04X\n", uintTempCanID);
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
		uintTempCanID |= 0x400;   //v1.3后的版本 认证之后的命令 can id需要或上0x400
		printf("转换后的CAN ID:%04X\n", uintTempCanID);
		GET_T->sendCanData(dataObj, uintTempCanID);
	}
	return true;
}
//读取充电状态
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
			bool bCharge = atoi(vtStrCommand[5].c_str()) == 1;  //1充电
			GET_P->getCommandCharge(dataObj, true, posBattery, bCharge);
		}
		
		uintTempCanID = Uint8ToUint16(szTempCanID);
		uintTempCanID |= 0x400;   //v1.3后的版本 认证之后的命令 can id需要或上0x400
		printf("转换后的CAN ID:%04X\n", uintTempCanID);
		GET_T->sendCanData(dataObj, uintTempCanID);
	}
	return true;
}

//写入充电状态
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
		bool bCharge = atoi(vtStrCommand[5].c_str()) == 1;  //1充电
		GET_P->getCommandCharge(dataObj, true, posBattery, bCharge);
		uintTempCanID = Uint8ToUint16(szTempCanID);
		uintTempCanID |= 0x400;   //v1.3后的版本 认证之后的命令 can id需要或上0x400
		printf("转换后的CAN ID:%04X\n", uintTempCanID);
		GET_T->sendCanData(dataObj, uintTempCanID);
	}
	return true;
}