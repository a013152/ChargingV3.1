#include "charging.h"
#include "CommandQueue.h"
#include "CommonFunction.h"
#include "CanProcess.h"

#pragma execution_character_set("utf-8")

static char szPrintf[MAX_BUF_SIZE] = { 0 };

void charging::onBtnCanDevice(bool check){
	onOpenOrCloseCanDevice(check);
}

//打开或者关闭CAN设备
void charging::onOpenOrCloseCanDevice(bool bOpenOrClose)
{
	if (bOpenOrClose)	//打开 
	{
		bool resultBool = GET_CAN->startCanDeviceProcess(szPrintf);
		printfDebugInfo(QString::fromLocal8Bit(szPrintf), enDebugInfoPriority::DebugInfoLevelOne, resultBool==false);

		::Sleep(100);
		char sztemp[256] = { 0 },szReceive[MAX_BUF_SIZE] = { 0 };
		sprintf_s(sztemp, 256, "%s,F1", C2S);
		if (GET_CAN->sendToCanDeviceProcess(sztemp, 256, szPrintf) == 0){
			printfDebugInfo(QString::fromLocal8Bit(szPrintf), enDebugInfoPriority::DebugInfoLevelOne, true);
		}
		else{
			::Sleep(100);
			if(0 == GET_CAN->receiveFromCanDeviceProcess(szReceive, szPrintf))
				printfDebugInfo(QString::fromLocal8Bit(szPrintf), enDebugInfoPriority::DebugInfoLevelOne, true);
			else{ 
				onReadCAN(QString::fromLocal8Bit(szPrintf));
			}			
		}		
	}
	else{  //关闭
		GET_CAN->clossCanDeviceProcess(szPrintf);
		isOpenCANProcess = false;
	 
	}
	
}