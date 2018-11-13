#include "charging.h"
#include "CommandQueue.h"
#include "CanProcess.h"
#pragma execution_character_set("utf-8")
 


auto pSetQPushButtonChecked = [](QPushButton *pBtn, bool checked){
	if (checked)
	{
		pBtn->setStyleSheet("QPushButton{font-size:16px; color:white;"
			"border-image: url(" + QString(g_AppPath) + "/img/radioChecked_pressd.png);}"
			"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/radioChecked_pressd.png);}"
			"QPushButton:pressed{border-image: url(" + QString(g_AppPath) + "/img/radioChecked_pressd.png);}");
	}
	else
	{
		pBtn->setStyleSheet("QPushButton{font-size:16px; color:white;"
			"border-image: url(" + QString(g_AppPath) + "/img/radioChecked_bg.png);}"
			"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/radioChecked_hover.png);}"
			"QPushButton:pressed{border-image: url(" + QString(g_AppPath) + "/img/radioChecked_hover.png);}");
	}
};

//改变柜子编号
void charging::OnClickClosetNo()
{ 
	static int sCloset = 0;
	QPushButton * radioButton = (QPushButton *)QObject::sender();
	extractNumber(radioButton->text(), m_iCurrentCloset);
	//QString strClosetUINo = radioButton->text().right(2);
	//m_iCurrentCloset = strClosetUINo.toInt();
	if (sCloset != m_iCurrentCloset)
		sCloset = m_iCurrentCloset;
	else
		return;
	
	for (int i = 0; i <m_vtUiClosetRadio.size(); i++){
		if (m_vtUiClosetRadio[i] == radioButton)
			pSetQPushButtonChecked(m_vtUiClosetRadio[i], true);
		else
			pSetQPushButtonChecked(m_vtUiClosetRadio[i], false);

	}	
	
	refreshCurrentUI();

	show_all_data_now(); 
	
}
//上一页
void charging::OnClickPrePage()
{	
	if (m_iCurrentClosetPage - 1  >= 1){ 
		m_iCurrentClosetPage--;
		bool showFlag = false;
		 
		for (int i = 0; i < m_vtUiClosetRadio.size(); i++){
			showFlag = (i < m_iCurrentClosetPage * ONE_PAGE_CLOSET && i >= (m_iCurrentClosetPage - 1) * ONE_PAGE_CLOSET);
			m_vtUiClosetRadio[i]->setVisible(showFlag);			
		} 
	}	
}
//下一页
void charging::OnClickNextPage()
{	 
	int SumPage = m_mapCloset.size() / ONE_PAGE_CLOSET + (m_mapCloset.size() % ONE_PAGE_CLOSET > 0);
	bool flag3 = SumPage - m_iCurrentClosetPage >= 1;
	if (flag3)
	{ 
		m_iCurrentClosetPage++;
		bool showFlag = false;
		 
		for (int i = 0; i < m_vtUiClosetRadio.size(); i++){
			showFlag = (i < m_iCurrentClosetPage * ONE_PAGE_CLOSET && i >= (m_iCurrentClosetPage - 1) * ONE_PAGE_CLOSET);
			m_vtUiClosetRadio[i]->setVisible(showFlag);
		}		
	}
}

//转换电压 到 Pro
int  transfVol(QString strVol)
{
	float val = strVol.toFloat();
	int ret = 0;
	if (val <= 3.5)
		ret = 0;
	else if(val >= 4.35)
		ret = 99;
	else if (val > 3.5 && val < 4.35)
	{
		ret = (val-3.5) * 100 / (4.35-3.5);

	}
	return ret;

}

// 更新电压 int 是下标
void charging::show_a_voltage(QString message, int index)
{ 
//	if (m_vtUiChargGrid.size()>0 && index < m_vtUiChargGrid.size())
//		m_vtUiChargGrid[index]->setVol(message.toFloat());  	  //设置电压	
}

//转换电流 
int transfCurrent(QString strCur)
{
	// 暂时规定最大电流8A
	float val = strCur.toFloat();
	int ret = 0;
	if (val <= 0.0)
		ret = 0;
	else if (val >= 8)
		ret = 99;
	else if (val < 8 && val > 0.0)
		ret = val * 100 / 8;
	return ret;
}
void charging::show_a_current(QString message, int i)
{
	if (m_vtUiChargGrid.size()>0)
		m_vtUiChargGrid[i - 1]->setCurrent(message.toFloat()); 
}
 
//转换温度 -25°C 到125°C
int transfTemperature(QString strTemperature)
{
	int ret = 0;
	float val = strTemperature.toFloat();
	if (val <= -25)
		ret = 0;
	else if (val >= 125)
		ret = 100;
	else if (val <125 && val > -25)
		ret = val * 100 / 150;
	return ret;
}

void charging::show_a_temperature(QString message, int i)
{
	if (m_vtUiChargGrid.size()>0)
		m_vtUiChargGrid[i - 1]->setTemperature(message.toFloat()); 
} 

//状态
void charging::show_a_state(QString message, int indexArray, bool balence, QString chargerState, QString strVol)
{
	emit RefreshState(enRefreshType::BatteryState, indexArray);	
} 

//暂停扫描
void charging::onPauseScand(bool checked)
{
	checked;
	QAction * action = (QAction *)QObject::sender();
	if (/*SERIAL_PORT->isOpen()*/ isOpenSerialPort || isOpenCANProcess  ){
		m_bContinueScan = !action->isChecked() ;
		//m_bContinueScan 为 false 不继续扫描 
		m_CommandQueue.setPause(m_bContinueScan);
	}
	else{
		action->blockSignals(true);
		action->setChecked(!checked);
		action->blockSignals(false);
		QString str;
		if (isOpenSerialPort == false)
			str = "串口设备";
		if (isOpenCANProcess == false)
			str += (isOpenSerialPort ? "" : "+");
			str += "CAN设备";
		str += "未打开";
		printfDebugInfo(str, enDebugInfoPriority::DebugInfoLevelOne, true);
	}	
	CReadIniFile::getInstance()->writeProfileInfo("SET", "ContinueScan", QString("%1").arg(m_bContinueScan), QString(g_AppPath) + "\\set.ini", &iError);

}

