#include "charging.h"
#include "CanProcess.h"
#pragma execution_character_set("utf-8")
 
void charging::timer_out()
{	
	COperatorFile::GetInstance()->tryCreateLogFile(); 
	static QTime s_ScanQtime, s_ScanQtime2; 
	static bool firstScanQtime = true; if (firstScanQtime){ firstScanQtime = false;	  s_ScanQtime2.start(); }
		
	if (s_ScanQtime2.elapsed() >= m_ScanDeviceInterval){
		s_ScanQtime2.restart();
		addChargerScanTime();   //���ɨ��������жϳ�����Ƿ�����

		detectChargeRecord();  //����ж���Ҫ��¼���/ֹͣ��� add 20181008
	}

	if (m_bConnectServerIsSeccuss && SERIAL_PORT->isOpen())
	{
		static QTime s_Qtime;
		static bool first = true;
		if (first){
			first = false;
			s_Qtime.start(); 
		}
		int iElaped = s_Qtime.elapsed();
		//qDebug() << "detect submit battery elaped:" << iElaped;
		if (s_Qtime.elapsed() >= m_SubmitInterval && m_bContinueSubmit)	//���ڼ������ʱ���� Ĭ��15��
		{  
			//detectServerBatteryState();//��� �������ĵ���������	

			//detectSubmitBatteryState(); //�ύ�����Ϣ
			m_ConnectDBThread.doDetectServerDB();

			s_Qtime.restart();
		}
		iElaped = s_Qtime.elapsed();
		qDebug() << "interval elaped:" << iElaped;
	} 
	else if (false == m_bConnectServerIsSeccuss && m_bContinueSubmit){
		//���Կ����̲߳�������������� 20180620
		if (m_ConnectDBThread.isFinished()){
			static QTime s_Qtime;
			static bool first = true;
			if (first){
				first = false;
				s_Qtime.start();
			}
			int iElaped = s_Qtime.elapsed();
			qDebug() << "reconnect server elaped:" << iElaped;
			if (s_Qtime.elapsed() >= m_SubmitInterval){
				//�����߳� �������ӷ��������ݿ�
				m_ConnectDBThread.start(QThread::NormalPriority);
			}
		}
	}

	if (SERIAL_PORT->isOpen() == true)
	{
		processApplyBatteryToCharging();//������������/ֹͣ 
	}

	detectTextEdit();  //���edit�ؼ� �����Ƿ����

	detectHideDebugInfo(); //������� ������Ϣ

	releaseBatteryStateRefresh(); //�ͷ�ˢ������ 
}
 
void charging::scanAllBatteryState()
{
	
}
//��ʼɨ��
void charging::beginScanBatteryState(bool reStart)
{
	static bool first = true;
	if (first){
		first = false;
		scanOneBatteryState(m_iCurrentCloset);
	}
	if (reStart && first == false){
		scanOneBatteryState(m_iCurrentCloset);
	}
}

 //ɨ�赥���ӵĵ������
void charging::scanOneBatteryState(unsigned int nClosetId, stCommand::enPriority enPriority)
{
	/*if (GET_CAN->isPreareSendOrRead()== false){
		char szPrintf[256] = { 0 };
		GET_CAN->startCanDeviceProcess(szPrintf);
	}*/


	MAP_CLOSET_IT itCloset; ; MAP_CHARGER_IT itCharger;
	QVector<stCommand> vtStCommand;
	itCloset = m_mapCloset.find(nClosetId);
	int nCount = 0;
	if (itCloset != m_mapCloset.end())
	{  
		
		for (itCharger = itCloset->second.mapCharger.begin(); itCharger != itCloset->second.mapCharger.end();itCharger++ )
		{
			stCommand stComm("", enPriority);
			stComm.chargerType = itCharger->second.chargerType;
			if (itCharger->second.chargerType == NF_Charger){
				//G�����ȡ��ش���״̬ 
				stComm.m_strCommand = packageCommand("G," + QString::number(itCharger->second.id) + ",");					
			}
			else if (itCharger->second.chargerType == DJI_Charger)
			{
				//�󽮳��۴����߼�
				if (GET_CAN->isPreareSendOrRead())  //�жϽ��̡�ͨ�����Ѿ�׼����
				{
					QString strCommad;
					if (itCharger->second.bOnline)
						strCommad.sprintf("C2S,F8,%d,R", itCharger->second.id);  //���ߵĻ����Ͳ�ѯ����
					else
						strCommad.sprintf("C2S,F4,%d", itCharger->second.id);    //�����ߵĻ�������֤����
					stComm.m_strCommand = strCommad;
				}		
			}
			//�ж����һ�������������ѭ�����ͱ�־
			if (++nCount == itCloset->second.mapCharger.size())
			{
				stComm.lastCommandFlag = true;
			}
			vtStCommand.append(stComm);
		}
		m_CommandQueue.addVtCommand(vtStCommand);
	} 
}
//�ж��ύ�������� //���з����ۼӴ��ڵ�����������ύ
void charging::detectSubmitBatteryState()
{	
	//������id��״̬���¶ȣ���ѹ���������Ƿ�ʼ���
	//�¶ȣ���ѹ����������3λ��Ч����
	QString my_id, state, temperature, voltage, current; bool bcharging = 0;
	for (int i = 0; i < battery_my_id.size(); i++)
	{
		my_id = battery_my_id[i];
		state = getBatteryState(i); //battery_state[i];
		temperature = battery_temperature[i];
		voltage = battery_voltage[i];
		current = battery_current[i];
		bcharging = battery_charging_record[i];
		m_submitServer.msg_put(my_id, state, temperature, voltage, current, bcharging);
		battery_charging_record[i] = false;  //���ó���¼ 
	}
	// �ύֹͣ�������� add 20180524 
	if (battery_cancel_charging_info.size() > 0){
		for (int i = battery_cancel_charging_info.size() - 1; i >= 0; i--)
		{
			m_submitServer.msg_put_message(QString::number(battery_cancel_charging_info[i].myID), " ");
			battery_cancel_charging_info.remove(i);
		}
	}

	QTime t = QTime::currentTime();
	t.start();
	if (m_submitServer.msg_put_now() == 0)
	{
		int temp = t.elapsed();
		QString str = "\r\n��������ϴ����,��ʱ";
		str += QString::number(temp / 1000);
		str += "��";
		str += QString::number(temp % 1000); str += "����";//
		//printfDebugInfo(str, enDebugInfoPriority::DebugInfoLevelOne);
		emit printfed(str);
	}
	else
	{	//printfDebugInfo("\r\n��������ϴ�ʧ��", enDebugInfoPriority::DebugInfoLevelOne);
		emit printfed("\r\n��������ϴ�ʧ��");
	}
}
//��������� ���������߳���ִ�У����ɲ���UI
void charging::detectServerBatteryState()
{ 

	if (m_bConnectServerIsSeccuss){
		//��ȡ���������ݿ�ĵ���������	 
		battery_apply = m_submitServer.msg_get_now(battery_my_id);
		for (int i = 0; i < battery_apply.size(); i++)
		{
			stApplyInfo item = battery_apply_charging[i];
			QString strMyId = battery_my_id[i];
			if (item.myID == strMyId.toInt())
			{
				bool flag = battery_apply[i];
				if (item.bApply != flag)
				{				
					if (item.bApply == false && flag == true)
						item.bNeedCharge = true;
					else
						item.bNeedCharge = false;
					item.bApply = flag;  //�����������
					item.iCount = 0;   //���Գ���������
					battery_apply_charging[i] = item;
				}
			}
		} 

		//�����������״̬ ����ֹͣ�����Ϣ add20180524
		battery_server_state = m_submitServer.msg_get_message_now(battery_my_id);
		for (int i = 0; i < battery_server_state.size(); i++){
			stApplyInfo item = battery_apply_charging[i];
			QString strMyId = battery_my_id[i];
			if (item.myID == strMyId.toInt())
			{
				QString strState = battery_server_state[i];
				if (strState == "ȡ��" )  //�յ���ȡ�����ַ�������ʾȡ�����
				{
					bool findFlag = false;
					for (auto it : battery_cancel_charging_info)
					{
						if (it.myID == item.myID)
						{
							findFlag = true;  
							break;
						}
					}
					if (findFlag == false) //֮ǰȡ���Ķ�����û���ҵ���ǰ��,������ӵ�ȡ������
					{
						item.bNeedStopCharge = true;
						battery_apply_charging[i] = item;
					}
				}
			}
		}
	}	
}
 
