#include "stdafx.h"
#include "functionLayer.h"
#include <regex>
#include <sstream>

static int iResult = 0;
//匹配CANID
bool isGoodCANID(const char* szCANID)
{ 
	char pattern_[] = "[0-9a-fA-F]{3,4}";
	return regex_match(szCANID, std::regex(pattern_));
}
//CANID 字符转换十六进制数
std::vector<BYTE> str_to_hex(const std::string& str)
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
	return vec;
}
void openCAN(char * resultString)
{
	//打开设备
	int result = GET_T->openCanDev();
	if (result == 0){
		printf("打开设备成功。\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDeviErrorCode::Success, "打开设备成功。\n");


	}
	else if (result == -1)
	{
		printf("打开设备失败。\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDeviErrorCode::DetailError, "打开设备失败。\n");
	}
	else if (result == -2){
		printf("初始化设备库失败。\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDeviErrorCode::DetailError, "初始化设备库失败。\n");
	}
	else if (result != -3)
	{
		printf("启动CAN设备库失败。\n");
		sprintf_s(resultString, 256, "%s,F1,%d,%s", S2C, enCANDeviErrorCode::DetailError, "启动CAN设备库失败。\n");
	}
}

void closeCAN(char * resultString)
{
	GET_T->closeCanDev();
	sprintf_s(resultString, 256, "%s,F2,%d,%s", S2C, enCANDeviErrorCode::Success, "CAN设备已经关闭。\n");
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
			sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDeviErrorCode::DetailError, "CAN设备未打开.\n");
		}
	}
	else if (vtStrCommand[2].compare("W") == 0)
	{
		printf("判断CAN ID是否符合规律\n");
		if (isGoodCANID(vtStrCommand[3].c_str() )){
			printf("判断CAN ID是否符合规律->YSE\n");
			sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDeviErrorCode::Success, "写入CAN_ID成功.\n");

			BYTE szTempCanID[2] = { 0 };
			std::vector<BYTE> vt = str_to_hex(vtStrCommand[3]);
			szTempCanID[0] = vt[0];
			szTempCanID[1] = vt[1];
			printf("转换后的CAN ID:%02X %02X\n", szTempCanID[0], szTempCanID[1]);

			stCAN_DevData dataObj;
			GET_P->getCommandWriteCanId(dataObj, szTempCanID);
			iResult = GET_T->sendCanData(dataObj, Uint8ToUint16(g_CAN_ID_Common));
		}
		else{ 
			sprintf_s(resultString, 256, "%s,%s,%d,%s", S2C, vtStrCommand[1].c_str(), enCANDeviErrorCode::DetailError, "写入CAN_ID错误，CAN_ID不符合规格(3到4位十六进制数例如：201 、7FE).\n");
			return false;
		}

	}
	return true;
}