//一键充电
void charging::onOneKeyCharger()
{
	MAP_CLOSET_IT itCloset2;	MAP_BATTERY_IT itBattery2, itBattery3; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
	QString strId; int iResult = 0;
	MAP_CLOSET_IT itCloset = m_mapCloset.find(1);
	for (MAP_BATTERY_IT itBattery = itCloset->second.mapBattery.begin(); itBattery != itCloset->second.mapBattery.end(); itBattery++)
	{
		strId = QString::fromLocal8Bit(itBattery->second.id); 
		//	continue;

		if (getBatteryIdRelatedInfo(strId, itCloset2, itBattery2, itBatteryModel, itCharger, itLevel))
		{
			if (itCharger->second.bOnline == false)
				continue;

			//添加充电器种类判断
			if (itCharger->second.chargerType == NF_Charger)
			{
				bool isCharging = itCharger->second.isCharging;
				if (!isCharging)
				{
					// 判断充电条件，获取对应的电池结构、充电电压、电流
					int iResult = 0;
					if (chargingByLocalID(strId, &iResult,false))
					{
						int indexArray = batteryIDtoArrayIndex(strId);
						battery_state_enable_refresh[indexArray] = false;
						itBattery2->second.timeLockUI.restart();
						itBattery2 = itLevel->second.mapBattery.find(itBattery2->first);
						itBattery2->second.timeLockUI.restart();

						charger_state[indexArray] = STATE_CHARGING;//"充电中";
						emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
						printfDebugInfo(" " + strId + "一键充电", enDebugInfoPriority::DebugInfoLevelOne);
						COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "一键充电\n");
						itBattery2->second.stRecord.beginChargeFlag = true;
						itBattery2->second.stRecord.strRemrk = "一键充电";
					}
				}				
			}
			else if (itCharger->second.chargerType == DJI_Charger)
			{
				
				if (itBattery2->second.isExisted == false || itBattery2->second.state != 3){  //如果电池不在位，或者不在静默状态，退出
					 continue;
				}

				//拼装 读取充电状态命令 ，再设置充电状态命令
				QVector<stCommand> vtStCommand;
				QString strCommad;
				strCommad.sprintf("C2S,F9,%d,R", itCharger->second.id);  //读取充电状态命令
				stCommand stCommR = stCommand(strCommad, stCommand::hight); stCommR.chargerType = DJI_Charger;
				vtStCommand.append(stCommR);
				strCommad.sprintf("C2S,F9,%d,W,%d,%d", itCharger->second.id, strId.toInt() % 100, 1);  //设置充电状态命令
				stCommand stCommW = stCommand(strCommad, stCommand::hight); stCommW.chargerType = DJI_Charger;
				vtStCommand.append(stCommW);
				m_CommandQueue.addVtCommand(vtStCommand);

				//处理ui
				int indexArray = batteryIDtoArrayIndex(strId);
				battery_state_enable_refresh[indexArray] = false;
				itBattery2->second.timeLockUI.restart();
				itBattery3 = itLevel->second.mapBattery.find(itBattery2->first); //这里更新锁ui时间
				itBattery3->second.timeLockUI.restart();

				if (itBattery2->second.state == 3){
					charger_state[indexArray] = STATE_CHARGING;//"充电中";
					emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
					printfDebugInfo(" " + strId + "一键充电", enDebugInfoPriority::DebugInfoLevelOne);
					COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "一键充电\n");
					 
					itBattery2->second.stRecord.beginChargeFlag = true;
					itBattery2->second.stRecord.strRemrk = "一键充电";
					 
				}
			}
		}
	}


}
//设置自动放电天数
void charging::setDischargeDay()
{
	MAP_CLOSET_IT itCloset = m_mapCloset.find(1);
	for (MAP_CHARGER_IT itCharger = itCloset->second.mapCharger.begin(); itCharger != itCloset->second.mapCharger.end(); itCharger++){
		if (itCharger->second.bOnline){
			if (itCharger->second.chargerType == NF_Charger){
				//
			}
			else if (itCharger->second.chargerType == DJI_Charger){
				//设置自动放电
				QVector<stCommand> vtStCommand;
				QString strCommad;
				strCommad.sprintf("C2S,F9,%d,R", itCharger->second.id);  //读取充电状态命令 //更新内存中的在线情况
				stCommand stCommR = stCommand(strCommad, stCommand::hight); stCommR.chargerType = DJI_Charger;
				vtStCommand.append(stCommR);
				strCommad.sprintf("C2S,F11,%d,W,%d", itCharger->second.id, m_nDischargeDay);  //拼接自动放电命令
				stCommand stCommW = stCommand(strCommad, stCommand::hight); stCommW.chargerType = DJI_Charger;
				vtStCommand.append(stCommW);
				m_CommandQueue.addVtCommand(vtStCommand);
			}
		}
	}
}

//暂停扫描
void charging::onPauseSubmit(bool checked)
{
	if (checked) 
		m_bContinueSubmit = false;// 不继续扫描   
	else 
		m_bContinueSubmit = true;// 继续扫描 

	CReadIniFile::getInstance()->writeProfileInfo("SET", "ContinueSubmit", QString("%1").arg(m_bContinueSubmit), QString(g_AppPath) + "\\set.ini", &iError);

	QString temp = (checked ? "暂停" : "继续");
	printfDebugInfo(temp + "网络传输", enDebugInfoPriority::DebugInfoLevelOne);
}
 
//打开配置窗口进程
void charging::onOpenOptionWin( )
{
	QString strExe, strPath, strTemp;
	
	if (m_pProcess == NULL)
		m_pProcess = new QProcess(this);

#if defined(QT_DEBUG)
	strPath = QString(g_AppPath) + "/" + CHARGINGOPTIONPROCESS;
	strExe = CHARGINGOPTIONPROCESS;
#   else
	strExe = CHARGINGOPTIONPROCESS;
#  endif

	// 判断进程是否存在
	QProcess tasklist;
	tasklist.start("tasklist",
		QStringList() << "/NH"<< "/FO" << "CSV"	<< "/FI" << QString("IMAGENAME eq %1").arg(strExe));
	tasklist.waitForFinished();
	QByteArray byteArrayOutput = tasklist.readAllStandardOutput();
	QString strOutput = byteArrayOutput;
	if (strOutput.startsWith(QString("\"%1").arg(strExe))) //如果存在进程，
	{
		QString strWin = SETING_WINDOW_TITLE;
		QByteArray ba = strWin.toLocal8Bit(); // must
		 
		char* sz = ba.data();
		SetWindowTop(sz);  //窗口置顶
		//QProcess taskkill;
		//taskkill.execute("taskkill -f -fi \"imagename eq " + strExe + "\" ");  //关闭进程
	}
	else{
		QStringList arguments;
		//arguments << byteArray;
		m_pProcess->startDetached(strExe, arguments); //分离启动进程
		//m_pProcess->execute(strPath);//阻塞启动进程
	} 
}
//打开数据窗口
void charging::onOpenDataDialog()
{
	m_dataDlg->show();
}


//显示提示框 0提示、1警告、2错误
void charging::showTipsMessagebox(int iType , QString strMsg)
{
	if (m_msgDlg != nullptr 
		&& m_msgDlg->isActiveWindow() == false   //提示框未显示
		&& m_bShowDebugInfo == false			//调试信息区域未显示
		){
		m_msgDlg->setMessageText(strMsg);
		m_msgDlg->setTypeShow(iType);
	}
}

