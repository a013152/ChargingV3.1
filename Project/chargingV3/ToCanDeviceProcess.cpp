#include "charging.h"
#include "CommandQueue.h"
#include "CommonFunction.h"
#include "CanProcess.h"

#pragma execution_character_set("utf-8")

static char szPrintf[256] = { 0 };

//打开或者关闭CAN设备
void charging::onOpenCanDevice(bool checked)
{
	if (checked)	//打开 
	{
		bool resultBool = GET_CAN->startCanDeviceProcess(szPrintf);
		printfDebugInfo(QString::fromLocal8Bit(szPrintf), enDebugInfoPriority::DebugInfoLevelOne, resultBool==false);

		::Sleep(100);
		char sztemp[256] = { 0 };
		sprintf_s(sztemp, 256, "%s,F1", C2S);
		if (GET_CAN->sendToCanDeviceProcess(sztemp, 256, szPrintf) == 0){
			printfDebugInfo(QString::fromLocal8Bit(szPrintf), enDebugInfoPriority::DebugInfoLevelOne, true);
		}
		else{
			::Sleep(100);
			char szReceive[MAX_BUF_SIZE] = { 0 };
			if(0== GET_CAN->receiveFromCanDeviceProcess(szReceive, szPrintf))
				printfDebugInfo(QString::fromLocal8Bit(szPrintf), enDebugInfoPriority::DebugInfoLevelOne, true);
			else{
				//打开can 设备成功，循环认证所以can充电器。
				for (auto itCharger : m_mapCharger)
				{
					if (itCharger.second.chargerType == DJI_Charger){
						char count_ = 0; int wLen = 0;
						sprintf_s(sztemp, 256, "%s,%s,%d", C2S, "F4", itCharger.second.id);
						while (wLen == 0)
						{
							wLen = GET_CAN->sendToCanDeviceProcess(sztemp, 256, szPrintf);
							if (count_++ > 2)
							{
								printfDebugInfo(QString::fromLocal8Bit(szPrintf) , enDebugInfoPriority::DebugInfoLevelOne, true);
								break;
							}
							Sleep(50);
						}
						if (wLen != 0){
							Sleep(200);
							wLen = GET_CAN->receiveFromCanDeviceProcess(szReceive, szPrintf);
							printfDebugInfo(QString::fromLocal8Bit(szPrintf), enDebugInfoPriority::DebugInfoLevelOne, wLen==0);
						}

					}
				} 
			}
			
		}
		
	}
	else{  //关闭
		if(GET_CAN->clossCanDeviceProcess(szPrintf))
			printfDebugInfo(QString(szPrintf), enDebugInfoPriority::DebugInfoLevelOne);
	}
	
}