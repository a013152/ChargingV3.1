#include "charging.h"
#pragma execution_character_set("utf-8")

static QString s_preStr;

QAction * charging::serial_scan(QAction * parentMenu)
{
	QAction * action = new QAction("�ر��豸", m_menuCom);
	action->setCheckable(true); action->setChecked(true);
	m_menuCom->addAction(action);

	QString temp = "", temp2, temp3, temp4; QAction * actionBase = NULL;
	char szCom[256] = { 0 }; //int iError = 0;
	m_iError = 0;
	SERIAL_PORT->getComInfo(szCom, m_iError);
	if (m_iError == 0){
	
		temp = QString("%1").arg(szCom);
		QStringList strlist = temp.split(",");
		for (int i = 0; i < strlist.size(); i++){
			if (strlist[i].isEmpty())
				strlist.removeAt(i);
		}
		// ��������
		for (int i = 0; i < strlist.size() - 1; i++){
			for (int j = 0; j < strlist.size() - 1 - i; j++){
				temp2 = strlist[j]; temp3 = strlist[j + 1];
				temp2.remove("COM"); temp3.remove("COM");
				if (temp2.toInt() > temp3.toInt()){
					temp4 = strlist[j];
					strlist[j] = strlist[j + 1];
					strlist[j + 1] = temp4;
				}
			}
		}
		//�������ڲ˵�
		for (int i = 0; i < strlist.size(); i++){
			if (strlist[i].isEmpty() == 0){
				QAction * action = new QAction(strlist[i], m_menuCom);  action->setCheckable(true);
				m_menuCom->addAction(action);
				actionBase = action;
			}
		}
		parentMenu->setMenu(m_menuCom);
	} 
	return actionBase;
	 
}

//�˵� ���ڵ���¼�
void charging::OnClickMenuCom(QAction * action)
{
	QList<QAction*> listAction = m_menuCom->actions();
	for (int i = 0; i < listAction.size(); i++){
		listAction[i]->setChecked(false);
	}

	if (action->text().indexOf("�رմ���") >= 0)
	{
		listAction[0]->setChecked(true);
		if (SERIAL_PORT->isOpen())
			SERIAL_PORT->ClosePort();
		isOpenSerialPort = false;
		printfDebugInfo("�رմ���" + s_preStr, enDebugInfoPriority::DebugInfoLevelOne); 
		s_preStr = ""; 
		return;
	}
	
	qDebug() << action->text();
	
	if (s_preStr != action->text() || !isOpenSerialPort)
	{
		s_preStr = action->text();
		if (SERIAL_PORT->isOpen())
			SERIAL_PORT->ClosePort(); 
		UINT portNo = action->text().remove("COM").toInt();		
		if (SERIAL_PORT->openPort(portNo))
		{
			isOpenSerialPort = true;	
			action->setChecked(true);
			//m_CommandQueue.init(&my_Serial , m_mapBattery.size()); 
			m_CommandQueue.init( m_mapBattery.size()); 
			printfDebugInfo("�򿪴���" + s_preStr + "�ɹ�", enDebugInfoPriority::DebugInfoLevelOne);
			

			if (!meTimer->isActive())
				meTimer->start();
			if (m_bContinueScan)			
				printfDebugInfo("��ʼɨ���豸��\n", enDebugInfoPriority::DebugInfoLevelOne);	
			beginScanBatteryState();  
		}
		else{ 
			printfDebugInfo("�򿪴���" + s_preStr + "ʧ��", enDebugInfoPriority::DebugInfoLevelOne);
			isOpenSerialPort = false;
		}
	} 
	if (!isOpenSerialPort)
	{
		for (int i = 0; i < listAction.size(); i++){
			if (i == 0)
				listAction[i]->setChecked(true);
			else
				listAction[i]->setChecked(false);
		}
	}

	//��CAN�豸����                        
} 

