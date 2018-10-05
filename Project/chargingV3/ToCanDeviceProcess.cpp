#include "charging.h"
#include "CommandQueue.h"
#include "CommonFunction.h"
#include "CanProcess.h"

#pragma execution_character_set("utf-8")

static char szPrintf[256] = { 0 };

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
				//打开can 设备成功，循环认证所以can充电器。
				m_menuItemCan->blockSignals(true);
				m_menuItemCan->setChecked(true);
				m_menuItemCan->blockSignals(false);
				//for (auto itCharger : m_mapCharger)
				//{
				//	if (itCharger.second.chargerType == DJI_Charger){
				//		char count_ = 0; int wLen = 0;
				//		sprintf_s(sztemp, 256, "C2S,F4,%d", itCharger.second.id);
				//		do  						
				//		{
				//			wLen = GET_CAN->sendToCanDeviceProcess(sztemp, 256, szPrintf);
				//			if (wLen == 0)
				//			{
				//				printfDebugInfo(QString::fromLocal8Bit("主进程：CAN进程未应答！") , enDebugInfoPriority::DebugInfoLevelOne, true);
				//				break;
				//			}
				//			if (wLen != 0){
				//				Sleep(200);
				//				wLen = GET_CAN->receiveFromCanDeviceProcess(szReceive, szPrintf);
				//				printfDebugInfo(QString::fromLocal8Bit(szPrintf), enDebugInfoPriority::DebugInfoLevelOne, wLen == 0);
				//				QString strReceiv = QString::fromLocal8Bit(szReceive);
				//				QStringList strReceiveList = strReceiv.split(",");
				//				if (strReceiveList.size() > 4){
				//					if (strReceiveList[3] == "0")
				//					{
				//						//更新ui在线状态
				//						itCharger.second.bOnline = true;
				//						MAP_LEVEL_IT itLevel = m_mapLevel.find(itCharger.second.nLevel);
				//						if (itLevel != m_mapLevel.end())
				//						{
				//							//电池在位情况
				//							for (auto itBattery : itLevel->second.mapBattery)
				//							{
				//								int indexArray = batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery.second.id));
				//								charger_state[indexArray] = STATE_FREE;//"在线闲置";
				//								emit RefreshState(enRefreshType::ChargerOnlineState, \
				//									batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery.second.id))); //更新在线状态
				//							}
				//						}
				//						break;
				//					}
				//					else{
				//						//失败
				//						if (strReceiveList[4] == "收到数据：空"){
				//							printfDebugInfo(QString::fromLocal8Bit("CAN进程：发送认证命令等待应答超时！"), enDebugInfoPriority::DebugInfoLevelOne, true);
				//						}										
				//					}
				//				}
				//			}
				//		} while (0);
				//	}
				//} 
			}			
		}		
	}
	else{  //关闭
		if(GET_CAN->clossCanDeviceProcess(szPrintf))
			printfDebugInfo(QString(szPrintf), enDebugInfoPriority::DebugInfoLevelOne);
	}
	
}