//���������� �� �������
void charging::processApplyBatteryToCharging()
{
	int iResult = 0; QString strId;
	for (int i = 0; i < battery_apply.size(); i++)
	{
		stApplyInfo item = battery_apply_charging[i];
		if (battery_apply[i] == true && item.bNeedCharge == true)  //��������,ͬʱ��Ҫ���
		{
			item.bNeedCharge = false;
			battery_apply_charging[i] = item;

			strId = battery_local_id[i];
			int indexArray = batteryIDtoArrayIndex(strId);

			printfDebugInfo("Զ������" + strId + "��أ����Գ��", enDebugInfoPriority::DebugInfoLevelTwo);
			COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + "Զ������" + strId + "��أ����Գ��\n");

			if (detectChargingCondition(strId, &iResult,false)) //���������
			{ 
				MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
				if (getBatteryIdRelatedInfo(strId, itCloset, itBattery, itBatteryModel, itCharger, itLevel))
				{
					if (itCharger->second.chargerType == NF_Charger){
						if (chargingByLocalID(strId, &iResult))
						{
							//UI���� 
							charger_state[indexArray] = STATE_DISCHARGING;		//�����
							emit RefreshState(enRefreshType::ChargerState, indexArray);
							item.timeLockUI.restart();		//�������ɹ� ����2���ڽ�ֹˢ�³��״̬
							battery_apply_charging[i] = item;
						}
					}else if (itCharger->second.chargerType == DJI_Charger)
					{				
						QVector<stCommand> vtStCommand;
						QString strCommad;
						strCommad.sprintf("C2S,F9,%d,R", itCharger->second.id);  //��ȡ���״̬����
						stCommand stCommR = stCommand(strCommad, stCommand::hight); stCommR.chargerType = DJI_Charger;
						vtStCommand.append(stCommR);
						strCommad.sprintf("C2S,F9,%d,W,%d,%d", itCharger->second.id, strId.toInt() % 100, 1);  //���ó��״̬����
						stCommand stCommW = stCommand(strCommad, stCommand::hight); stCommW.chargerType = DJI_Charger;
						vtStCommand.append(stCommW);
						m_CommandQueue.addVtCommand(vtStCommand);
					}
					
					itBattery->second.stRecord.beginChargeFlag = true;
					itBattery->second.stRecord.strRemrk = "Զ��������";
				}
				
			}
			else{
				//������ʧ��
				printfDebugInfo("Զ������" + strId + "��أ����Գ��ʧ�ܡ�", enDebugInfoPriority::DebugInfoLevelTwo);
				COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss") + " Զ������" + strId + "���Գ��ʧ�ܡ�");
				//��ӵ�������
				//appendChargingQueue(strId);  
			}		 
		}

		//����ֹͣ���  add 20180523
		//��ӷŵ紦��  add 20180627
		if (item.bNeedStopCharge == true){
			item.bNeedStopCharge = false;
			battery_apply_charging[i] = item;
			strId = battery_local_id[i];
			 
			MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
			if (getBatteryIdRelatedInfo(strId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
			{
				if (itCharger->second.bOnline)  
				{
					bool stopFlag = false;
					if (itCharger->second.chargerType == NF_Charger)
					{
						if (itCharger->second.isCharging || itCharger->second.isDisCharging)  //�ڳ����߷ŵ�
						{
							if (stopByLocalID(strId))
								stopFlag = true;

						}
						else if (itBatteryModel->second.balance == false &&  //������ ���
							itCharger->second.isDisCharging == false)  //û�ŵ�
						{
							//�����ܵ�ش����ŵ�
							QString strId = QString::fromLocal8Bit(itBattery->second.id);
							QString strConnectType = itBatteryModel->second.connectType;//��ؽṹ
							QString strStorageVol; strStorageVol.sprintf("%5.2f", itBatteryModel->second.storageVoltage);//�洢��ѹ
							toSend("Q," + QString::number(itCharger->second.id) + "," + strConnectType.left(2) + "," + strStorageVol, stCommand::hight);
							int indexArray = batteryIDtoArrayIndex(strId);
							battery_state_enable_refresh[indexArray] = false;
							charger_state[indexArray] = STATE_DISCHARGING;//"�ŵ���";
							m_vtUiChargGrid[indexArray % MAX_BATTERY]->setChargerState(charger_state[indexArray]);
							printfDebugInfo(" Զ�̴���" + strId + "�ŵ硣", enDebugInfoPriority::DebugInfoLevelOne);
							COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ")
								+ " Զ�̴���" + strId + "�ŵ�\n");
						}
					}
					else if (itCharger->second.chargerType == DJI_Charger)
					{
						if (itBattery->second.state == 1)//���ڳ��
						{
							//ֹͣ���
							QVector<stCommand> vtStCommand;	QString strCommad;
							strCommad.sprintf("C2S,F9,%d,R", itCharger->second.id);  //��ȡ���״̬����
							stCommand stCommR = stCommand(strCommad, stCommand::hight); stCommR.chargerType = DJI_Charger;
							vtStCommand.append(stCommR);
							strCommad.sprintf("C2S,F9,%d,W,%d,%d", itCharger->second.id, strId.toInt() % 100, 2);  //����ֹͣ���״̬����
							stCommand stCommW = stCommand(strCommad, stCommand::hight); stCommW.chargerType = DJI_Charger;
							vtStCommand.append(stCommW);
							m_CommandQueue.addVtCommand(vtStCommand);
							stopFlag = true;
						}
						else if (itBattery->second.state == 3 || itBattery->second.state == 0)
						{
							//Զ�̴����ŵ�
							//ƴװ ��ȡ�ŵ�״̬���� �������÷ŵ�״̬����
							QVector<stCommand> vtStCommand;
							QString strCommad;
							strCommad.sprintf("C2S,F10,%d,R", itCharger->second.id);  //��ȡ�ŵ�״̬����
							stCommand stCommR = stCommand(strCommad, stCommand::hight); stCommR.chargerType = DJI_Charger;
							vtStCommand.append(stCommR);
							strCommad.sprintf("C2S,F10,%d,W,%d,%d", itCharger->second.id, itBattery->first % 100, 1);  //���÷ŵ�״̬����
							stCommand stCommW = stCommand(strCommad, stCommand::hight); stCommW.chargerType = DJI_Charger;
							vtStCommand.append(stCommW);
							m_CommandQueue.addVtCommand(vtStCommand);

							//UI����
							int indexArray = batteryIDtoArrayIndex(strId);
							battery_state_enable_refresh[indexArray] = false;
							itBattery->second.timeLockUI.restart();
							itBattery = itLevel->second.mapBattery.find(itBattery->first);
							itBattery->second.timeLockUI.restart();

							printfDebugInfo(strId + "Զ�̴����ŵ�", enDebugInfoPriority::DebugInfoLevelOne);
							COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "Զ�̴����ŵ�\n");
 
							charger_state[indexArray] =  STATE_DISCHARGING;//"�ŵ���";
						}
						else if (itBattery->second.state == 2)
						{
							//ֹͣ�ŵ�
							//ƴװ ��ȡ�ŵ�״̬���� �������÷ŵ�״̬����
							QVector<stCommand> vtStCommand;
							QString strCommad;
							strCommad.sprintf("C2S,F10,%d,R", itCharger->second.id);  //��ȡ�ŵ�״̬����
							stCommand stCommR = stCommand(strCommad, stCommand::hight); stCommR.chargerType = DJI_Charger;
							vtStCommand.append(stCommR);
							strCommad.sprintf("C2S,F10,%d,W,%d,%d", itCharger->second.id, itBattery->first % 100, 0);  //���÷ŵ�״̬����
							stCommand stCommW = stCommand(strCommad, stCommand::hight); stCommW.chargerType = DJI_Charger;
							vtStCommand.append(stCommW);
							m_CommandQueue.addVtCommand(vtStCommand);

							//UI����
							int indexArray = batteryIDtoArrayIndex(strId);
							battery_state_enable_refresh[indexArray] = false;
							itBattery->second.timeLockUI.restart();
							itBattery = itLevel->second.mapBattery.find(itBattery->first);
							itBattery->second.timeLockUI.restart();
							charger_state[indexArray] = STATE_FREE;//����;

							printfDebugInfo(strId + "Զ��ֹͣ�ŵ�", enDebugInfoPriority::DebugInfoLevelOne);
							COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "Զ��ֹͣ�ŵ�\n");

							stopFlag = true;
						}
						
					}
					if (stopFlag){
						int indexArray = batteryIDtoArrayIndex(strId);
						battery_state_enable_refresh[indexArray] = false;
						itBattery->second.timeLockUI.restart();  //����ֹͣ��� ����2���ڽ�ֹˢ�³��״̬
						itBattery = itLevel->second.mapBattery.find(itBattery->first);
						itBattery->second.timeLockUI.restart();
						charger_state[indexArray] = STATE_FREE;//"���������";
						m_vtUiChargGrid[indexArray % MAX_BATTERY]->setChargerState(charger_state[indexArray]);
						printfDebugInfo("Զ��ֹͣ" + strId + "��ŵ�", enDebugInfoPriority::DebugInfoLevelOne);
						COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + "Զ��ֹͣ" + strId + "���\n");
						//ȡ��Ԥ��
						removeChargingQueue(strId);
					}
				}
			}
			battery_cancel_charging_info.append(item); //add 20180524
		}
	}

	//��������δ�������
	if (m_vtApplyDontCharge.size())
	{
		printfDebugInfo("����Ԥ�����", enDebugInfoPriority::DebugInfoLevelTwo);
		QString str;
		for (int i = m_vtApplyDontCharge.size() - 1, indexArray = 0; i >= 0; i--){
			str += " " + QString::number(m_vtApplyDontCharge[i].nBatteryID);
			MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
			if (getBatteryIdRelatedInfo(QString::number(m_vtApplyDontCharge[i].nBatteryID), itCloset, itBattery, itBatteryModel, itCharger,itLevel))
			{
				itBattery->second.isApplyCharging = true;
				indexArray = batteryIDtoArrayIndex(QString::number(m_vtApplyDontCharge[i].nBatteryID));
				if (indexArray != -1){
					//UI���� 
					charger_state[indexArray] = STATE_APPLYCHARGING;  //Ԥ��
					emit RefreshState(enRefreshType::ChargerState, indexArray);
				}
			}
		}
		printfDebugInfo("Ԥ�������Ϣ:"+ str, enDebugInfoPriority::DebugInfoLevelThree);
		//COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss") + "��������Ϣ:" + str);

	}
	for (int i = m_vtApplyDontCharge.size()-1; i >= 0; i--){
		//���Գ��
		strId = QString::number(m_vtApplyDontCharge[i].nBatteryID);
		if (chargingByLocalID(strId, &iResult, false) && iResult == 0)
		{			
			//���ɹ��� ɾ��֮ǰ����δ��ļ�¼
			m_vtApplyDontCharge.remove(i);
			//UI����
			int indexArray = batteryIDtoArrayIndex(strId); 
			charger_state[indexArray] = STATE_CHARGING; //"�����";
			emit RefreshState(enRefreshType::ChargerState, indexArray); 
			stApplyInfo item = battery_apply_charging[indexArray];
			item.timeLockUI.restart();		//���������� ����2���ڽ�ֹˢ�³��״̬
		} 
	}

	//�Ƚ� ����δ���ı��� �Ƿ��в�ͬ����ͬ����µ������ļ�	
	writeApplyChargingQueueToFile();
	
	

}

