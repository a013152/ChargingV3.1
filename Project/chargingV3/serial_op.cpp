#include "charging.h"
#pragma execution_character_set("utf-8")

static QString s_preStr;

QAction * charging::serial_scan(QAction * parentMenu)
{
	QAction * action = new QAction("关闭设备", m_menuCom);
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
		// 串口排序
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
		//创建串口菜单
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

//菜单 串口点击事件
void charging::OnClickMenuCom(QAction * action)
{
	QList<QAction*> listAction = m_menuCom->actions();
	for (int i = 0; i < listAction.size(); i++){
		listAction[i]->setChecked(false);
	}

	if (action->text().indexOf("关闭串口") >= 0)
	{
		listAction[0]->setChecked(true);
		if (SERIAL_PORT->isOpen())
			SERIAL_PORT->ClosePort();
		isOpenSerialPort = false;
		printfDebugInfo("关闭串口" + s_preStr, enDebugInfoPriority::DebugInfoLevelOne); 
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
			printfDebugInfo("打开串口" + s_preStr + "成功", enDebugInfoPriority::DebugInfoLevelOne);
			

			if (!meTimer->isActive())
				meTimer->start();
			if (m_bContinueScan)			
				printfDebugInfo("开始扫描设备！\n", enDebugInfoPriority::DebugInfoLevelOne);	
			beginScanBatteryState();  
		}
		else{ 
			printfDebugInfo("打开串口" + s_preStr + "失败", enDebugInfoPriority::DebugInfoLevelOne);
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

	//打开CAN设备进程                        
} 

QString temp_command;
//拼装命令，加入发送队列
void charging::toSend(QString strCommand, stCommand::enPriority enPriority)
{
	stCommand stComm(packageCommand(strCommand), enPriority);
	emit AddCommamdIntoQueue(stComm);
}

//组装命令
QString  charging :: packageCommand(QString command)
{
	//如果最后没有逗号，就加一个
	if (command.at(command.size() - 1) != QChar(','))
		command = command + ",";
	//加上包头 *NF
	if (command.indexOf("*NF,") < 0)
		command = "*NF," + command;
	
	//计算xor校验和
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

//测检收到的数据XOR
bool charging::detectRecvXOR(QString strContent)
{
	bool ret = false;
	strContent.indexOf("");

	//计算xor校验和
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
		
		if (xor == xor1) //匹配成功
			ret = true;
		 
	}
	
	return  ret;
}

//解析收到的串口数据
void charging::readSerial(QString type, QString strContent, int iError)
{
	int nPosEnd = strContent.lastIndexOf("\r\n");
	if (nPosEnd != -1)
		printfDebugInfo("I read: " + strContent.left(nPosEnd-2)+ "\\r\\n", enDebugInfoPriority::DebugInfoLevelThree);
	else
		printfDebugInfo("I read: " + strContent , enDebugInfoPriority::DebugInfoLevelThree);
	if (strContent.indexOf("*NF") == -1) //没有 *NF 不处理数据
		return;
	if (!detectRecvXOR(strContent))
	{
		printfDebugInfo("xor校验失败 接收内容"+ strContent  , enDebugInfoPriority::DebugInfoLevelThree, true); 
		return;
	}
	int loopA = -1, loopB = -1, nBatteryID = 0;
	QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间 
	QString strTime, strChargerState; strTime.sprintf("\n");
	strTime += time.toString("hh:mm:ss");//设置显示格式 		 
	if (type == QChar('G') && iError == ChargingClosetError::noError)  //处理 充电状态 温度 电池在线情况
	{		 
		//COperatorFile::GetInstance()->writeLog(strTime + " G 读:" + strContent); 
		QString strContent2 = strContent;
		strContent2.remove("*NF,Y,");
		int len1 = strContent2.indexOf(",");
		if (len1 != -1)
		{ 
			QString strID = strContent2.left(len1), strBatteryID, strTemperature, strBatteryExist,strBatteryChanged;
			QString strChargerStateInfo = get_back_message_at(strContent2, 2);
			strTemperature = get_back_message_at(strContent2, 3);  //温度
			strBatteryExist = get_back_message_at(strContent2, 4); //在位
			strBatteryChanged = get_back_message_at(strContent2, 5); //在位时间是否变化
			int nChargerId = strID.toInt();
			MAP_CLOSET_IT itCloset = m_mapCloset.find(nChargerId / 100);
			if (itCloset != m_mapCloset.end()){
				MAP_CHARGER_IT itCharger = itCloset->second.mapCharger.find(nChargerId);
				
				if (itCharger != itCloset->second.mapCharger.end())
				{ 
					nBatteryID = chargerIDtoBatteryId(nChargerId);
					int indexArray = batteryIDtoArrayIndex(QString::number(nBatteryID)); 
					//保存电池在线情况
					MAP_BATTERY_IT itBattery = itCloset->second.mapBattery.find(nBatteryID);
					MAP_BATTERY_MODEL_IT itBatteryMode = m_mapBatteryModel.find(itBattery->second.modelId);

					if (itBattery != itCloset->second.mapBattery.end()
						&& itBatteryMode != m_mapBatteryModel.end()
						&& itBatteryMode->second.balance == true )  //智能电池判断在位逻辑
					{
						battery_state[indexArray] = strBatteryExist == "1" ? "电池在线" : "未放置电池";
						itBattery->second.isExisted = strBatteryExist == "1" ? true : false;
						itBattery->second.isChanged = strBatteryChanged == "1" ? true : false;
						emit RefreshState(enRefreshType::BatteryState, indexArray);
					}
					
					//保存充电器在线状态 
					itCharger->second.bOnline = true;  
					//通知ui刷新充电器在线状态
					//emit RefreshState(enRefreshType::ChargerOnlineState, batteryIDtoArrayIndex(QString::number(itCharger->first)));
					//更新在线看门狗
					itCharger->second.nScanWatchDog = 0;
					//保存充电器 的 充电状态
					QString strChargerState = get_back_message_at(strContent2, 2);
					if (indexArray >= 0 && indexArray < charger_state.size()){
						//保存温度	判断是否过热
						float fTemp = itCharger->second.saveTemperature(strTemperature.toFloat());
						itCharger->second.isOverHeat =(fTemp >= m_fOverHeatTemperature) ? true : false;
						battery_temperature[indexArray].sprintf("%4.1f", fTemp);

						QString strChargerState = get_back_message_at(strContent2, 2);
						if (strChargerState == QChar('1'))//
							charger_state[indexArray] = STATE_FREE;//"充电器闲置";
						else if (strChargerState == QChar('2'))
							charger_state[indexArray] = STATE_CHARGING;// "充电中";
						else if (strChargerState == QChar('3'))
							charger_state[indexArray] = STATE_DISCHARGING;//"放电中"; 
						emit RefreshState(enRefreshType::ChargerState, indexArray);
					}
					if (itCharger->second.timeLockChargingState.elapsed() > 2000){
						itCharger->second.isCharging = (strChargerState == QChar('2'));
						itCharger->second.isDisCharging = (strChargerState == QChar('3')); 
					}  
				
					
					//正在充电
					if (itCharger->second.isCharging)
					{
						//如果正在充电 计算充电时间,超时触发停止充电 add 20180527
						int iChargingTime = itCharger->second.calculateChargeTime();
						if (itCharger->second.calculateChargeTime() >= m_nChargeLimitTime)
						{
							int nBatteryID = chargerIDtoBatteryId(itCharger->first);
							if (nBatteryID != -1){
								QString strContent = QString::number(nBatteryID) + "电池关联" + \
									QString::number(itCharger->first) + "充电器,充电时间达到" + \
									QString::number(itCharger->second.calculateChargeTime()) + "分钟，触发停止";
								printfDebugInfo(strContent, enDebugInfoPriority::DebugInfoLevelOne);
								COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strContent + "\n");
								//更新UI 
								int indexArray = batteryIDtoArrayIndex(QString::number(nBatteryID));
								if (charger_state[indexArray] != STATE_FREE/*"充电器闲置"*/ \
									&&itBattery->second.isApplyCharging == false){
									//判断是否有申请充电锁UI,如果没被申请，或者被申请了时间超过了5秒，则刷新UI
									stApplyInfo item = battery_apply_charging[indexArray];
									bool flag1 = (item.bApply == false || (item.bApply == true && item.timeLockUI.elapsed() > 2000));
									bool flag2 = itBattery->second.timeLockUI.elapsed() > 2000;
									int elapsed1 = itBattery->second.timeLockUI.elapsed();
									if ((item.bApply == false || (item.bApply == true && item.timeLockUI.elapsed() > 2000)) \
										&& itBattery->second.timeLockUI.elapsed() > 2000){
										charger_state[indexArray] = STATE_FREE;//"充电器闲置";
										emit RefreshState(enRefreshType::ChargerState, indexArray);
									}
								}
								//充电超时则发送停止命令
								stopByLocalID(QString::number(nBatteryID));
							}							
						}
						//如果过热,停止充电
						else if (itCharger->second.isOverHeat){
							toSend("P," + QString::number(itCharger->second.id), stCommand::hight);
						} 
					} 
					//充电器在一下情况读取电压
					//	1：智能电池在位并且充电时
					//	2：非智能电池
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
	else if (type == QChar('D') && iError == ChargingClosetError::noError)//处理接收的电压、电流
	{
		//*NF, Y, 201, 6S, 0.00, 0.00, -, 
		//COperatorFile::GetInstance()->writeLog(strTime + " D 读:" + strContent); 
		QString strContent2 = strContent;		
		strContent2.remove("*NF,Y,");
		int len1 = strContent2.indexOf(",");		
		if (len1 != -1)
		{
			QString strID = strContent2.left(len1);
			int chargerId = strID.toInt();
			QString str;
			float vol = 0.0, curr = 0.0;  //电压电流
			MAP_CLOSET_IT itCloset = m_mapCloset.find(chargerId / 100);
			if (itCloset != m_mapCloset.end())
			{
				MAP_CHARGER_IT itCharger = itCloset->second.mapCharger.find(chargerId);
				if (itCharger != itCloset->second.mapCharger.end()){
					itCharger->second.bOnline = true; 
					//通知ui刷新充电器在线状态
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
						if (nCommlength >= 27 && nCommlength < 77)  {  // 短命令大于等于27，小于77 
							//智能电池 总电压除与电池结构数
							str = get_back_message_at(strContent2, 3);
							vol = str.toFloat();
							if (vol > 0 && iConnectType > 0)
								vol = vol / iConnectType;
							//电流
							str = get_back_message_at(strContent2, 4);
							curr = str.toFloat();

						}
						else if (nCommlength > 77)
						{//1代 非智能电池的通讯协议 长命令大于77
							//非智能  读取每一节电池电压相加后除电池结构数	
							vol = 0; curr = 0;
							for (int i = 3; i <= (iConnectType*2+2); i++){
								if (i % 2 == 1){
									//电压
									str = get_back_message_at(strContent2, i );
									vol += str.toFloat();
								}
								else{
									//电流
									str = get_back_message_at(strContent2, i);
									curr += str.toFloat();
								} 
							}							
							if (vol > 0 && iConnectType > 0)
								vol = vol / iConnectType;
							if (curr > 0 && iConnectType > 0)
								curr = curr / iConnectType;
						}
						
						//非智能电池根据电压大于3.1V判断电池是否存在							
						if (itBatteryModel->second.balance == false)
						{
							battery_state[indexArray] = vol > 3.1 ? "电池在线" : "未放置电池";
							itBattery->second.isExisted = vol > 3.1 ? true : false;
						}
						//智能电池电池关闭时，并且电池未更换，读取原来的电压
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
						//保存电压	电流	
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
	else if (type == QChar('O') && iError == ChargingClosetError::noError)  //处理 充电命令
	{
		//COperatorFile::GetInstance()->writeLog(strTime + " O 读:" + strContent);
		QString strContent2 = strContent;
		strContent2.remove("*NF,Y,");
		int len1 = strContent2.indexOf(",");
		if (len1 != -1)
		{
			QString strID = strContent2.left(len1); //充电器ID

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
						if (strChargerState == "1") //充电成功 记录充电标志，提交服务器
						{
							itCharger->second.bOnline = true;
							if (itCharger->second.timeLockChargingState.elapsed() > 2000){								
								itCharger->second.isCharging = (strChargerState == QChar('1'));//充电成功
							}
							battery_charging_record[indexArray] = true;
						}
					} 
				}
			}
		}		
	}
	else if (type == QChar('P') && iError == ChargingClosetError::noError)  //处理 停止命令
	{
		QString strContent2 = strContent;
		strContent2.remove("*NF,Y,");
		int len1 = strContent2.indexOf(",");
		if (len1 != -1)
		{
			QString strID = strContent2.left(len1); //充电器ID

			int nBatteryId = chargerIDtoBatteryId(strID.toInt());
			if (nBatteryId != -1){
				int indexArray = batteryIDtoArrayIndex(strID);
				if (indexArray != -1){					
					//停止充电后， 智能电池的充电电流强制设置为0a
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
	else if (type == QChar('T') && iError == ChargingClosetError::noError)  //处理 继电器闭合回路命令
	{
	}
	else if (type == QChar('M') && iError == ChargingClosetError::noError)  //处理 查询电池存在、回路闭合状态命令
	{
	}
}
int getCanDJIBattery(int CANID, int pos)
{	
	int temp1 = CANID / 100 * 100; //抹去个位十位数字
	if (temp1){
		temp1 += pos;
	}
	return temp1;
}
// 解析接收到的CAN的内容param 1 内容。
void charging::onReadCAN(QString strContent)
{
	QStringList strList = strContent.split(",");
	if (strList.size() > 3)
	{
		//分析命令类型
		if (strList[1].compare("F4") == 0)
		{
			//认证通过			
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
							//电池在位情况
							for (auto itBattery : itLevel->second.mapBattery)
							{
								//if (itBattery.second.timeLockUI.elapsed() > 5000)
								{
									int indexArray = batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery.second.id));
									charger_state[indexArray] = STATE_FREE;//"充电闲置";
									emit RefreshState(enRefreshType::ChargerOnlineState, \
										batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery.second.id))); //更新在线状态
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
					if (strList[3].toInt() == 0)//返回成功
					{
						itCharger->second.bOnline = true;
						itCharger->second.nScanWatchDog = 0; 
						
					}
					else if (strList[3].toInt() == 2) //超时
					{
						itCharger->second.bOnline = false;
					}
				}
				if (itLevel != m_mapLevel.end() && strList[3].toInt() == 0)//返回成功
				{
					itCharger = itLevel->second.mapCharger.find(CANID); itCharger->second.bOnline = true; itCharger->second.nScanWatchDog = 0;
					 
					//电池在位情况
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
						battery_state[indexArray] = itBattery->second.isExisted ? "电池在线" : "未放置电池";
						emit RefreshState(enRefreshType::BatteryState, \
							batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery->second.id))); //更新在线状态
						indexArray = batteryIDtoArrayIndex(itBattery->second.id);
						if (itBattery->second.isExisted == false && itBattery->second.timeLockUI.elapsed() > 5000){
							charger_state[indexArray] = STATE_FREE;//"电池不在线，更新充电器闲置";
						}
					}

					QVector<int> idTemp ; int pos_ = 0;
					//电池动态信息
					if (strList.size() >= 6)
					{						
						for (int i = 5; i < strList.size(); i++){
							QString strBattery = strList[i];
							QStringList strList2 = strBattery.split(" ");
							if (strList2.size() < 4)
								continue;
							//[0]位置 [1]状态 [2]电压 [3]温度
							//pos:%d state:%d vol:%3.1fV T:%3.1f
							
							int pos = strList2[0].toInt();
							idTemp.append(pos);
							int state = strList2[1].toInt(); //电池状态:	0x00 满电	0x01 充电中	0x02 放电中	0x03 静默
							float vol = strList2[2].toFloat();//电压
							float tem = strList2[3].toFloat();//温度
							//赋值给内存中的电池映射
							int nBatteryId = getCanDJIBattery(CANID, pos);
							itBattery = itLevel->second.mapBattery.find(nBatteryId);
							if (itBattery != itLevel->second.mapBattery.end()){
								//赋值
								int indexArray = batteryIDtoArrayIndex(itBattery->second.id);
								battery_voltage[indexArray].sprintf("%4.1f", vol);
								battery_temperature[indexArray].sprintf("%4.1f", tem);
								//battery_current[indexArray].sprintf("%4.2f",curr);
								emit RefreshState(enRefreshType::BatteryVol, indexArray);
								//itCharger->second.saveVoltage(vol);
								//itCharger->second.fCurrent = curr;
								emit RefreshState(enRefreshType::BatteryState, indexArray);
								if (itBattery->second.timeLockUI.elapsed() > 5000){
									if (state == 0 || state == 3)// 满电/静默
										charger_state[indexArray] = STATE_FREE;//"充电器闲置";
									else if (state == 1)
										charger_state[indexArray] = STATE_CHARGING;// "充电中";
									else if (state == 2)
										charger_state[indexArray] = STATE_DISCHARGING;//"放电中"; 
									emit RefreshState(enRefreshType::ChargerState, indexArray);
								}								
							}
						}
					} 
					////电池在位情况
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

							charger_state[indexArray] = STATE_FREE;//"充电闲置";
							emit RefreshState(enRefreshType::ChargerOnlineState, \
								batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery.second.id))); //更新在线状态
						}
					}
						 
					 
				}
			}
			
		}
		else if (strList[1].compare("F9") == 0)
		{
			//充电反馈
			int CANID = strList[2].toInt();
		}
		else if (strList[1].compare("F10") == 0)
		{
			//放电反馈
			int CANID = strList[2].toInt();
		}

	}
}
