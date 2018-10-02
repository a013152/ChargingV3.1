#include "charging.h"
#pragma execution_character_set("utf-8")
 
void charging::timer_out()
{	
	COperatorFile::GetInstance()->tryCreateLogFile(); 
	static QTime s_ScanQtime, s_ScanQtime2; 
	static bool firstScanQtime = true; if (firstScanQtime){ firstScanQtime = false;	  s_ScanQtime2.start(); }
		
	if (s_ScanQtime2.elapsed() >= m_ScanDeviceInterval){
		s_ScanQtime2.restart();
		addChargerScanTime();   //添加扫描次数，判断充电器是否在线
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
		if (s_Qtime.elapsed() >= m_SubmitInterval && m_bContinueSubmit)	//大于检测申请时间间隔 默认15秒
		{  
			detectServerBatteryState();//检测 服务器的电池申请情况	

			detectSubmitBatteryState(); //提交电池信息

			s_Qtime.restart();
		}
	} 
	else if (false == m_bConnectServerIsSeccuss && m_bContinueSubmit){
		//尝试开启线程测试网络连接情况 20180620
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
				//运行线程 测试连接服务器数据库
				m_ConnectDBThread.start(QThread::NormalPriority);
			}
		}
	}

	if (SERIAL_PORT->isOpen() == true)
	{
		processApplyBatteryToCharging();//处理：申请与充电/停止 
	}

	detectTextEdit();  //检测edit控件 文字是否过多

	detectHideDebugInfo(); //检测隐藏 调试信息

	releaseBatteryStateRefresh(); //释放刷新限制 
}
 
void charging::scanAllBatteryState()
{
	
}
//开始扫描
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

 //扫描单柜子的电池数据
void charging::scanOneBatteryState(unsigned int nClosetId, stCommand::enPriority enPriority)
{
	MAP_CLOSET_IT itCloset; ; MAP_CHARGER_IT itCharger;
	QVector<stCommand> vtStCommand;
	itCloset = m_mapCloset.find(nClosetId);
	if (itCloset != m_mapCloset.end())
	{  
		for (itCharger = itCloset->second.mapCharger.begin(); itCharger != itCloset->second.mapCharger.end(); )
		{
			stCommand stComm("");
			stComm.chargerType = itCharger->second.chargerType;
			if (itCharger->second.chargerType == NF_Charger){
				//G命令读取电池存在状态 
				stComm =stCommand(packageCommand("G," + QString::number(itCharger->second.id) + ","), enPriority);					
			}
			else{
				//大疆充电槽
				QString strCommad; strCommad.sprintf("C2S,F8,%d,R", itCharger->second.id);

				stComm = stCommand(strCommad, enPriority);

			}
			//判断最后一个充电器
			if (++itCharger == itCloset->second.mapCharger.end())
			{
				stComm.lastCommandFlag = true;
			}
			vtStCommand.append(stComm);
		}
		m_CommandQueue.addVtCommand(vtStCommand);
	} 
}
//判断提交到服务器 //队列发送累加大于电池数量触发提交
void charging::detectSubmitBatteryState()
{	
	//参数：id，状态，温度，电压，电流，是否开始充电
	//温度，电压，电流保留3位有效数字
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
		battery_charging_record[i] = false;  //重置充电记录 
	}
	// 提交停止充电的命令 add 20180524 
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
		QString str = "\r\n电池数据上传完成,耗时";
		str += QString::number(temp / 1000);
		str += "秒";
		str += QString::number(temp % 1000); str += "毫秒";//
		printfDebugInfo(str, enDebugInfoPriority::DebugInfoLevelOne);
	}
	else
		printfDebugInfo("\r\n电池数据上传失败", enDebugInfoPriority::DebugInfoLevelOne);

}
//检测申请电池
void charging::detectServerBatteryState()
{ 
	if (m_bConnectServerIsSeccuss){
		//获取服务器数据库的电池申请情况	 
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
					item.bApply = flag;  //保存申请情况
					item.iCount = 0;   //尝试充电次数清零
					battery_apply_charging[i] = item;
				}
			}
		} 

		//检测服务器电池状态 处理停止充电信息 add20180524
		battery_server_state = m_submitServer.msg_get_message_now(battery_my_id);
		for (int i = 0; i < battery_server_state.size(); i++){
			stApplyInfo item = battery_apply_charging[i];
			QString strMyId = battery_my_id[i];
			if (item.myID == strMyId.toInt())
			{
				QString strState = battery_server_state[i];
				if (strState == "取消" )  //收到“取消”字符串，表示取消充电
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
					if (findFlag == false) //之前取消的队列中没有找到当前的,才能添加到取消队列
					{
						item.bNeedStopCharge = true;
						battery_apply_charging[i] = item;
					}
				}
			}
		}
	}	
}
 