//��ʾ����������Ϣ
void charging::show_all_data_now()
{ 
	// ��ȡ��Ӧ�ĵ���Ƿ�ƽ��  ���� ������
	auto getBalence = [&](QString strId)->bool 
	{		  		
		MAP_BATTERY_IT it = m_mapBattery.find(strId.toInt());
		if (it != m_mapBattery.end())
		{
			MAP_BATTERY_MODEL_IT it2 = m_mapBatteryModel.find(it->second.modelId);
			if (it2 != m_mapBatteryModel.end())
			{
				bool balance = it2->second.balance;
				return balance;
			}
		}
		return true; //Ĭ�Ϸ��� ���ܵ��
	}; 
	 
	//Ѱ�Ҷ�Ӧlocalid�ĵ��
	float fTemperature = 0, ftemp = 0; bool bHighTemperature = false;
	QString strClosetUiNo = m_iCurrentCloset < 10 ? "0" + QString::number(m_iCurrentCloset) : QString::number(m_iCurrentCloset);
	if (battery_local_id.size() >= MAX_BATTERY){
		int indexArray = 0;//int indexArray = 0 +(m_iCurrentCloset - 1)* MAX_BATTERY;
		//��ȡ�ڴ�λ��
		for (auto it : battery_local_id){
			indexArray++;
			if (m_iCurrentCloset == it.left(2).toInt())
				break;
		}
		indexArray -= 1;
		for (; indexArray < battery_local_id.size();  indexArray++)
		{
			QString strClosetNo = battery_local_id[indexArray].left(2);
			if (strClosetNo == strClosetUiNo)  //��ǰ���ӱ����ͬ����UI��Ϣ
			{ 				
				//��ʾ״̬����ѹ���������¶�
				//qDebug() << battery_local_id[j]; 
				show_a_voltage(battery_voltage[indexArray], indexArray % MAX_BATTERY);
				//show_a_current(battery_current[j], i);
				//show_a_temperature(battery_temperature[j], i);
				ftemp = battery_temperature[indexArray].toFloat();
				if (ftemp >= 40)
				{
					bHighTemperature = true; 
					show_temperature(ftemp);
				}else
					fTemperature += battery_temperature[indexArray].toFloat();
				if (battery_state_enable_refresh[indexArray])
				{
					show_a_state(battery_state[indexArray], indexArray % MAX_BATTERY, getBalence(battery_local_id[indexArray]), charger_state[indexArray], battery_voltage[indexArray]);
					emit RefreshState(enRefreshType::ChargerOnlineState, batteryIDtoArrayIndex(battery_local_id[indexArray]));				
				} 
			}
		}
	}
	
	if (bHighTemperature == false){
		//fTemperature /= MAX_BATTERY; 
		//show_temperature(fTemperature);
	}
}

