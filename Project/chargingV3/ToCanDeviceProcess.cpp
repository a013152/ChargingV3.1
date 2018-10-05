#include "charging.h"
#include "CommandQueue.h"
#include "CommonFunction.h"
#include "CanProcess.h"

#pragma execution_character_set("utf-8")

static char szPrintf[256] = { 0 };

//�򿪻��߹ر�CAN�豸
void charging::onOpenCanDevice(bool checked)
{
	if (checked)	//�� 
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
				//��can �豸�ɹ���ѭ����֤����can�������
				for (auto itCharger : m_mapCharger)
				{
					if (itCharger.second.chargerType == DJI_Charger){
						char count_ = 0; int wLen = 0;
						sprintf_s(sztemp, 256, "%s,%s,%d", C2S, "F4", itCharger.second.id);
						while (count_++ < 2)  //���ѭ��2��
						{
							wLen = GET_CAN->sendToCanDeviceProcess(sztemp, 256, szPrintf);
							if (wLen == 0)
							{
								printfDebugInfo(QString::fromLocal8Bit("�����̣�CAN����δӦ��") , enDebugInfoPriority::DebugInfoLevelOne, true);
								break;
							}
							if (wLen != 0){
								Sleep(200);
								wLen = GET_CAN->receiveFromCanDeviceProcess(szReceive, szPrintf);
								printfDebugInfo(QString::fromLocal8Bit(szPrintf), enDebugInfoPriority::DebugInfoLevelOne, wLen == 0);
								QString strReceiv = QString::fromLocal8Bit(szReceive);
								QStringList strReceiveList = strReceiv.split(",");
								if (strReceiveList.size() > 4){
									if (strReceiveList[3] == "0")
									{
										//����ui����״̬
										itCharger.second.bOnline = true;
										MAP_LEVEL_IT itLevel = m_mapLevel.find(itCharger.second.nLevel);
										if (itLevel != m_mapLevel.end())
										{
											//�����λ���
											for (auto itBattery : itLevel->second.mapBattery)
											{
												int indexArray = batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery.second.id));
												charger_state[indexArray] = STATE_FREE;//"�����";
												emit RefreshState(enRefreshType::ChargerOnlineState, \
													batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery.second.id))); //��������״̬
											}
										}

										break;
									}
									else{
										//ʧ��
										if (strReceiveList[4] == "�յ����ݣ���"){
											printfDebugInfo(QString::fromLocal8Bit("CAN���̣�������֤����ȴ�Ӧ��ʱ��"), enDebugInfoPriority::DebugInfoLevelOne, true);
										}
										
									}
								}
							}
						}						

					}
				} 
			}
			
		}
		
	}
	else{  //�ر�
		if(GET_CAN->clossCanDeviceProcess(szPrintf))
			printfDebugInfo(QString(szPrintf), enDebugInfoPriority::DebugInfoLevelOne);
	}
	
}