//处理申请电池 与 触发充电
void charging::processApplyBatteryToCharging()
{
	int iResult = 0; QString strId;
	for (int i = 0; i < battery_apply.size(); i++)
	{
		stApplyInfo item = battery_apply_charging[i];
		if (battery_apply[i] == true && item.bNeedCharge == true)  //被申请了,同时需要充电
		{
			item.bNeedCharge = false;
			battery_apply_charging[i] = item;

			strId = battery_local_id[i];
			int indexArray = batteryIDtoArrayIndex(strId);

			printfDebugInfo("远程申请" + strId + "电池，尝试充电", enDebugInfoPriority::DebugInfoLevelTwo);
			COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + "远程申请" + strId + "电池，尝试充电\n");

			if (detectChargingCondition(strId, &iResult,false)) //检测充电条件
			{ 
				if (chargingByLocalID(strId, &iResult))
				{
					//UI更新 
					charger_state[indexArray] = STATE_DISCHARGING;		//充电中
					emit RefreshState(enRefreshType::ChargerState, indexArray);	
					item.timeLockUI.restart();		//触发充电成功 ，则2秒内禁止刷新充电状态
					battery_apply_charging[i] = item;
				} 
			}
			else{
				//申请充电失败
				printfDebugInfo("远程申请" + strId + "电池，尝试充电失败。", enDebugInfoPriority::DebugInfoLevelTwo);
				COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss") + " 远程申请" + strId + "尝试充电失败。");
				//添加到充电队列
				//appendChargingQueue(strId);  
			}		 
		}

		//处理停止充电  add 20180523
		//添加放电处理  add 20180627
		if (item.bNeedStopCharge == true){
			item.bNeedStopCharge = false;
			battery_apply_charging[i] = item;
			strId = battery_local_id[i];
			 
			MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger;
			if (getBatteryIdRelatedInfo(strId, itCloset, itBattery, itBatteryModel, itCharger))
			{
				if (itCharger->second.bOnline)  
				{
					if (itCharger->second.isCharging || itCharger->second.isDisCharging)  //在充电或者放电
					{
						if (stopByLocalID(strId))
						{
							int indexArray = batteryIDtoArrayIndex(strId);
							battery_state_enable_refresh[indexArray] = false;
							itBattery->second.timeLockUI.restart();
							charger_state[indexArray] = STATE_FREE;//"充电器闲置";
							m_vtUiChargGrid[indexArray % MAX_BATTERY]->setChargerState(charger_state[indexArray]);
							printfDebugInfo("远程停止" + strId + "充放电", enDebugInfoPriority::DebugInfoLevelOne);
							COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + "远程停止" + strId + "充电\n");
							//取消预充
							removeChargingQueue(strId);
						}
					}
					else if (itBatteryModel->second.balance == false &&  //非智能 电池
						itCharger->second.isDisCharging == false )  //没放电
					{
						//非智能电池触发放电
						QString strId = QString::fromLocal8Bit(itBattery->second.id);
						QString strConnectType = itBatteryModel->second.connectType;//电池结构
						QString strStorageVol; strStorageVol.sprintf("%5.2f", itBatteryModel->second.storageVoltage);//存储电压
						toSend("Q," + QString::number(itCharger->second.id) + "," + strConnectType.left(2) + "," + strStorageVol, stCommand::hight);
						int indexArray = batteryIDtoArrayIndex(strId);
						battery_state_enable_refresh[indexArray] = false;
						charger_state[indexArray] = STATE_DISCHARGING;//"放电中";
						m_vtUiChargGrid[indexArray % MAX_BATTERY]->setChargerState(charger_state[indexArray]);
						printfDebugInfo(" 远程触发" + strId + "放电。", enDebugInfoPriority::DebugInfoLevelOne);
						COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ")
							+ " 远程触发" + strId + "放电\n");
					}
				}
			}

			battery_cancel_charging_info.append(item); //add 20180524
		}
	}

	//处理申请未充电的情况
	if (m_vtApplyDontCharge.size())
	{
		printfDebugInfo("处理预充队列", enDebugInfoPriority::DebugInfoLevelTwo);
		QString str;
		for (int i = m_vtApplyDontCharge.size() - 1, indexArray = 0; i >= 0; i--){
			str += " " + QString::number(m_vtApplyDontCharge[i].nBatteryID);
			MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger;
			if (getBatteryIdRelatedInfo(QString::number(m_vtApplyDontCharge[i].nBatteryID), itCloset, itBattery, itBatteryModel, itCharger))
			{
				itBattery->second.isApplyCharging = true;
				indexArray = batteryIDtoArrayIndex(QString::number(m_vtApplyDontCharge[i].nBatteryID));
				if (indexArray != -1){
					//UI更新 
					charger_state[indexArray] = STATE_APPLYCHARGING;  //预充
					emit RefreshState(enRefreshType::ChargerState, indexArray);
				}
			}
		}
		printfDebugInfo("预充队列信息:"+ str, enDebugInfoPriority::DebugInfoLevelThree);
		//COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss") + "充电队列信息:" + str);

	}
	for (int i = m_vtApplyDontCharge.size()-1; i >= 0; i--){
		//尝试充电
		strId = QString::number(m_vtApplyDontCharge[i].nBatteryID);
		if (chargingByLocalID(strId, &iResult, false) && iResult == 0)
		{			
			//充电成功， 删除之前申请未充的记录
			m_vtApplyDontCharge.remove(i);
			//UI更新
			int indexArray = batteryIDtoArrayIndex(strId); 
			charger_state[indexArray] = STATE_CHARGING; //"充电中";
			emit RefreshState(enRefreshType::ChargerState, indexArray); 
			stApplyInfo item = battery_apply_charging[indexArray];
			item.timeLockUI.restart();		//触发充电成功 ，则2秒内禁止刷新充电状态
		} 
	}

	//比较 申请未充电的备份 是否有不同，不同则跟新到本地文件	
	writeApplyChargingQueueToFile();
	
	

}