//�����Ϣ�ı��Ƿ����
void charging::detectTextEdit()
{
	//
	static QTime s_Qtime;
	static bool first = true;
	if (first){
		first = false;
		s_Qtime.start();
	}
	int iElaped = s_Qtime.elapsed();
	//qDebug() << "clear TextEdit elaped:" << iElaped;
	if (s_Qtime.elapsed() >= 10 * 60 * 1000)	//10����ɾһ��
	{
		QString str = m_TextEdit_DebugInfo->toPlainText(), str2;
		QStringList strlist = str.split("\n");	 
		int nCount = strlist.size();
		for (int i = 0; i < strlist.size(); i++){
			if (strlist.size() > 300)
				strlist.removeAt(0); 
		}
		for (int i = 0; i < strlist.size(); i++){
			str2 += strlist[i] + "\r\n";
		}
		
		m_TextEdit_DebugInfo->setText(str2);
		s_Qtime.restart();
	}

}

void charging::detectHideDebugInfo()  //������ص�����Ϣ
{
	if (m_bAutoHideDebugInfo == true && m_bShowDebugInfo == true){
		static bool s_b_hide = false;
		if (s_b_hide == false){
			s_b_hide = true;
		}
		else{
			s_b_hide = false;
			OnBtnHideDebugInfo();
		} 
	}
}
//����ɨ��������жϳ�����Ƿ�����
void charging::addChargerScanTime()
{  
	MAP_CLOSET_IT itCloset;  MAP_CHARGER_IT itCharger; 
	itCloset = m_mapCloset.find(m_iCurrentCloset);
	if (itCloset != m_mapCloset.end())
	{			
		for (itCharger = itCloset->second.mapCharger.begin(); \
			itCharger != itCloset->second.mapCharger.end(); itCharger++)
		{
			//����ɨ�����				
			itCharger->second.nScanWatchDog++;	
			if (itCharger->second.nScanWatchDog > 3)
			{
				//�������жϡ�
				itCharger->second.bOnline = false;  //�����������
				//itCharger->second.fTemperature = 0;
				//itCharger->second.fVoltage = 0;
				//itCharger->second.fCurrent = 0;
				if (itCharger->second.nScanWatchDog == 4)
				{					
					//����ui������״̬ 
					if (itCharger->second.chargerType == NF_Charger)
					{						
						//�����λ���						 
						{
							MAP_BATTERY_IT itBattery = itCloset->second.mapBattery.find(itCharger->second.id); //�ܷɳ���� һ���������Ӧһ�����
							int indexArray = batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery->second.id));
							charger_state[indexArray] = STATE_OFFLINE;//"�����������";
							itBattery->second.timeLockUI.restart();
							emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);  
							//��ѹ
							battery_voltage[indexArray] = "0.0";
							emit RefreshState(enRefreshType::BatteryVol, indexArray);
							//���״̬
							battery_state[indexArray] = "δ���õ��";
							emit RefreshState(enRefreshType::BatteryState, indexArray);
						}
							
					}
					else if (itCharger->second.chargerType == DJI_Charger)
					{
						MAP_LEVEL_IT itLevel = m_mapLevel.find(itCharger->second.nLevel);
						if (itLevel != m_mapLevel.end())
						{
							//�����λ���
							for (auto itBattery : itLevel->second.mapBattery)
							{
								int indexArray = batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery.second.id));
								charger_state[indexArray] = STATE_OFFLINE;//"�����������";
								itBattery.second.timeLockUI.restart();
								emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);

								//��ѹ
								battery_voltage[indexArray] = "0.0";
								emit RefreshState(enRefreshType::BatteryVol, indexArray);
								//���״̬
								battery_state[indexArray] = "δ���õ��";
								emit RefreshState(enRefreshType::BatteryState, indexArray);
							}
						}
					}
									
				}
			}
		} 
	}
}