//处理充电
void charging::OnBtnChargingOrStopCharging1()
{

	QPushButton* btn = qobject_cast<QPushButton*>(sender()); 
	ui_charg_grid *groupBox = (ui_charg_grid *)btn->parent(); 
	qDebug() << groupBox->title() + " clicked: " + (groupBox->getCharging() ? "Stop" : "Charging");
	QString strId = groupBox->title(); 

	 
	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
	if (getBatteryIdRelatedInfo(strId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
	{		
		//添加充电器种类判断
		if (itCharger->second.chargerType == NF_Charger){
			
			bool isCharging = itCharger->second.isCharging;
			if (!isCharging)
			{
				// 判断充电条件，获取对应的电池结构、充电电压、电流
				int iResult = 0;
				if (chargingByLocalID(strId, &iResult))
				{
					int indexArray = batteryIDtoArrayIndex(strId);
					battery_state_enable_refresh[indexArray] = false;
					itBattery->second.timeLockUI.restart();
					itBattery = itLevel->second.mapBattery.find(itBattery->first);
					itBattery->second.timeLockUI.restart();

					charger_state[indexArray] = STATE_CHARGING;//"充电中";
					emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
					printfDebugInfo(" " + strId + "手动充电", enDebugInfoPriority::DebugInfoLevelOne);
					COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "手动充电\n");
					MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
					if (getBatteryIdRelatedInfo(strId, itCloset, itBattery, itBatteryModel, itCharger, itLevel))
					{
						itBattery->second.stRecord.beginChargeFlag = true;
						itBattery->second.stRecord.strRemrk = "手动充电";
					}
				}
				else
				{
					//预充 
				}
			}
			else {
				//2 判断停止条件
				if (stopByLocalID(strId))
				{
					int indexArray = batteryIDtoArrayIndex(strId);
					battery_state_enable_refresh[indexArray] = false;
					itBattery->second.timeLockUI.restart();
					itBattery = itLevel->second.mapBattery.find(itBattery->first);
					itBattery->second.timeLockUI.restart();

					charger_state[indexArray] = STATE_FREE;//"充电器闲置"; 
					emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
					printfDebugInfo(strId + "手动停止", enDebugInfoPriority::DebugInfoLevelOne);
					COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "手动停止\n");
					//取消预充
					itBattery->second.isApplyCharging = false;
					removeChargingQueue(strId);
				}
			}
		}
		else if (itCharger->second.chargerType == DJI_Charger)
		{
			bool doCharging = false, doStop = false; //充电、放电、停止 标志
			int iResult = 0;
			//如果没有充电放电，触发充电，
			if (groupBox->getCharging() == false &&groupBox->getDisCharging() == false)
			{
				if (detectChargingCondition(QString::fromLocal8Bit(itBattery->second.id), &iResult))
				{
					doCharging = true;
					
					QVector<stCommand> vtStCommand;
					QString strCommad;
					strCommad.sprintf("C2S,F9,%d,R", itCharger->second.id);  //读取充电状态命令
					stCommand stCommR = stCommand(strCommad, stCommand::hight); stCommR.chargerType = DJI_Charger;
					vtStCommand.append(stCommR);
					strCommad.sprintf("C2S,F9,%d,W,%d,%d", itCharger->second.id, strId.toInt() % 100, 1);  //设置充电状态命令 1充电 2 停止 
					stCommand stCommW = stCommand(strCommad, stCommand::hight); stCommW.chargerType = DJI_Charger;
					vtStCommand.append(stCommW);
					m_CommandQueue.addVtCommand(vtStCommand);
				}
			}
			//如果正在充电，则停止
			if (groupBox->getCharging() )
			{
				doStop = true;				
				QVector<stCommand> vtStCommand;
				QString strCommad; 
				strCommad.sprintf("C2S,F9,%d,R", itCharger->second.id);  //读取充电状态命令
				stCommand stCommR = stCommand(strCommad, stCommand::hight); stCommR.chargerType = DJI_Charger;
				vtStCommand.append(stCommR);
				strCommad.sprintf("C2S,F9,%d,W,%d,%d", itCharger->second.id, strId.toInt() % 100, 2);  //设置充电状态命令 1充电 2 停止 
				stCommand stCommW = stCommand(strCommad, stCommand::hight); stCommW.chargerType = DJI_Charger;
				vtStCommand.append(stCommW);
				m_CommandQueue.addVtCommand(vtStCommand);
			}
			//如果正在放电，则停止
			if (groupBox->getDisCharging())
			{
				doStop = true;
				QVector<stCommand> vtStCommand;
				QString strCommad;
				strCommad.sprintf("C2S,F10,%d,R", itCharger->second.id);  //读取放电状态命令
				stCommand stCommR = stCommand(strCommad, stCommand::hight); stCommR.chargerType = DJI_Charger;
				vtStCommand.append(stCommR);
				strCommad.sprintf("C2S,F10,%d,W,%d,%d", itCharger->second.id, strId.toInt() % 100,  0 );  //设置放电状态命令
				stCommand stCommW = stCommand(strCommad, stCommand::hight); stCommW.chargerType = DJI_Charger;
				vtStCommand.append(stCommW);
				m_CommandQueue.addVtCommand(vtStCommand);
			}
			//判断电池是否在充电
			if (doCharging || doStop)
			{	 
				//处理ui
				int indexArray = batteryIDtoArrayIndex(strId);
				battery_state_enable_refresh[indexArray] = false;
				itBattery->second.timeLockUI.restart();
				itBattery = itLevel->second.mapBattery.find(itBattery->first);
				itBattery->second.timeLockUI.restart();

				if (doCharging)
				{
					charger_state[indexArray] = STATE_CHARGING;//"充电中";
					emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
					printfDebugInfo(" " + strId + "手动充电", enDebugInfoPriority::DebugInfoLevelOne);
					COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "手动充电\n");
					MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
					if (getBatteryIdRelatedInfo(strId, itCloset, itBattery, itBatteryModel, itCharger, itLevel))
					{
						itBattery->second.stRecord.beginChargeFlag = true;
						itBattery->second.stRecord.strRemrk = "手动充电";
					}
				}
				else if(doStop){
					charger_state[indexArray] = STATE_FREE;//"充电器闲置"; 
					emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
					printfDebugInfo(strId + "手动停止", enDebugInfoPriority::DebugInfoLevelOne);
					COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "手动停止\n");
					//取消预充
					itBattery->second.isApplyCharging = false;
					removeChargingQueue(strId);
				}
			}
		}
	}  	
}
//接收到放电
void charging::OnBtnDisChargingOrStop1()
{
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	ui_charg_grid *groupBox = (ui_charg_grid *)btn->parent();	 
	QString strId = groupBox->title();
	int iResult = 0;
	if (detectDisChargingCondition(strId, &iResult))
	{
		MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
		if (getBatteryIdRelatedInfo(strId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
		{
			if (itCharger->second.chargerType == NF_Charger){
				if (!(itCharger->second.isDisCharging))
				{
					QString strConnectType = itBatteryModel->second.connectType;//电池结构
					QString strStorageVol; strStorageVol.sprintf("%5.2f", itBatteryModel->second.storageVoltage);//存储电压
					toSend("Q," + QString::number(itCharger->second.id) + "," + strConnectType.left(2) + "," + strStorageVol, stCommand::hight);
					itCharger->second.isDisCharging = true;
					int indexArray = batteryIDtoArrayIndex(strId);
					battery_state_enable_refresh[indexArray] = false;
					charger_state[indexArray] = STATE_DISCHARGING;//"放电中";
					emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
					printfDebugInfo(strId + "手动放电", enDebugInfoPriority::DebugInfoLevelOne);
					COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "手动放电\n");
				}
				else
				{
					toSend("P," + QString::number(itCharger->second.id), stCommand::hight);
					itCharger->second.isDisCharging = false;
					int indexArray = batteryIDtoArrayIndex(strId);
					battery_state_enable_refresh[indexArray] = false;
					charger_state[indexArray] = STATE_FREE;//"充电器闲置";
					emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
					printfDebugInfo(strId + "手动停止", enDebugInfoPriority::DebugInfoLevelOne);
					COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "手动停止放电\n");
				}
			}	
			else if (itCharger->second.chargerType == DJI_Charger)
			{
				//先判断是否在充电状态
				if ( groupBox->getCharging() == false)
				{
					//拼装 读取放电状态命令 ，再设置放电状态命令 
					QVector<stCommand> vtStCommand;
					QString strCommad;
					strCommad.sprintf("C2S,F10,%d,R", itCharger->second.id);  //读取放电状态命令
					stCommand stCommR = stCommand(strCommad, stCommand::hight); stCommR.chargerType = DJI_Charger;
					vtStCommand.append(stCommR);
					strCommad.sprintf("C2S,F10,%d,W,%d,%d", itCharger->second.id, strId.toInt() % 100, groupBox->getDisCharging()?0: 1);  //设置放电状态命令 1 放电 0 停止
					stCommand stCommW = stCommand(strCommad, stCommand::hight); stCommW.chargerType = DJI_Charger;
					vtStCommand.append(stCommW);
					m_CommandQueue.addVtCommand(vtStCommand);
					

					//UI更新
					int indexArray = batteryIDtoArrayIndex(strId);
					battery_state_enable_refresh[indexArray] = false;
					itBattery->second.timeLockUI.restart();
					itBattery = itLevel->second.mapBattery.find(itBattery->first);
					itBattery->second.timeLockUI.restart();

					if (groupBox->getDisCharging() == false){
						printfDebugInfo(strId + "手动放电", enDebugInfoPriority::DebugInfoLevelOne);
						COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "手动放电\n");
					}
					else{
						printfDebugInfo(strId + "手动停止", enDebugInfoPriority::DebugInfoLevelOne);
						COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "手动停止放电\n");
					}
					charger_state[indexArray] = groupBox->getDisCharging() ? STATE_FREE : STATE_DISCHARGING;//"放电中";
					emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);  //信号处理函数中 更新groupBox->getDisCharging()


				}
				else{
					printfDebugInfo(strId + "正在充电，放电命令无效！", enDebugInfoPriority::DebugInfoLevelOne, true);
					showTipsMessagebox(1, strId + "正在充电，放电命令无效！");
				}
			}
		}
	}
}