QString temp_command;
//ƴװ������뷢�Ͷ���
void charging::toSend(QString strCommand, stCommand::enPriority enPriority)
{
	stCommand stComm(packageCommand(strCommand), enPriority);
	emit AddCommamdIntoQueue(stComm);
}

//��װ����
QString  charging :: packageCommand(QString command)
{
	//������û�ж��ţ��ͼ�һ��
	if (command.at(command.size() - 1) != QChar(','))
		command = command + ",";
	//���ϰ�ͷ *NF
	if (command.indexOf("*NF,") < 0)
		command = "*NF," + command;
	
	//����xorУ���
	QByteArray myData(command.toLatin1());
	char xor;
	for (int i = 0; i < myData.size(); i++)
	{
		if (i == 0)
		{
			xor = myData.data()[0];
			continue;
		}
		xor = xor^myData[i];
	}
	if (xor == 0   )
		xor = 0x0b;

	command = command + xor;
	command = command + "," + (QString('\r')) + QString('\n');
	return command;
}

//����յ�������XOR
bool charging::detectRecvXOR(QString strContent)
{
	bool ret = false;
	strContent.indexOf("");

	//����xorУ���
	QByteArray myData(strContent.toLatin1());
	char xor = 0, xor1= 0;

	int pos = myData.indexOf("\r\n");
	if (pos != -1)
	{
		xor = myData[pos - 2];
		QByteArray myData2 = myData.left(pos - 2);
		
		for (int i = 0; i < myData2.size(); i++)
		{
			if (i == 0)
			{
				xor1 = myData2.data()[0];
				continue;
			}
			xor1 = xor1^myData2[i];
		}
		if (xor1 == 0)
			xor1 = 0x0b;
		
		if (xor == xor1) //ƥ��ɹ�
			ret = true;
		 
	}
	
	return  ret;
}