//�ύ����ͺ�
void charging::submitBatteryModel(MAP_BATTERY& mapNew, MAP_BATTERY& mapBatteryOld)
{
	if (m_bConnectServerIsSeccuss == false)
		return;
	//�Ƚ��������map���Ա��Ƿ��޸ĵ���ͺ� add20180418
	typedef std::map<unsigned int, unsigned int> MAP_B_C; MAP_B_C mapCompareBatteryM;  //��ضԱ��б�
	for (MAP_BATTERY_IT itNew = mapNew.begin(); itNew != mapNew.end(); itNew++){
		if (mapBatteryOld.size() != 0){
			MAP_BATTERY_IT itOld = mapBatteryOld.find(atoi(itNew->second.id));
			if (itOld != mapBatteryOld.end()){
				if (itOld->second.modelId != itNew->second.modelId)  //�ͺ��Ƿ��޸�
				{
					mapCompareBatteryM.insert(\
						MAP_B_C::value_type(itNew->second.dbid, itNew->second.modelId));
				}
			}
		}
		else
		{  //���mapBatteryOld �ǿգ�ȫ�����µ���ͺ� //�����ڳ�ʼ�����
			mapCompareBatteryM.insert(\
				MAP_B_C::value_type(itNew->second.dbid, itNew->second.modelId));
		}
		
	}
	if (mapCompareBatteryM.empty() == false ){
		//�����޸ĵ��ͺ�
		for (auto &itB : mapCompareBatteryM)  //��ÿ��Ԫ��
		{
			MAP_BATTERY_MODEL_IT itM = m_mapBatteryModel.find(itB.second);
			if (itM != m_mapBatteryModel.end()){
				QString strModel = itM->second.droneModel;
				m_submitServer.msg_put2(QString::number(itB.first), strModel);
			}
		}
		m_submitServer.msg_put_now();
	}
}