//切换层级
void charging::OnBtnLevel()
{
	QPushButton *pBtn = (QPushButton*)sender();
	QString str = pBtn->text(), tmp;
	for (int j = 0; j < str.length(); j++)
	{
		if (str[j] > '0' && str[j] < '9')
			tmp.append(str[j]);
	}
	unsigned int nLevel = tmp.toUInt();
	if (nLevel == m_nCurrentLevel)
		return;
	else{
		m_nCurrentLevel = nLevel;
		//更新层级按钮的样式
		int tempLevel = 1;
		for (auto itpBtnLevel : m_vtUiLevelBtn)
		{		

			levelBtnData* pData = (levelBtnData*)itpBtnLevel->userData(tempLevel++);
			if (pData->nlevel == m_nCurrentLevel)  //选中的层级
			{
				itpBtnLevel->setStyleSheet("QPushButton{font-size:22px; color:white;border-image: url(" + QString(g_AppPath) + "/img/btnLevel_Checked.png);}" //
					"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/btnLevel_Checked.png);}"
					"QPushButton:pressed{border-image: url(" + QString(g_AppPath) + "/img/btnLevel_Checked.png);}"); //
			}
			else
				itpBtnLevel->setStyleSheet("QPushButton{font-size:16px; color:white;border-image: url(" + QString(g_AppPath) + "/img/btnLevel_Normal.png);}" //
				"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/btnLevel_Normal.png);}"
				"QPushButton:pressed{border-image: url(" + QString(g_AppPath) + "/img/btnLevel_Normal.png);}"); //
		}

		//更新充电格区域:是否隐藏，电压、电流、充电状态、在位状态

		MAP_LEVEL_IT itLevel = m_mapLevel.find(m_nCurrentLevel);
		if (itLevel != m_mapLevel.end())
		{
			for (int i = 0; i < MAX_BATTERY; i++)
			{
				bool flag = i < itLevel->second.mapBattery.size();
				m_vtUiChargGrid[i]->setVisible(flag);
				QString strTitle; strTitle.sprintf("%02d%02d", m_nCurrentLevel, i + 1);
				m_vtUiChargGrid[i]->setTitle(strTitle);

				//待添加：电压、电流、充电状态、在位状态
				//电池在位情况
				if (flag){
					MAP_BATTERY_IT itBattery = itLevel->second.mapBattery.begin(); int k = 0;
					for (; k < i; k++){ itBattery++;}					
					int indexArray = batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery->second.id));
					////充电器在线状态
					emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
					//电压
					emit RefreshState(enRefreshType::BatteryVol, indexArray);
					//电池状态
					emit RefreshState(enRefreshType::BatteryState, indexArray);					
				}				
			}
		}
	}
}

//刷新UI
void charging::onRefreshUI(QString strID)
{
	// 根据 strID 刷新对应的电池UI信息

	show_all_data_now();
}


// 更新textEdit 并且设置光标到末尾
void charging::updateTextEdit(QString strContent)
{ 
	printfDebugInfo(strContent, enDebugInfoPriority::DebugInfoLevelThree); 
} 

//刷新解锁：允许所有刷新
void charging::releaseBatteryStateRefresh()
{
	static char count = 0;
	count++;
	bool flag = false;
	if (m_mapCloset.size() >= 4 && count == 1)	
		flag = true;	
	else if (m_mapCloset.size() == 3 && count == 2)
		flag = true;
	else if (m_mapCloset.size() == 2 && count == 3)
		flag = true;
	else if (m_mapCloset.size() == 1 && count == 5)  //一个柜子 5个轮询释放一次
		flag = true;

	if (flag)
	{
		count = 0;
		for (int j = 0; j < battery_state_enable_refresh.size(); j++)
			battery_state_enable_refresh[j] = true;
	}
}

QAction * charging::createMenus()
{ 
	m_menuSys = new QMenu(ui.btnMenuSys);
	QAction * action = new QAction("暂停串口通信", m_menuSys);
	action->setCheckable(true);	
	connect(action, SIGNAL(triggered(bool )), this, SLOT(onPauseScand(bool)));
	action->setChecked(!m_bContinueScan);
	m_CommandQueue.setPause(m_bContinueScan);
	m_menuSys->addAction(action);

	QAction * action9 = new QAction("暂停网络传输", m_menuSys);
	action9->setCheckable(true);
	action9->setChecked(!m_bContinueSubmit);
	connect(action9, SIGNAL(triggered(bool)), this, SLOT(onPauseSubmit(bool)));
	m_menuSys->addAction(action9);
	
	QAction * action8 = new QAction("显示系统参数", m_menuSys);
	m_menuSys->addAction(action8);
	connect(action8, SIGNAL(triggered()), this, SLOT(onOpenOptionWin()));

	QAction * action10 = new QAction("显示调试信息", m_menuSys);
	m_menuSys->addAction(action10);
	connect(action10, SIGNAL(triggered()), this, SLOT(OnBtnShowDebugInfo()));

	QAction * action12 = new QAction("显示电池信息", m_menuSys);
	connect(action12, SIGNAL(triggered()), this, SLOT(onOpenDataDialog()));
	m_menuSys->addAction(action12);

	m_menuItemCan = new QAction("CAN设备", m_menuSys);
	m_menuSys->addAction(m_menuItemCan); 
	m_menuItemCan->setCheckable(true);
	connect(m_menuItemCan, SIGNAL(triggered(bool)), this, SLOT(onBtnCanDevice(bool)));
	//action12->setVisible(false);


	//扫描串口，将扫描结果加入菜单栏，自动打开串口号最大的一个串口
	QAction * action11 = new QAction("串口设备", m_menuSys);
	m_menuSys->addAction(action11);


	QAction * action4 = new QAction("显示信息等级", m_menuSys);
	m_menuSys->addAction(action4);
	m_menuDebugInfoLevel = new QMenu(m_menuSys);	
	QAction * action5 = new QAction("一级", m_menuDebugInfoLevel); action5->setCheckable(true);
	m_menuDebugInfoLevel->addAction(action5);
	QAction * action6 = new QAction("二级", m_menuDebugInfoLevel); action6->setCheckable(true);
	m_menuDebugInfoLevel->addAction(action6);
	QAction * action7 = new QAction("三级", m_menuDebugInfoLevel); action7->setCheckable(true);
	m_menuDebugInfoLevel->addAction(action7);
	if (m_enShowDebugInfoLevel == DebugInfoLevelOne)
		action5->setChecked(true);
	else if (m_enShowDebugInfoLevel == DebugInfoLevelTwo)
		action6->setChecked(true);
	else if (m_enShowDebugInfoLevel == DebugInfoLevelThree)
		action7->setChecked(true);
	connect(m_menuDebugInfoLevel, SIGNAL(triggered(QAction *)), this, SLOT(OnClickMenuDebugInfoLeve(QAction *)));
	action4->setMenu(m_menuDebugInfoLevel);
	 
	m_menuCom = new QMenu(m_menuSys);
	connect(m_menuCom, SIGNAL(triggered(QAction *)), this, SLOT(OnClickMenuCom(QAction *))); 

	
	
	return serial_scan(action11);
}
void charging::OnClickMenuDebugInfoLeve(QAction * action)
{
	QString str = action->text();
	if (str == "一级")
		m_enShowDebugInfoLevel = DebugInfoLevelOne;
	if (str == "二级")
		m_enShowDebugInfoLevel = DebugInfoLevelTwo;
	if (str == "三级")
		m_enShowDebugInfoLevel = DebugInfoLevelThree;
	QList<QAction*> listAction = m_menuDebugInfoLevel->actions();
	for (int i = 0; i < listAction.size(); i++){
		if (listAction[i] != action)
			listAction[i]->setChecked(false);
	}
}