//�����յ��Ĵ�������
void charging::readSerial(QString type, QString strContent, int iError)
{
	int nPosEnd = strContent.lastIndexOf("\r\n");
	if (nPosEnd != -1)
		printfDebugInfo("I read: " + strContent.left(nPosEnd-2)+ "\\r\\n", enDebugInfoPriority::DebugInfoLevelThree);
	else
		printfDebugInfo("I read: " + strContent , enDebugInfoPriority::DebugInfoLevelThree);
	if (strContent.indexOf("*NF") == -1) //û�� *NF ����������
		return;
	if (!detectRecvXOR(strContent))
	{
		printfDebugInfo("xorУ��ʧ�� ��������"+ strContent  , enDebugInfoPriority::DebugInfoLevelThree, true); 
		return;
	}
	int loopA = -1, loopB = -1, nBatteryID = 0;
	QDateTime time = QDateTime::currentDateTime();//��ȡϵͳ���ڵ�ʱ�� 
	QString strTime, strChargerState; strTime.sprintf("\n");
	strTime += time.toString("hh:mm:ss");//������ʾ��ʽ 		 
	if (type == QChar('G') && iError == ChargingClosetError::noError)  //���� ���״̬ �¶� ����������
	{		 
		//COperatorFile::GetInstance()->writeLog(strTime + " G ��:" + strContent); 
		QString strContent2 = strContent;
		strContent2.remove("*NF,Y,");
		int len1 = strContent2.indexOf(",");
		if (len1 != -1)
		{ 
			QString strID = strContent2.left(len1), strBatteryID, strTemperature, strBatteryExist,strBatteryChanged;
			QString strChargerStateInfo = get_back_message_at(strContent2, 2);
			strTemperature = get_back_message_at(strContent2, 3);  //�¶�
			strBatteryExist = get_back_message_at(strContent2, 4); //��λ
			strBatteryChanged = get_back_message_at(strContent2, 5); //��λʱ���Ƿ�仯
			int nChargerId = strID.toInt();
			MAP_CLOSET_IT itCloset = m_mapCloset.find(nChargerId / 100);
			if (itCloset != m_mapCloset.end()){
				MAP_CHARGER_IT itCharger = itCloset->second.mapCharger.find(nChargerId);
				
				if (itCharger != itCloset->second.mapCharger.end())
				{ 
					nBatteryID = chargerIDtoBatteryId(nChargerId);
					int indexArray = batteryIDtoArrayIndex(QString::number(nBatteryID)); 
					//�������������
					MAP_BATTERY_IT itBattery = itCloset->second.mapBattery.find(nBatteryID);
					MAP_BATTERY_MODEL_IT itBatteryMode = m_mapBatteryModel.find(itBattery->second.modelId);

					if (itBattery != itCloset->second.mapBattery.end()
						&& itBatteryMode != m_mapBatteryModel.end()
						&& itBatteryMode->second.balance == true )  //���ܵ���ж���λ�߼�
					{
						battery_state[indexArray] = strBatteryExist == "1" ? "�������" : "δ���õ��";
						itBattery->second.isExisted = strBatteryExist == "1" ? true : false;
						itBattery->second.isChanged = strBatteryChanged == "1" ? true : false;
						emit RefreshState(enRefreshType::BatteryState, indexArray);
					}
					
					//������������״̬ 
					itCharger->second.bOnline = true;  
					//֪ͨuiˢ�³��������״̬
					//emit RefreshState(enRefreshType::ChargerOnlineState, batteryIDtoArrayIndex(QString::number(itCharger->first)));
					//�������߿��Ź�
					itCharger->second.nScanWatchDog = 0;
					//�������� �� ���״̬
					QString strChargerState = get_back_message_at(strContent2, 2);
					if (indexArray >= 0 && indexArray < charger_state.size()){
						//�����¶�	�ж��Ƿ����
						float fTemp = itCharger->second.saveTemperature(strTemperature.toFloat());
						itCharger->second.isOverHeat =(fTemp >= m_fOverHeatTemperature) ? true : false;
						battery_temperature[indexArray].sprintf("%4.1f", fTemp);

						QString strChargerState = get_back_message_at(strContent2, 2);
						if (strChargerState == QChar('1'))//
							charger_state[indexArray] = STATE_FREE;//"���������";
						else if (strChargerState == QChar('2'))
							charger_state[indexArray] = STATE_CHARGING;// "�����";
						else if (strChargerState == QChar('3'))
							charger_state[indexArray] = STATE_DISCHARGING;//"�ŵ���"; 
						emit RefreshState(enRefreshType::ChargerState, indexArray);
					}
					if (itCharger->second.timeLockChargingState.elapsed() > 2000){
						itCharger->second.isCharging = (strChargerState == QChar('2'));
						itCharger->second.isDisCharging = (strChargerState == QChar('3')); 
					}  
				
					
					//���ڳ��
					if (itCharger->second.isCharging)
					{
						//������ڳ�� ������ʱ��,��ʱ����ֹͣ��� add 20180527
						int iChargingTime = itCharger->second.calculateChargeTime();
						if (itCharger->second.calculateChargeTime() >= m_nChargeLimitTime)
						{
							int nBatteryID = chargerIDtoBatteryId(itCharger->first);
							if (nBatteryID != -1){
								QString strContent = QString::number(nBatteryID) + "��ع���" + \
									QString::number(itCharger->first) + "�����,���ʱ��ﵽ" + \
									QString::number(itCharger->second.calculateChargeTime()) + "���ӣ�����ֹͣ";
								printfDebugInfo(strContent, enDebugInfoPriority::DebugInfoLevelOne);
								COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strContent + "\n");
								//����UI 
								int indexArray = batteryIDtoArrayIndex(QString::number(nBatteryID));
								if (charger_state[indexArray] != STATE_FREE/*"���������"*/ \
									&&itBattery->second.isApplyCharging == false){
									//�ж��Ƿ�����������UI,���û�����룬���߱�������ʱ�䳬����5�룬��ˢ��UI
									stApplyInfo item = battery_apply_charging[indexArray];
									bool flag1 = (item.bApply == false || (item.bApply == true && item.timeLockUI.elapsed() > 2000));
									bool flag2 = itBattery->second.timeLockUI.elapsed() > 2000;
									int elapsed1 = itBattery->second.timeLockUI.elapsed();
									if ((item.bApply == false || (item.bApply == true && item.timeLockUI.elapsed() > 2000)) \
										&& itBattery->second.timeLockUI.elapsed() > 2000){
										charger_state[indexArray] = STATE_FREE;//"���������";
										emit RefreshState(enRefreshType::ChargerState, indexArray);
									}
								}
								//��糬ʱ����ֹͣ����
								stopByLocalID(QString::number(nBatteryID));
							}							
						}
						//�������,ֹͣ���
						else if (itCharger->second.isOverHeat){
							toSend("P," + QString::number(itCharger->second.id), stCommand::hight);
						} 
					} 
					//�������һ�������ȡ��ѹ
					//	1�����ܵ����λ���ҳ��ʱ
					//	2�������ܵ��
					bool readVol = false;
					if (itBatteryMode->second.balance == true
						&& itCharger->second.isCharging && itBattery->second.isExisted)
						readVol = true;
					if (itBatteryMode->second.balance == false)
						readVol = true;
					if (readVol){
						toSend("D," + QString::number(itCharger->second.id) + ",", stCommand::front);
					}
				}
			}
		}   
	}
	else if (type == QChar('D') && iError == ChargingClosetError::noError)//������յĵ�ѹ������
	{
		//*NF, Y, 201, 6S, 0.00, 0.00, -, 
		//COperatorFile::GetInstance()->writeLog(strTime + " D ��:" + strContent); 
		QString strContent2 = strContent;		
		strContent2.remove("*NF,Y,");
		int len1 = strContent2.indexOf(",");		
		if (len1 != -1)
		{
			QString strID = strContent2.left(len1);
			int chargerId = strID.toInt();
			QString str;
			float vol = 0.0, curr = 0.0;  //��ѹ����
			MAP_CLOSET_IT itCloset = m_mapCloset.find(chargerId / 100);
			if (itCloset != m_mapCloset.end())
			{
				MAP_CHARGER_IT itCharger = itCloset->second.mapCharger.find(chargerId);
				if (itCharger != itCloset->second.mapCharger.end()){
					itCharger->second.bOnline = true; 
					//֪ͨuiˢ�³��������״̬
					emit RefreshState(enRefreshType::ChargerOnlineState,\
						batteryIDtoArrayIndex(QString::number(itCharger->first)));
				}
				int indexArray = batteryIDtoArrayIndex(QString::number(chargerIDtoBatteryId(chargerId)));
				if (indexArray >= 0 && indexArray < battery_state.size())
				{									
					MAP_BATTERY_IT itBattery = itCloset->second.mapBattery.find(chargerIDtoBatteryId(chargerId));
					if (itBattery != itCloset->second.mapBattery.end()){
						MAP_BATTERY_MODEL_IT itBatteryModel = m_mapBatteryModel.find(itBattery->second.modelId);
						str = itBatteryModel->second.connectType;
						int iConnectType = str.left(str.indexOf("S")).toInt();
						int nCommlength = strContent.length();
						if (nCommlength >= 27 && nCommlength < 77)  {  // ��������ڵ���27��С��77 
							//���ܵ�� �ܵ�ѹ�����ؽṹ��
							str = get_back_message_at(strContent2, 3);
							vol = str.toFloat();
							if (vol > 0 && iConnectType > 0)
								vol = vol / iConnectType;
							//����
							str = get_back_message_at(strContent2, 4);
							curr = str.toFloat();

						}
						else if (nCommlength > 77)
						{//1�� �����ܵ�ص�ͨѶЭ�� ���������77
							//������  ��ȡÿһ�ڵ�ص�ѹ��Ӻ����ؽṹ��	
							vol = 0; curr = 0;
							for (int i = 3; i <= (iConnectType*2+2); i++){
								if (i % 2 == 1){
									//��ѹ
									str = get_back_message_at(strContent2, i );
									vol += str.toFloat();
								}
								else{
									//����
									str = get_back_message_at(strContent2, i);
									curr += str.toFloat();
								} 
							}							
							if (vol > 0 && iConnectType > 0)
								vol = vol / iConnectType;
							if (curr > 0 && iConnectType > 0)
								curr = curr / iConnectType;
						}
						
						//�����ܵ�ظ��ݵ�ѹ����3.1V�жϵ���Ƿ����							
						if (itBatteryModel->second.balance == false)
						{
							battery_state[indexArray] = vol > 3.1 ? "�������" : "δ���õ��";
							itBattery->second.isExisted = vol > 3.1 ? true : false;
						}
						//���ܵ�ص�عر�ʱ�����ҵ��δ��������ȡԭ���ĵ�ѹ
						else  
						{
							float volTemp = battery_voltage[indexArray].toFloat(); 
							float currTemp = battery_current[indexArray].toFloat();
							if (itBattery->second.isExisted 
								&& itBattery->second.isChanged == false
								&& (volTemp > 3.25 && volTemp <= 4.5)
								&& (vol < 3.25 ))
							{
								vol = volTemp;
							}
						}
						//�����ѹ	����	
						indexArray = batteryIDtoArrayIndex(itBattery->second.id);
						battery_voltage[indexArray].sprintf("%4.1f",vol);
						battery_current[indexArray].sprintf("%4.1f", curr);
						emit RefreshState(enRefreshType::BatteryVol, indexArray);
						itCharger->second.saveVoltage(vol);
						itCharger->second.fCurrent = curr;
						emit RefreshState(enRefreshType::BatteryState, indexArray);
					}
				}
			}
		}
	}
	else if (type == QChar('O') && iError == ChargingClosetError::noError)  //���� �������
	{
		//COperatorFile::GetInstance()->writeLog(strTime + " O ��:" + strContent);
		QString strContent2 = strContent;
		strContent2.remove("*NF,Y,");
		int len1 = strContent2.indexOf(",");
		if (len1 != -1)
		{
			QString strID = strContent2.left(len1); //�����ID

			int nBatteryId = chargerIDtoBatteryId(strID.toInt());
			if (nBatteryId != -1){
				int indexArray = batteryIDtoArrayIndex(strID);
				if (indexArray != -1)
				{
					QString strBatteryId = QString::number(nBatteryId);
					MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
					if (getBatteryIdRelatedInfo(strBatteryId, itCloset, itBattery, itBatteryModel, itCharger, itLevel))
					{
						QString strChargerState = get_back_message_at(strContent2, 2);
						if (strChargerState == "1") //���ɹ� ��¼����־���ύ������
						{
							itCharger->second.bOnline = true;
							if (itCharger->second.timeLockChargingState.elapsed() > 2000){								
								itCharger->second.isCharging = (strChargerState == QChar('1'));//���ɹ�
							}
							battery_charging_record[indexArray] = true;
						}
					} 
				}
			}
		}		
	}
	else if (type == QChar('P') && iError == ChargingClosetError::noError)  //���� ֹͣ����
	{
		QString strContent2 = strContent;
		strContent2.remove("*NF,Y,");
		int len1 = strContent2.indexOf(",");
		if (len1 != -1)
		{
			QString strID = strContent2.left(len1); //�����ID

			int nBatteryId = chargerIDtoBatteryId(strID.toInt());
			if (nBatteryId != -1){
				int indexArray = batteryIDtoArrayIndex(strID);
				if (indexArray != -1){					
					//ֹͣ���� ���ܵ�صĳ�����ǿ������Ϊ0a
					battery_current[indexArray] = QString("%1").arg(0);
					MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
					if (getBatteryIdRelatedInfo(QString::number(nBatteryId), itCloset, itBattery, itBatteryModel, itCharger,itLevel))
					{
						if(itBatteryModel->second.balance == true)
							itCharger->second.fCurrent = 0;
					}					
				}
			}
		}
	}
	else if (type == QChar('T') && iError == ChargingClosetError::noError)  //���� �̵����պϻ�·����
	{
	}
	else if (type == QChar('M') && iError == ChargingClosetError::noError)  //���� ��ѯ��ش��ڡ���·�պ�״̬����
	{
	}
}
int getCanDJIBattery(int CANID, int pos)
{	
	int temp1 = CANID / 100 * 100; //Ĩȥ��λʮλ����
	if (temp1){
		temp1 += pos;
	}
	return temp1;
}
// �������յ���CAN������param 1 ���ݡ�
void charging::onReadCAN(QString strContent)
{
	QStringList strList = strContent.split(",");
	if (strList.size() > 3)
	{
		//������������
		if (strList[1].compare("F4") == 0)
		{
			//��֤ͨ��			
			MAP_CLOSET_IT itCloset;; MAP_CHARGER_IT itCharger;
			QVector<stCommand> vtStCommand;	itCloset = m_mapCloset.find(1);
			if (itCloset != m_mapCloset.end())
			{
				itCharger = itCloset->second.mapCharger.find(strList[2].toInt());
				if (itCharger != itCloset->second.mapCharger.end())
				{
					if (strList[3].toInt() == 0){

						itCharger->second.bOnline = true;
						MAP_LEVEL_IT itLevel = m_mapLevel.find(itCharger->second.nLevel);
						if (itLevel != m_mapLevel.end())
						{
							//�����λ���
							for (auto itBattery : itLevel->second.mapBattery)
							{
								//if (itBattery.second.timeLockUI.elapsed() > 5000)
								{
									int indexArray = batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery.second.id));
									charger_state[indexArray] = STATE_FREE;//"�������";
									emit RefreshState(enRefreshType::ChargerOnlineState, \
										batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery.second.id))); //��������״̬
								} 
							}
						}
					}					
				}
			}
		}
		else if (strList[1].compare("F8") == 0){
			//CAN ID
			int CANID = strList[2].toInt();	
			MAP_CLOSET_IT itCloset; itCloset = m_mapCloset.find(1);
			if (itCloset != m_mapCloset.end()  )  
			{ 
				MAP_CHARGER_IT itCharger = itCloset->second.mapCharger.find(CANID); MAP_LEVEL_IT itLevel;
				if (itCharger != itCloset->second.mapCharger.end())
				{
					itLevel = m_mapLevel.find(itCharger->second.nLevel);
					if (strList[3].toInt() == 0)//���سɹ�
					{
						itCharger->second.bOnline = true;
						itCharger->second.nScanWatchDog = 0; 
						
					}
					else if (strList[3].toInt() == 2) //��ʱ
					{
						itCharger->second.bOnline = false;
					}
				}
				if (itLevel != m_mapLevel.end() && strList[3].toInt() == 0)//���سɹ�
				{
					itCharger = itLevel->second.mapCharger.find(CANID); itCharger->second.bOnline = true; itCharger->second.nScanWatchDog = 0;
					 
					//�����λ���
					QString strOnline = strList[4];
					MAP_BATTERY_IT itBattery; int i;
					MAP_CLOSET_IT itCloset2;	MAP_BATTERY_IT itBattery2; MAP_BATTERY_MODEL_IT itBatteryModel2; MAP_CHARGER_IT itCharger2; MAP_LEVEL_IT itLevel2;
					
					for (itBattery = itLevel->second.mapBattery.begin(), i = 0;
						itBattery != itLevel->second.mapBattery.end() && i < 15; itBattery++, i++)
					{
						itBattery->second.isExisted = (strOnline[i] == QChar('1'));
						if (getBatteryIdRelatedInfo(QString::fromLocal8Bit(itBattery->second.id), itCloset2, itBattery2, itBatteryModel2, itCharger2, itLevel2))
							itBattery2->second.isExisted = (strOnline[i] == QChar('1'));
						int indexArray = batteryIDtoArrayIndex(QString::fromLocal8Bit((itBattery->second.id)));
						battery_state[indexArray] = itBattery->second.isExisted ? "�������" : "δ���õ��";
						emit RefreshState(enRefreshType::BatteryState, \
							batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery->second.id))); //��������״̬
						indexArray = batteryIDtoArrayIndex(itBattery->second.id);
						if (itBattery->second.isExisted == false && itBattery->second.timeLockUI.elapsed() > 5000){
							charger_state[indexArray] = STATE_FREE;//"��ز����ߣ����³��������";
						}
					}

					QVector<int> idTemp ; int pos_ = 0;
					//��ض�̬��Ϣ
					if (strList.size() >= 6)
					{						
						for (int i = 5; i < strList.size(); i++){
							QString strBattery = strList[i];
							QStringList strList2 = strBattery.split(" ");
							if (strList2.size() < 4)
								continue;
							//[0]λ�� [1]״̬ [2]��ѹ [3]�¶�
							//pos:%d state:%d vol:%3.1fV T:%3.1f
							
							int pos = strList2[0].toInt();
							idTemp.append(pos);
							int state = strList2[1].toInt(); //���״̬:	0x00 ����	0x01 �����	0x02 �ŵ���	0x03 ��Ĭ
							float vol = strList2[2].toFloat();//��ѹ
							float tem = strList2[3].toFloat();//�¶�
							//��ֵ���ڴ��еĵ��ӳ��
							int nBatteryId = getCanDJIBattery(CANID, pos);
							itBattery = itLevel->second.mapBattery.find(nBatteryId);
							if (itBattery != itLevel->second.mapBattery.end()){
								//��ֵ
								int indexArray = batteryIDtoArrayIndex(itBattery->second.id);
								battery_voltage[indexArray].sprintf("%4.1f", vol);
								battery_temperature[indexArray].sprintf("%4.1f", tem);
								//battery_current[indexArray].sprintf("%4.2f",curr);
								emit RefreshState(enRefreshType::BatteryVol, indexArray);
								//itCharger->second.saveVoltage(vol);
								//itCharger->second.fCurrent = curr;
								emit RefreshState(enRefreshType::BatteryState, indexArray);
								if (itBattery->second.timeLockUI.elapsed() > 5000){
									if (state == 0 || state == 3)// ����/��Ĭ
										charger_state[indexArray] = STATE_FREE;//"���������";
									else if (state == 1)
										charger_state[indexArray] = STATE_CHARGING;// "�����";
									else if (state == 2)
										charger_state[indexArray] = STATE_DISCHARGING;//"�ŵ���"; 
									emit RefreshState(enRefreshType::ChargerState, indexArray);
								}								
							}
						}
					} 
					////�����λ���
					for (auto itBattery : itLevel->second.mapBattery)
					{
						bool nextId = false;
						for (auto iD : idTemp)
						{
							if (itBattery.first == itBattery.second.relatedCharger/100*100 + iD)
							{
								nextId = true;
								break;
							}
						}
						if (nextId )
							continue;
						if (itBattery.second.timeLockUI.elapsed() > 5000){
							int indexArray = batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery.second.id));

							charger_state[indexArray] = STATE_FREE;//"�������";
							emit RefreshState(enRefreshType::ChargerOnlineState, \
								batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery.second.id))); //��������״̬
						}
					}
						 
					 
				}
			}
			
		}
		else if (strList[1].compare("F9") == 0)
		{
			//��練��
			int CANID = strList[2].toInt();
		}
		else if (strList[1].compare("F10") == 0)
		{
			//�ŵ練��
			int CANID = strList[2].toInt();
		}

	}
}