//����ж���Ҫ���ݿ��¼���/ֹͣ���
void charging::detectChargeRecord()
{
	//if (GET_CAN->isPreareSendOrRead() )
	MAP_CLOSET_IT itCloset = m_mapCloset.find(1);
	for (MAP_BATTERY_IT itBattery = itCloset->second.mapBattery.begin(); 
		itBattery != itCloset->second.mapBattery.end(); itBattery++)
	{
		float fVol = battery_voltage[batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery->second.id))].toFloat();
		if (itBattery->second.stRecord.beginChargeFlag && fVol > 1.5)
		{
			//�����µĳ���¼
			m_OperDB.onAddChargedRecord(itBattery->first, 
				fVol, itBattery->second.stRecord.strRemrk, iError);
			itBattery->second.stRecord.beginTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
			itBattery->second.stRecord.beginChargeFlag = false;
			itBattery->second.stRecord.pendingEndFlag = true;
			itBattery->second.timeLockChargeRecord.restart();  //���¼�ʱ����ֹDJI��ض�ȡ����״̬3
		}
		else if (itBattery->second.stRecord.endChargeFlag)
		{
			//����ֹͣ����¼
			m_OperDB.onAddStopChargedRecord(itBattery->first,
				fVol, itBattery->second.stRecord.beginTime, iError);
			itBattery->second.stRecord.endChargeFlag = false;
			
		}
	}
}