//所有电池
void charging::allClosetCharging()
{
	//
	if (isOpenSerialPort)
	{
		for (int i = 0; i < battery_local_id.size(); i++)
		{
			QString strId = battery_local_id[i];
			int iResult = 0;
			chargingByLocalID(strId, &iResult);
		}
	}

}
//当前柜子所有电池充电
void charging::oneClosetCharging()
{
	if (isOpenSerialPort)
	{ 
		QString   strId;
		QString strClosetUiNo = m_iCurrentCloset < 10 ? "0" + QString::number(m_iCurrentCloset) : QString::number(m_iCurrentCloset);
		for (int i = 0; i < battery_local_id.size(); i++)
		{
			strId = battery_local_id[i];
			if (strId.left(2) == strClosetUiNo)
			{
				int iResult = 0;
				chargingByLocalID(strId,&iResult);
			}
		}
	} 
}


//创建充电UI格+ 充电器层级
void charging::createChargGrid()
{
	m_gridLayout = new QGridLayout(this);
	m_groupBox = new QGroupBox("", this); 

	QString strID; 
	int nBatteryNumber = MAX_BATTERY;
	
	for (int i = 0; i < nBatteryNumber; i++)
	{
		strID.sprintf("ui_chage_item%d", i + 1);
		ui_charg_grid *p_charge_grid = new ui_charg_grid(this, strID);
		QString strTitle; strTitle.sprintf("%02d%02d", m_iCurrentCloset, i + 1);
		p_charge_grid->setTitle(strTitle);
		//p_charge_grid->setBalence(i < 41);
		MAP_BATTERY_IT itBattery = m_mapBattery.find(strTitle.toInt());
		if (itBattery != m_mapBattery.end()){
			MAP_BATTERY_MODEL_IT itBatteryModel = m_mapBatteryModel.find(itBattery->second.modelId);
			if (itBatteryModel != m_mapBatteryModel.end()){
				p_charge_grid->setBatteryModel(QString::fromLocal8Bit(itBatteryModel->second.droneModel));
			}
		}
		
		m_gridLayout->addWidget(p_charge_grid, i / 5, i % 5);	

		m_vtUiChargGrid.append(p_charge_grid);

		QObject::connect(p_charge_grid->getChargingBtn(), &DoubleClickedButton::singleClicked, this, &charging::OnBtnChargingOrStopCharging1);
		QObject::connect(p_charge_grid->getChargingBtn(), &DoubleClickedButton::doubleClicked, this, &charging::OnBtnDisChargingOrStop1);

		p_charge_grid->setChargerState(STATE_OFFLINE);

		
	}
	MAP_LEVEL_IT itLevel = m_mapLevel.find(m_nCurrentLevel);
	if (itLevel != m_mapLevel.end())
	{
		for (int i = 0; i < MAX_BATTERY; i++){
			bool flag = i < itLevel->second.mapBattery.size();
			m_vtUiChargGrid[i]->setVisible(flag);
		}
	}
	 
	// 设置水平间距
	m_gridLayout->setHorizontalSpacing(1);
	// 设置垂直间距
	m_gridLayout->setVerticalSpacing(1);

	m_groupBox->setLayout(m_gridLayout);

	//暂时不需要显示非智能电池
	//for (int i = 40; i < 44; i++)
	//{
	//	strID.sprintf("ui_chage_item%d", i + 0 );
	//	ui_charg_grid *p_charge_grid = new ui_charg_grid(this, strID);
	//	QString strTitle; strTitle.sprintf("%02d%02d", m_iCurrentCloset, i + 1);
	//	p_charge_grid->setTitle(strTitle);
	//	p_charge_grid->setBalence(i < 40);
	//	MAP_BATTERY_IT itBattery = m_mapBattery.find(strTitle.toInt());
	//	if (itBattery != m_mapBattery.end()){
	//		MAP_BATTERY_MODEL_IT itBatteryModel = m_mapBatteryModel.find(itBattery->second.modelId);
	//		if (itBatteryModel != m_mapBatteryModel.end()){
	//			p_charge_grid->setBatteryModel(QString::fromLocal8Bit(itBatteryModel->second.droneModel));
	//		}
	//	} 
	//	m_vtUiChargGrid.append(p_charge_grid); 
	//	QObject::connect(p_charge_grid->getChargingBtn(), &DoubleClickedButton::singleClicked, this, &charging::OnBtnChargingOrStopCharging1);
	//	QObject::connect(p_charge_grid->getChargingBtn(), &DoubleClickedButton::doubleClicked, this, &charging::OnBtnDisChargingOrStop1);
	//	//定制非智能状态
	//	p_charge_grid->setFixedSize(QSize(85*2 +12, 70));
	//	p_charge_grid->setGeometry(QRect(19 + ((i - 40) * 193), 523, 85 * 2 + 12, 73));
	//	p_charge_grid->getChargingBtn()->setGeometry(QRect(1, 3, 85 * 2 + 11, 68));
	//	p_charge_grid->m_strBtnStyleOffline = "QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_Gray_W.png);}";
	//	p_charge_grid->m_strBtnStyleFree = "QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_W.png);}"
	//		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_Hover_W.png);}";
	//	p_charge_grid->m_strBtnStyleCharging = "QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Pressed_W.png);}"
	//		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnCharge_Pressed_Hover_W.png);}";
	//	p_charge_grid->m_strBtnStyleDischarg = "QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Pressed_W.png);}"
	//		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnCharge_Pressed_Hover_W.png);}";
	//	p_charge_grid->m_strBtnStyleOverHeat = "QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_Red_W.png);}"
	//		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_Red_W.png);}";
	//	p_charge_grid->setChargerState(STATE_OFFLINE);
	//}

	//创建层级
	QString strPushButton;
	QRect rect_;
	int count = 1;
	for (auto itLevel : m_mapLevel)
	{
		strPushButton = QString("%1层").arg(itLevel.first);
		QPushButton* pushButton = new QPushButton(strPushButton, this);
		//设置位置
		rect_.setLeft(10);
		rect_.setTop(70 + 65 * count++);
		rect_.setWidth(110);
		rect_.setHeight(40);
		pushButton->setGeometry(rect_);
		if (itLevel.first == m_nCurrentLevel)  //选中的层级
		{
			pushButton->setStyleSheet("QPushButton{font-size:20px; color:white;border-image: url(" + QString(g_AppPath) + "/img/btnLevel_Checked.png);}" //
				"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/btnLevel_Checked.png);}"
				"QPushButton:pressed{border-image: url(" + QString(g_AppPath) + "/img/btnLevel_Checked.png);}"); //
		}else
		pushButton->setStyleSheet("QPushButton{font-size:16px; color:white;border-image: url(" + QString(g_AppPath) + "/img/btnLevel_Normal.png);}" //
			"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/btnLevel_Normal.png);}"
			"QPushButton:pressed{border-image: url(" + QString(g_AppPath) + "/img/btnLevel_Normal.png);}"); //
		levelBtnData * pData = new levelBtnData; pData->nlevel = itLevel.first;
		pushButton->setUserData(itLevel.first, pData);

		QObject::connect(pushButton, &QPushButton::clicked, this, &charging::OnBtnLevel);

		m_vtUiLevelBtn.append(pushButton);
	}
	
}