//显示所有数据信息
void charging::show_all_data_now()
{ 
	// 获取对应的电池是否平衡  智能 非智能
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
		return true; //默认返回 智能电池
	}; 
	 
	//寻找对应localid的电池
	float fTemperature = 0, ftemp = 0; bool bHighTemperature = false;
	QString strClosetUiNo = m_iCurrentCloset < 10 ? "0" + QString::number(m_iCurrentCloset) : QString::number(m_iCurrentCloset);
	if (battery_local_id.size() >= MAX_BATTERY){
		int indexArray = 0;//int indexArray = 0 +(m_iCurrentCloset - 1)* MAX_BATTERY;
		//获取内存位置
		for (auto it : battery_local_id){
			indexArray++;
			if (m_iCurrentCloset == it.left(2).toInt())
				break;
		}
		indexArray -= 1;
		for (; indexArray < battery_local_id.size();  indexArray++)
		{
			QString strClosetNo = battery_local_id[indexArray].left(2);
			if (strClosetNo == strClosetUiNo)  //当前柜子编号相同更新UI信息
			{ 				
				//显示状态、电压、电流、温度
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

//检测信息文本是否过长
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
	qDebug() << "clear TextEdit elaped:" << iElaped;
	if (s_Qtime.elapsed() >= 10 * 60 * 1000)	//10分钟删一次
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

void charging::detectHideDebugInfo()  //测检隐藏调试信息
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
//增加扫描计数，判断充电器是否在线
void charging::addChargerScanTime()
{  
	MAP_CLOSET_IT itCloset;  MAP_CHARGER_IT itCharger; 
	itCloset = m_mapCloset.find(m_iCurrentCloset);
	if (itCloset != m_mapCloset.end())
	{			
		for (itCharger = itCloset->second.mapCharger.begin(); \
			itCharger != itCloset->second.mapCharger.end(); itCharger++)
		{
			//增加扫描计数				
			itCharger->second.nScanWatchDog++;	
			if (itCharger->second.nScanWatchDog > 2)
			{
				//不在线判断。
				itCharger->second.bOnline = false;  //充电器不在线
				//itCharger->second.fTemperature = 0;
				//itCharger->second.fVoltage = 0;
				//itCharger->second.fCurrent = 0;
				if (itCharger->second.nScanWatchDog == 5)
				{					
					MAP_BATTERY_IT itBattery = itCloset->second.mapBattery.find(itCharger->first);
					if (itBattery != itCloset->second.mapBattery.end()){
						int indexArray = batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery->second.id));
						itBattery->second.timeLockUI.restart();
						
						charger_state[indexArray] = STATE_OFFLINE;//"充电器不在线";
				
						emit RefreshState(enRefreshType::ChargerState, indexArray);

						//通知ui刷新充电器在线状态						
						emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
						//电压
						battery_voltage[indexArray] = "0";
						emit RefreshState(enRefreshType::BatteryVol, indexArray);
						//电池状态
						battery_state[indexArray] = "未放置电池"; 
						emit RefreshState(enRefreshType::BatteryState, indexArray);
						//温度
						show_temperature(0); 
					}					
				}
			}
		} 
	}
}


//提交电池型号
void charging::submitBatteryModel(MAP_BATTERY& mapNew, MAP_BATTERY& mapBatteryOld)
{
	if (m_bConnectServerIsSeccuss == false)
		return;
	//比较两个电池map，对比是否修改电池型号 add20180418
	typedef std::map<unsigned int, unsigned int> MAP_B_C; MAP_B_C mapCompareBatteryM;  //电池对比列表
	for (MAP_BATTERY_IT itNew = mapNew.begin(); itNew != mapNew.end(); itNew++){
		if (mapBatteryOld.size() != 0){
			MAP_BATTERY_IT itOld = mapBatteryOld.find(atoi(itNew->second.id));
			if (itOld != mapBatteryOld.end()){
				if (itOld->second.modelId != itNew->second.modelId)  //型号是否修改
				{
					mapCompareBatteryM.insert(\
						MAP_B_C::value_type(itNew->second.dbid, itNew->second.modelId));
				}
			}
		}
		else
		{  //如果mapBatteryOld 是空，全部更新电池型号 //仅用于初始化软件
			mapCompareBatteryM.insert(\
				MAP_B_C::value_type(itNew->second.dbid, itNew->second.modelId));
		}
		
	}
	if (mapCompareBatteryM.empty() == false ){
		//更新修改的型号
		for (auto &itB : mapCompareBatteryM)  //对每个元素
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