//创建ui：调试信息区域
void charging::createDebugInfoUI()
{
	m_GroupBox_DebugInfo = new QGroupBox("", this);
	tablePage_DebugInfo = new QTabWidget(m_GroupBox_DebugInfo);
	tab_1 = new QWidget();	
	m_TextEdit_DebugInfo = new QTextEdit(tab_1);   //调试信息文本框
	tablePage_DebugInfo->addTab(tab_1, QString("传输信息"));
	m_CheckBox_DebugInfo = new QCheckBox("自动隐藏", m_GroupBox_DebugInfo);   //调试信息自动隐藏
	m_BtnMin_DebugInfo = new QPushButton(m_GroupBox_DebugInfo);   //调试信息隐藏按钮
	m_Label_DebugInfo = new QLabel("信息调试区域", m_GroupBox_DebugInfo);
	tab_2 = new QWidget();
	m_listview_Battery = new QListView(tab_2); 
	QStringList strList  ; 
	MAP_CLOSET_IT itCloset;  MAP_BATTERY_IT itBattery;
	itCloset = m_mapCloset.find(m_iCurrentCloset);
	if (itCloset != m_mapCloset.end()){
		for (itBattery = itCloset->second.mapBattery.begin(); \
			itBattery != itCloset->second.mapBattery.end(); itBattery++)
		{
			QString str;
			str.sprintf("电池：%d，温度：00.0°，电压：0.00v", (itBattery->first));
			strList << str;
		}
	}
	if (m_listview_Battery_model == nullptr){
		m_listview_Battery_model = new QStringListModel(strList);
	}
	m_listview_Battery->setModel(m_listview_Battery_model); 
	tablePage_DebugInfo->addTab(tab_2, QString("电池信息"));
}

//调整ui： 设置位置、隐藏控件、设置图片等
void charging::adjustUI()
{ 
	//设置主窗大小 、背景、标题
	QString str = "QMainWindow{background-image: url(" + QString(g_AppPath) + "/img/mainwin_background.png);}";
	this->setStyleSheet(str);
	this->setFixedSize(800, 600); 
	this->setWindowTitle(g_winTitle);
	ui.lab_logo->setStyleSheet("QLabel{border-image: url(" + QString(g_AppPath) + "/img/logo.png);}");
	ui.lab_logo->setText("");  
	ui.lab_title->setText(g_winTitle);
	ui.lab_title->setStyleSheet("QLabel{font-size:24px;font-weight:bold;color:white}");

	//设置主窗口 ，设置无边框，允许任务栏按钮右键菜单，允许最小化与还原。
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setWindowIcon(QIcon(QString(g_AppPath) + "/ICon/MainWindow.ico"));

	//设置 电池组区域 尺寸
	m_groupBox->setGeometry(QRect(120, 70, 698, 507));
	m_groupBox->setStyleSheet("QGroupBox{border: 0px }");
	

	//设置操作菜单
	m_menuSys->setStyleSheet(
		"QMenu {background-color: white; border: 1px solid white; }\
		 QMenu::item {background-color: transparent;padding:8px 20px;margin:0px 8px;border-bottom:1px solid #DBDBDB;}");
	m_menuCom->setStyleSheet(
		"QMenu {background-color: white; border: 1px solid white; }\
			QMenu::item {background-color: transparent;padding:8px 20px;margin:0px 8px;border-bottom:1px solid #DBDBDB;}");
	//																																																																			QMenu::item:selected{background - color: #2dabf9;}");
	/* sets background of the menu 设置整个菜单区域的背景色，我用的是白色：white*/
	//   /*整个菜单区域的边框粗细、样式、颜色*/
	//"   " +  /* sets background of menu item. set this to something non-transparent if you want menu color and menu item color to be different */
	//" "/*设置菜单项文字上下和左右的内边距，效果就是菜单中的条目左右上下有了间隔*/
	//"" +/*设置菜单项的外边距*/
	//    ""+/*为菜单项之间添加横线间隔*/
	//""+ /* when user selects item using mouse or keyboard */
	//""+/*这一句是设置菜单项鼠标经过选中的样式*/
	//"" 		

	//上一页、下一页的图片
	ui.btnPrePage->setText("");
	ui.btnPrePage->setStyleSheet("QPushButton{border-image: url(" + QString(g_AppPath) + "/img/btnPrePage.png);}" //
		"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/btnPrePage_hover.png);}"
		"QPushButton:pressed{border-image: url(" + QString(g_AppPath) + "/img/btnPrePage.png);}"); //

	ui.btnNextPage->setText("");
	ui.btnNextPage->setStyleSheet("QPushButton{border-image: url(" + QString(g_AppPath) + "/img/btnNextPage.png);}" //
		"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/btnNextPage_hover.png);}"
		"QPushButton:pressed{border-image: url(" + QString(g_AppPath )+ "/img/btnNextPage.png);}"); //
	 

	
	//设置操作按钮位置、大小、背景图
	ui.btnMenuSys->setGeometry(QRect(520, 3, 94, 62));
	ui.btnMenuSys->setText("");
	ui.btnMenuSys->setStyleSheet("QPushButton{border-image: url(" + QString(g_AppPath) + "/img/btn_operator.png);}" //
		"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/btn_operator_hover.png);}"
		"QPushButton:pressed{border-image: url(" + QString(g_AppPath) + "/img/btn_operator.png);}"); //
	
	//设置操作按钮位置、大小、背景图
	ui.btnOneKey->setGeometry(QRect(605, 3, 94, 62));
	ui.btnOneKey->setText("");
	ui.btnOneKey->setStyleSheet("QPushButton{border-image: url(" + QString(g_AppPath) + "/img/btn_onekey.png);}"
		"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/btn_onekey_hover.png);}"
		"QPushButton:pressed{border-image: url(" + QString(g_AppPath) + "/img/btn_onekey.png);}"); //

	//设置操作按钮位置、大小、背景图
	ui.btnSysClose->setGeometry(QRect(690, 3, 94, 62));
	ui.btnSysClose->setText("");
	ui.btnSysClose->setStyleSheet("QPushButton{border-image: url(" + QString(g_AppPath) + "/img/btn_close.png);}"
		"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/btn_close_hover.png);}"
		"QPushButton:pressed{border-image: url(" + QString(g_AppPath) + "/img/btn_close.png);}"); //

	//设置最小化按钮隐藏
	str = "QPushButton{border-image: url(" + QString(g_AppPath) + "/img/btn_min_normal.png);}"
		"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/btn_min_hover.png);}"
		"QPushButton:pressed{border-image: url(" + QString(g_AppPath) + "/img/btn_min_normal.png);}";
	ui.btnSysMin->setStyleSheet(str);
	ui.btnSysMin->setVisible(false);


	//温度控件
	//ui.lab_temperature->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/test001.png);}");
	ui.lab_temperature->setStyleSheet("QLabel{ border: 4px solid rgb(27,118,189);border-radius:4px; border-top-width:25px; background-color: rgb(10,38,60,255); }");
	ui.lab_temperature_title->setStyleSheet("QLabel{font-size:16px;font-weight:bold;color:white}");
	ui.lab_temperature_number->setStyleSheet("QLabel{font-size:24px;font-weight:bold;color:white}");
	ui.lab_temperature_imge->setText("");
	ui.lab_temperature_number->setText("0.0°");
	ui.lab_temperature_imge->setStyleSheet("QLabel{border-image: url(" + QString(g_AppPath) + "/img/temperature_disenable.png);}");


	//设置调试信息区域
	m_GroupBox_DebugInfo->setGeometry(QRect(800, 70, 350, 530));
	m_GroupBox_DebugInfo->setStyleSheet("QGroupBox {   background-color: white } ");
	m_CheckBox_DebugInfo->setGeometry(QRect(200, 10, 100, 20));
	m_CheckBox_DebugInfo->setStyleSheet("QCheckBox { font-size:16px;font-weight:bold;color:black } ");
	m_CheckBox_DebugInfo->setCheckState(m_bAutoHideDebugInfo ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
	m_Label_DebugInfo->setGeometry(QRect(10, 10, 150, 20));
	m_Label_DebugInfo->setStyleSheet("QLabel{font-size:16px;font-weight:bold;color:black} ");
	m_BtnMin_DebugInfo->setGeometry(QRect(309, 1, 40, 40));
	str = "QPushButton{border-image: url(" + QString(g_AppPath) + "/img/btnDebugInfoMinNomarl.png);}"
		"QPushButton:hover{border-image: url(" + QString(g_AppPath) + "/img/btnDebugInfoMinHover.png);}"
		"QPushButton:pressed{border-image: url(" + QString(g_AppPath )+ "/img/btnDebugInfoMinPressed.png);}";
	m_BtnMin_DebugInfo->setStyleSheet(str);

	tablePage_DebugInfo->setGeometry(QRect(0, 40, 350, 490));
	m_TextEdit_DebugInfo->setGeometry(QRect(0, 0, 348, 466));
	m_TextEdit_DebugInfo->setReadOnly(true);
	m_listview_Battery->setGeometry(QRect(0, 0, 348, 466));


	//设置充电柜区域 背景，样式调整background-color: white; border: 1px solid white;
	//QTabWidget::pane{border-width:2px; border-color:white;border-style: outset;background-color:#C2C7CB;}
	str = "QTabWidget::pane{border-width:2px; border-color:rgb(28, 116, 187, 255);border-style: outset;border-botton-radius:4px;;background-color:rgb(13, 41, 67, 255);}\
			QTabWidget::tab-bar{alignment:left;}\
			QTabBar::tab{background:rgb(11, 37, 62, 255);border-width:2px; border-color:rgb(28, 116, 187, 255);border-top-radius:4px;\
							color:white;min-width:30px;min-height:10px; width:175px;height:30px;}\
			QTabBar::tab:hover{	background:rgb(20, 29, 60, 255);}\
			QTabBar::tab:selected{background:rgb(28, 116, 187, 255);}";

	//ui.tabWidget->setStyleSheet("QTabWidget::pane{border-width:2px; border-color:lightGray;\
	//		border-style: outset;background-color:#C2C7CB;}\
	//		QTabBar::tab{border-bottom-color: #C2C7CB;border-top-left-radius: 0px;\
	//					border-top-right-radius: 0px;max-width: 75px; min-width:75px; min-height:25px;\
	//					font:14px Times New Roman;padding: 0px;} ");//background: transparent;
	ui.tabWidget->setStyleSheet(str);
	//CReadIniFile*  qtDllObj = CReadIniFile::getInstance();
	//QString strShow = qtDllObj->readProfileInfo("WidgetShow", "debugTextShow", g_AppPath + "\\set.ini", &iError);
	//bool flag = strShow == "1";
	//ui.textEdit_show->setVisible(flag); 
	//
	//strShow = qtDllObj->readProfileInfo("WidgetShow", "FullScreemShow", g_AppPath + "\\set.ini", &iError);
	//flag = strShow == "1";
	//if (flag){
	//	//设置全屏显示
	//	setWindowState(Qt::WindowFullScreen);
	//	//设置无边框且置顶
	//	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	//}
	//else{
	//	//固定大小窗口显示
	//	//this->setFixedSize(this->width(), this->height()); 
	//}
	//柜子页数调整：如果大与一页数，显示上一页下一页 
	//ui.btnPrePage->setVisible(m_mapCloset.size() > ONE_PAGE_CLOSET);
	//ui.btnNextPage->setVisible(m_mapCloset.size() > ONE_PAGE_CLOSET);
}

void charging::OnBtnShowSysMenu()
{
	QRect rect = ui.btnMenuSys->geometry(); 
	m_menuSys->move(mapToGlobal(QPoint(rect.left() + 10, rect.bottom())));
	//menu->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
	m_menuSys->show();
}

//显示
void charging::OnBtnShowComMenu()
{
	QRect rect = ui.btnOneKey->geometry();
	m_menuCom->move(mapToGlobal(QPoint(rect.left() + 10, rect.bottom())));
	//menu->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
	m_menuCom->show();
}

//最小化
void charging::OnBtnSysMin()
{
	setWindowState(Qt::WindowMinimized);
}

//关闭
void charging::OnBtnSysClose()
{
	QMessageBox::StandardButton button;
	button = (QMessageBox::StandardButton)QMessageBox::question(this, "提示",
		"程序正在运行，是否退出?",
		"是", "否");
	if (button == 0) { //保存
		if (meTimer)
			meTimer->stop();
		m_CommandQueue.stop();
		m_CommandQueue.clearCommand();
		while (!m_CommandQueue.isFinished()){
			Sleep(10); 
		}
		m_OperDB.onCloseDbFile();
		//在线程run结束前关闭串口和can进程
		m_ConnectDBThread.stop();
		while (!m_ConnectDBThread.isFinished()){
			Sleep(10);
		} 
		/*
		SERIAL_PORT->ClosePort();
		onOpenOrCloseCanDevice(false);*/
		emit ExitApp();
	}
}
void charging::OnBtnHideDebugInfo()
{
	if (m_bShowDebugInfo){
		if (m_ThreadHideOrShowDebugInfo.isRunning() == false){
			QRect rect = m_GroupBox_DebugInfo->geometry();
			m_ThreadHideOrShowDebugInfo.setBasePosition(rect);
			m_ThreadHideOrShowDebugInfo.setShowOrHide(false);
			m_ThreadHideOrShowDebugInfo.start();
			m_bShowDebugInfo = false;
		} 
	}

}
void charging::OnBtnShowDebugInfo()
{
	if (m_bShowDebugInfo == false){
		QRect rect = m_GroupBox_DebugInfo->geometry();
		if (m_ThreadHideOrShowDebugInfo.isRunning()==false){
			QRect rect = m_GroupBox_DebugInfo->geometry();
			m_ThreadHideOrShowDebugInfo.setBasePosition(rect);
			m_ThreadHideOrShowDebugInfo.setShowOrHide(true);
			m_ThreadHideOrShowDebugInfo.start();
			m_bShowDebugInfo = true;
		}
	}
}
void charging::OnCheckAutoShowDebugInfo(int nState)
{ 
	m_bAutoHideDebugInfo = (nState == 2);//打钩nState = 2, 不勾0
	(CReadIniFile::getInstance()->writeProfileInfo("WidgetShow", "AutoHideDebugInfo", QString::number(m_bAutoHideDebugInfo), QString(g_AppPath) + "\\set.ini", &iError));
}
void charging::printfDebugInfo(QString strInfo, enDebugInfoPriority enLevel, bool bErrorInfo )
{
	if (m_TextEdit_DebugInfo == nullptr)
		return;
	QTextCursor cursor = m_TextEdit_DebugInfo->textCursor();
	cursor.movePosition(QTextCursor::End);
	m_TextEdit_DebugInfo->setTextCursor(cursor);  //移动光标到末尾

	if (enLevel <= m_enShowDebugInfoLevel){
		QString strTemp;
		if (bErrorInfo)		//错误信息
		{
			strTemp = "<p><font size=\"5\" color=\"red\">" + strInfo + "</font></p>";
			m_TextEdit_DebugInfo->append(strTemp);
			if (ui.tabWidget->currentIndex()!= 1)
				ui.tabWidget->setCurrentIndex(1);
		}
		else{
			strTemp = "<p><font size=\"3\" color=\"black\">" + strInfo + "</font></p>";
			m_TextEdit_DebugInfo->append(strTemp);
		}
	}
}


void charging::OnRefreshState(enRefreshType type, int index)
{
	int uiIndex = 0;
	QString strLocalId = battery_local_id[index];
	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
	if (getBatteryIdRelatedInfo(strLocalId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
	{ 
		//比较是否对应当前层;防止申请别的层电池充电刷新到层
		//QString strClosetUiNo = m_nCurrentLevel< 10 ? "0" + QString::number(m_nCurrentLevel) : QString::number(m_nCurrentLevel);
		if (itCharger->second.nLevel == m_nCurrentLevel)
		{
			//strTitle = m_vtUiChargGrid[index2]->title();
			//映射到ui元素id
			/*if (index < MAX_BATTERY)
				uiIndex = index;
			else if (index >= MAX_BATTERY)
				uiIndex = index % MAX_BATTERY;*/
			for (auto itBattery_ : itLevel->second.mapBattery){   //找到当前层级对应的ui 元素id				
				if (strLocalId.toInt() == itBattery_.first)
					break;
				uiIndex++;
			}


			if (type == enRefreshType::ChargerState && battery_state_enable_refresh[index])
			{ 
				if (itCharger->second.isOverHeat )
					m_vtUiChargGrid[uiIndex]->setChargerState(STATE_OVER_HEAT);
				else
					m_vtUiChargGrid[uiIndex]->setChargerState(charger_state[index]);
				updateListviewBatteryModel(index);
			}
			else if (type == enRefreshType::BatteryState  && battery_state_enable_refresh[index]){
				m_vtUiChargGrid[uiIndex]->setBatteryState(battery_state[index], battery_voltage[index], battery_temperature[index]);

			}
			else if (type == enRefreshType::ChargerOnlineState)
			{  
				if (itCharger->second.isOverHeat)
					m_vtUiChargGrid[uiIndex]->setChargerState(STATE_OVER_HEAT);
				else
				{ 
					m_vtUiChargGrid[uiIndex]->setChargerState(charger_state[index]);
				}
				updateListviewBatteryModel(index);
			}
			else if (enRefreshType::BatteryVol == type){
				m_vtUiChargGrid[uiIndex]->setVol(battery_voltage[index], battery_temperature[index]);
				updateListviewBatteryModel(index);
			}
		}
	}
}
//创建柜子相关RadioUI
void charging::createClosetRadio()
{ 
	m_vtUiClosetRadio.clear();
	for (MAP_CLOSET_IT itCloset = m_mapCloset.begin(); itCloset != m_mapCloset.end(); itCloset++){
		QPushButton* qRadioBtn = new QPushButton(ui.tab);
		qRadioBtn->setText("充电柜"+ QString::fromLocal8Bit(itCloset->second.id));
		QObject::connect(qRadioBtn, &QPushButton::clicked, this, &charging::OnClickClosetNo);
		pSetQPushButtonChecked(qRadioBtn, false);

		m_vtUiClosetRadio.append(qRadioBtn);
	}
	for (int iCount = 0; iCount < m_vtUiClosetRadio.size(); iCount++){
		if ((iCount + 1) % ONE_PAGE_CLOSET == 1)
			m_vtUiClosetRadio[iCount]->setGeometry(QRect(43, 85, 263, 53));
		if ((iCount + 1 )% ONE_PAGE_CLOSET == 2)
			m_vtUiClosetRadio[iCount]->setGeometry(QRect(43, 153, 263, 53));
		if ((iCount + 1) % ONE_PAGE_CLOSET == 3)
			m_vtUiClosetRadio[iCount]->setGeometry(QRect(43, 221, 263, 53));
		if ((iCount + 1) % ONE_PAGE_CLOSET == 4)
			m_vtUiClosetRadio[iCount]->setGeometry(QRect(43, 289, 263, 53));		
		if ((iCount + 1) % ONE_PAGE_CLOSET == 0)
			m_vtUiClosetRadio[iCount]->setGeometry(QRect(43, 357, 263, 53));
		if (iCount >= ONE_PAGE_CLOSET)
			m_vtUiClosetRadio[iCount]->setVisible(false);
	} 
	if (m_vtUiClosetRadio.size() >= 1){		
		pSetQPushButtonChecked(m_vtUiClosetRadio[0], true);
		MAP_CLOSET_IT itCloset = m_mapCloset.begin();
		m_iCurrentCloset = itCloset->first;
	}
}
//刷新当前的电池信息
void charging::refreshCurrentUI()
{ 
	MAP_LEVEL_IT itLevel = m_mapLevel.find(m_nCurrentLevel);
	if (itLevel == m_mapLevel.end())
		return;
	int i = 0;
	for ( MAP_BATTERY_IT itBattery = itLevel->second.mapBattery.begin();
		i < MAX_BATTERY &&itBattery != itLevel->second.mapBattery.end();itBattery++,i++)
	{		
		if (m_vtUiChargGrid.size() > 0)
		{
			m_vtUiChargGrid[i]->setTitle(QString::fromLocal8Bit(itBattery->second.id));  	  //设置标题 			
			MAP_BATTERY_MODEL_IT itBatteryModel = m_mapBatteryModel.find(itBattery->second.modelId);
			if (itBatteryModel != m_mapBatteryModel.end()){//设置电池型号
				m_vtUiChargGrid[i]->setBatteryModel(QString::fromLocal8Bit(itBatteryModel->second.droneModel));
			}
		} 
	}
	
}
//按下鼠标
void charging::mousePressEvent(QMouseEvent *event)
{		
	bool flag = event->button() == Qt::LeftButton;
	if (flag)
	{
		this->setCursor(Qt::ClosedHandCursor);
		m_LButtonPress = true;
		m_MousePrePoint = event->globalPos() - this->pos();
	}	
}
//鼠标移动 
void charging::mouseMoveEvent(QMouseEvent *event)
{
	if (m_LButtonPress && m_bCanMoveWindow)
	{
		QPoint movePos = event->globalPos();
		this->move(movePos - m_MousePrePoint);  //实现窗口移动
	}
}
//鼠标松开
void charging::mouseReleaseEvent(QMouseEvent * event)
{
	if (m_LButtonPress)
	{
		m_LButtonPress = false;
		this->setCursor(Qt::ArrowCursor);
	}
}

//设置调试信息区域的位置
void charging::OnAdjustDebugInfoRect(QRect rect)
{
	m_GroupBox_DebugInfo->setGeometry(rect);
}

void charging::updateListviewBatteryModel(int indexMem )
{
	QString str; QModelIndex modelIndex;
	if (indexMem == -1){
		//更新当前柜子所以电池数据
		m_listview_Battery_model->removeRows(0, m_listview_Battery_model->rowCount()); //删除所以
		MAP_CLOSET_IT itCloset = m_mapCloset.find(m_iCurrentCloset);		
		int indexArray = 0;  int rowCount = 0; 
		for (MAP_BATTERY_IT itBattery = itCloset->second.mapBattery.begin();
			itBattery != itCloset->second.mapBattery.end();	itBattery++){
			m_listview_Battery_model->insertRows(rowCount, 1); //新增
			modelIndex = m_listview_Battery_model->index(rowCount++);
			indexArray = batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery->second.id));
			str.sprintf("电池：%s，温度：%03.1f°，电压：%0.2fv"\
				,(itBattery->second.id)
				,(battery_temperature[indexArray].toFloat())
				,(battery_voltage[indexArray].toFloat())
				);

			m_listview_Battery_model->setData(modelIndex, str);
			//m_listview_Battery->setCurrentIndex(index);
		}
	}
	else
	{
		//更新index对应的数据
		QString strBatteryId = battery_local_id[indexMem];
		MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
		if (getBatteryIdRelatedInfo(strBatteryId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
		{
			//int uiInsex = indexMem % MAX_BATTERY;
			QModelIndex modelIndex = m_listview_Battery_model->index(indexMem);
			QVariant variant = m_listview_Battery_model->data(modelIndex, Qt::DisplayRole);  //获取当前选择的项的文本 
			if (itCharger->second.chargerType == NF_Charger){
				str.sprintf("电池：%s，温度：%03.1f°，电压：%0.2fv" 
					, (itBattery->second.id)
					, (battery_temperature[indexMem].toFloat())
					, (battery_voltage[indexMem].toFloat())
					);
			}else if (itCharger->second.chargerType == DJI_Charger)
			{
				str.sprintf("电池：%s，温度：%03.1f°，电压：%0.2fv"
					, (itBattery->second.id)
					, (battery_temperature[indexMem].toFloat())
					, (battery_voltage[indexMem].toFloat())
					);
			}
			
			m_listview_Battery_model->setData(modelIndex, str);
		} 
	}
}

