#include "charging.h"
#pragma execution_character_set("utf-8")

charging::charging(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//QRect rect = this->geometry();
	//this->setFixedSize(QSize(rect.width(), rect.height())); //固定窗口大小

	//最大化窗口显示
	//setWindowState(Qt::WindowMaximized);
	meTimer = NULL;	//定时器指针

	m_bContinueScan = true;// 继续扫描标志

	m_bContinueSubmit = true;//继续提交网络标志

	isOpenSerialPort = false; 

	m_enShowDebugInfoLevel = DebugInfoLevelThree;

	m_nChargeLimitTime = CHARGING_LIMIT_TIME;

	m_msgDlg = new CMessageBoxCus(this, 1500); //
	m_msgDlg->setWindowModality(Qt::WindowModal);

	m_loginDlg = new CLoginDialog(this);
	m_loginDlg->setWindowModality(Qt::WindowModal);


	//init_now();
}

charging::~charging()
{

}

void charging::closeEvent(QCloseEvent *event)
{
	qDebug() << "Close：1进入关闭函数";
	if (meTimer)
		meTimer->stop();
	qDebug() << "Close：2关闭定时器"; 
	m_CommandQueue.stop();
	qDebug() << "Close：3关闭命令队列";
	SERIAL_PORT->ClosePort();
	//my_Serial.serialClose();
	qDebug() << "Close：4关闭串口";
	event->accept();  //接受退出信号，程序退出
}



void charging::OnExitApp()
{
	this->close();
}

//转换 电池id对应 数组Index
int charging::batteryIDtoArrayIndex(QString strID )
{
	int indexArray= -1, iClosetId = 0;
	if (strID.length() == 3)
		strID = "0" + strID;

	for (int i = 0; i < battery_local_id.size(); i++)
	{
		if (battery_local_id[i] == strID)
		{
			indexArray = i;
			break;
		}
	}
	return indexArray; 
}

//充电器ID 查找对应继电器闭合的电池ID
int charging::chargerIDtoBatteryId(int chargerId)
{
	int nBatteryID = -1, nTheUint = chargerId % 100;
	MAP_CLOSET_IT itCloset = m_mapCloset.find(chargerId / 100);
	if (itCloset != m_mapCloset.end()){
		MAP_CHARGER_IT itCharger = itCloset->second.mapCharger.find(chargerId);
		MAP_BATTERY_IT itBattery = itCloset->second.mapBattery.find(chargerId);
		if (itCharger != itCloset->second.mapCharger.end()
			&&	itBattery != itCloset->second.mapBattery.end())
		{
			nBatteryID = itBattery->first;
		}
	}
	return nBatteryID;
} 
bool charging::getBatteryIdRelatedInfo(QString strBatteryId, MAP_CLOSET_IT& itCloset, MAP_BATTERY_IT& itBattery, MAP_BATTERY_MODEL_IT& itBatteryModel, MAP_CHARGER_IT& itCharger, MAP_LEVEL_IT& itLevel)
{	
	 
	itCloset = m_mapCloset.find(1);
	if (itCloset != m_mapCloset.end())
	{
		itBattery = itCloset->second.mapBattery.find(strBatteryId.toInt());
		if (itBattery != itCloset->second.mapBattery.end())
		{
			itCharger = itCloset->second.mapCharger.find(itBattery->second.relatedCharger);
			if (itCharger == itCloset->second.mapCharger.end())
			{
				printfDebugInfo(strBatteryId + "电池未匹配充电器", enDebugInfoPriority::DebugInfoLevelOne, true);
				return false;
			}
			itLevel = m_mapLevel.find(itCharger->second.nLevel);
			if (itLevel == m_mapLevel.end()){
				printfDebugInfo(strBatteryId + "电池未匹配层级", enDebugInfoPriority::DebugInfoLevelOne, true);
				return false;
			}

			itBatteryModel = m_mapBatteryModel.find(itBattery->second.modelId);
			if (itBatteryModel == m_mapBatteryModel.end())
			{
				printfDebugInfo(strBatteryId + "电池未匹配电池型号", enDebugInfoPriority::DebugInfoLevelOne, true);
				return false;
			}  
		}
		else
		{
			printfDebugInfo(strBatteryId + "未匹配电池", enDebugInfoPriority::DebugInfoLevelOne, true);
			return false;
		}
	}
	else{
		printfDebugInfo(strBatteryId + "未匹配充电柜编号", enDebugInfoPriority::DebugInfoLevelOne, true);
		return false;
	}
	return true;
}
//检测充电条件
bool charging::detectChargingCondition(QString strBatteryId, int* iResult, bool showDebugInfo)
{
	if (SERIAL_PORT->isOpen() == false)
	{
		if (showDebugInfo)
		{
			printfDebugInfo("串口未打开", enDebugInfoPriority::DebugInfoLevelOne, true);
			showTipsMessagebox(2, "串口未打开，充电命令无效!");
		}
		*iResult = ERROR_DONT_OPEN_SERIAL;
		return false;
	}
	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
	if (getBatteryIdRelatedInfo(strBatteryId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
	{
		if (false == itCharger->second.bOnline){
			if (showDebugInfo)
			{
				printfDebugInfo(strBatteryId + "充电器不在线，充电命令无效!", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "充电器不在线，充电命令无效!");
			}
			*iResult = ERROR_NO_CHARGER;
			return false;
		}
		if (true == itCharger->second.isOverHeat){
			if (showDebugInfo)
			{
				printfDebugInfo(strBatteryId + "过热，充电命令无效!", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "过热，充电命令无效!");
			}
			*iResult = ERROR_OVER_HEAT;
			return false;
		}		 

		//非智能电池根据电压判断电池是否存在			
		/*if (itBatteryModel->second.balance == false)
		{
			if (itCharger->second.getAverage(itCharger->second.fVoltage) <3.1)
			{
				if (showDebugInfo)
				{
					printfDebugInfo(strBatteryId + "电池未检测，命令无效", enDebugInfoPriority::DebugInfoLevelOne, true);
					showTipsMessagebox(1, strBatteryId + "电池未检测，充电命令无效!");
				}
				*iResult = ERROR_NO_BATTERY;
				return false;
			}
		}*/

		if (itCharger->second.isCharging == true)
		{
			if (showDebugInfo)
			{
				printfDebugInfo("电池正在充电，充电命令无效", enDebugInfoPriority::DebugInfoLevelOne);
				showTipsMessagebox(1, strBatteryId + "电池正在充电，充电命令无效!");
			}
			return false;
		}
		if (itCharger->second.isDisCharging)
		{
			if (showDebugInfo){
				printfDebugInfo(strBatteryId + "正在放电", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "电池正在放电，充电命令无效!");
			}
			*iResult = ERROR_BATTERY_CHARGING;
			return false;
		}
	}
	return true;
}

//检测放电条件
bool charging::detectDisChargingCondition(QString strBatteryId, int* iResult, bool showDebugInfo)
{
	if (SERIAL_PORT->isOpen() == false)
	{
		if (showDebugInfo)
			printfDebugInfo("串口未打开", enDebugInfoPriority::DebugInfoLevelOne, true);

		*iResult = ERROR_DONT_OPEN_SERIAL;
		return false;
	}
	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
	if (getBatteryIdRelatedInfo(strBatteryId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
	{
		if (false == itBattery->second.isExisted)
		{
			//printfDebugInfo(QString("<p><font size=\"%1\" color=\"%2\">%3</font></p>").arg(12).arg("red").arg(strBatteryId + "电池不存在，命令无效"), enDebugInfoPriority::DebugInfoLevelOne);
			if (showDebugInfo)
			{
				printfDebugInfo(strBatteryId + "电池不存在，放电命令无效", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "充电器不在线，放电命令无效!"); 
			}
			*iResult = ERROR_NO_BATTERY;
			return false;
		}
		if (itBatteryModel->second.balance)
		{
			if (showDebugInfo)
			{
				printfDebugInfo(strBatteryId + "属于智能电池，目前不支持放电", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "属于智能电池，目前不支持放电!");
			}
			*iResult = ERROR_NO_BATTERY;
			return false;
		}
		if (itCharger->second.isCharging)
		{
			if (showDebugInfo)
			{
				printfDebugInfo(strBatteryId + "正在充电", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "正在充电，请先停止充电!");
			}
			*iResult = ERROR_BATTERY_CHARGING;
			return false;
		}
		if (itCharger->second.isDisCharging)
		{
			if (showDebugInfo)
			{
				printfDebugInfo(strBatteryId + "正在放电", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "正在放电!");
			}
			*iResult = ERROR_BATTERY_CHARGING;
			return false;
		}
	}
	else{
		*iResult = ERROR_CONFIG;
		return false;
	} 
	return true;
}

//根据ID，拼装充电命令，插入命令队列
bool charging::chargingByLocalID(QString strBatteryId, int *iResult, bool showDebugInfo)
{

	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
	if (getBatteryIdRelatedInfo(strBatteryId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
	{
		if (!detectChargingCondition(strBatteryId, iResult, showDebugInfo))
		{
			return false;
		} 
		//设置充电标志
		itCharger->second.isCharging = true;
		itCharger->second.timeLockChargingState.restart();
		//开始充电计时
		itCharger->second.updateBeginChargeTime();

		//组装充电信息
		QString strConnectType = itBatteryModel->second.connectType;
		QString strChargingVol; strChargingVol.sprintf("%5.2f", itBatteryModel->second.chargingVoltage);
		QString strChargingCur; strChargingCur.sprintf("%5.2f", itBatteryModel->second.chargingCurrent);
		QString strCommand = "O,";
		strCommand += QString::number(itCharger->second.id) + ",";//no_zero(strId);
		strCommand += strConnectType.left(2) + ",";
		strCommand += strChargingVol.trimmed() + ",";
		strCommand += strChargingCur.trimmed() + ",";
		QString command = packageCommand(strCommand);
		toSend(strCommand, stCommand::hight);

		//设置充电标志
		itCharger->second.isCharging = true; 
	} 

	return true;
}
//根据电池ID，拼装停止命令，插入命令队列
bool charging::stopByLocalID(QString strBatteryId)
{
	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
	if (getBatteryIdRelatedInfo(strBatteryId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
	{
		if (itCharger->second.isCharging == false){
			return false;
		}
		QVector<stCommand> vtStCommand;
		//设置充电标志:空闲
		itCharger->second.isCharging = false;
		itCharger->second.timeLockChargingState.restart();
		//组装停止命令 添加到命令队列 
		stCommand stComm(packageCommand("P," + QString::number(itCharger->second.id) + ","), stCommand::hight);
		vtStCommand.append(stComm);
		if (vtStCommand.size())
			m_CommandQueue.addVtCommand(vtStCommand);
	} 	
	return true;
}
//当未检测到电池“未放置电池”，充电器充电时“充电中”， 电池满电且不在充电状态“充电完成”,其余状态显示“等待中”
QString charging::getBatteryState(int indexBattery)
{
	QString strResult;
	QString strBState = battery_state[indexBattery];//"电池在线" : "未放置电池";
	float fvol = battery_voltage[indexBattery].toFloat();  //电压
	QString strBatteryId = battery_local_id[indexBattery];//电池id
	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
	if (getBatteryIdRelatedInfo(strBatteryId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
	{
		if (itCharger->second.isCharging == true)//充电器 充电中
		{
			strResult = "充电中";
		}
		else  //充电器 未充电
		{
			if (strBState == "未放置电池"){
				strResult = strBState;
			}
			else{
				if (fvol == 0 || fvol < itBatteryModel->second.fullVoltage - 0.2)  //比较电压
					strResult = "等待中";
				else if (fvol >= itBatteryModel->second.fullVoltage - 0.2){
					strResult = "充电完成";
				}
			} 
		}
	}
 
	return strResult;
}

void charging::show_temperature(float ftemp, bool high)
{
	QString message; message.sprintf("%5.2f", ftemp);
	if(ftemp < 5)
		ui.lab_temperature_imge->setStyleSheet("QLabel{border-image: url(" + QString(g_AppPath) + "/img/temperature_disenable.png);}");
	else{
		if (high || ftemp >=40)
			ui.lab_temperature_imge->setStyleSheet("QLabel{border-image: url(" + QString(g_AppPath) + "/img/temperature_high.png);}");
		else
			ui.lab_temperature_imge->setStyleSheet("QLabel{border-image: url(" + QString(g_AppPath) + "/img/temperature_normal.png);}");
	}
	
	ui.lab_temperature_number->setText(message);
}

bool charging::nativeEvent(const QByteArray &eventType, void *message, long *result)//事件抓捕，处理进程通信
{
	MSG *param = static_cast<MSG *>(message);

	switch (param->message)
	{
		case WM_COPYDATA:
		{
			COPYDATASTRUCT *cds = reinterpret_cast<COPYDATASTRUCT*>(param->lParam);
			if (cds->dwData == CUSTOM_TYPE)
			{
				stProcessSimpltData *stObj = reinterpret_cast<stProcessSimpltData *>(cds->lpData);
				QString strMessage = QString::fromUtf8(stObj->strInfo);//QString::fromUtf8(<char*>(cds->lpData), cds->cbData);
				//QMessageBox::information(this, QStringLiteral("提示"), strMessage);
				printfDebugInfo( strMessage+",重新读取中。", enDebugInfoPriority::DebugInfoLevelOne);
				//添加电池型号更新判断 add20180418
				MAP_BATTERY mapBatteryOld = m_mapBattery;
				//m_vtUiChargGrid[uiIndex]->setChargerState([index]);				
				readConfig();
				m_nChargeLimitTime = stObj->nTime1; 
				m_SubmitInterval = stObj->nSubmitTime;	
				m_fOverHeatTemperature = stObj->fOverHeatTemperature;
				refreshCurrentUI();
				show_all_data_now(); 
				
				//对比后提交一次型号数据到服务器
				submitBatteryModel(m_mapBattery, mapBatteryOld);
				*result = 1;
				return true;
			}
		}
	}

	return QWidget::nativeEvent(eventType, message, result);
}


bool charging::getRelatedChargerOnline(QString strBatteryId)
{
	//
	int nBatteryId = strBatteryId.toInt();

	MAP_CLOSET_IT itCloset = m_mapCloset.find(nBatteryId / 100);
	if (itCloset != m_mapCloset.end()){
		MAP_BATTERY_IT itBattery = itCloset->second.mapBattery.find(nBatteryId);
		if (itBattery != itCloset->second.mapBattery.end()){
			MAP_CHARGER_IT itCharger = itCloset->second.mapCharger.find(itBattery->second.relatedCharger);
			if (itCharger != itCloset->second.mapCharger.end()){
				return itCharger->second.bOnline;
			}
		}
	}
	return false;		//错误判断
}



void charging::OnAddCommamdIntoQueue(stCommand strCommand)
{
	m_CommandQueue.addCommamd(strCommand);
}

//完成读取一个柜子编号 ，发送下一个柜子读取信息命令
void charging::OnReadyGetBatteryState(int nClosetId)  
{
	//static QVector<unsigned int > vtClosetId;  vtClosetId.clear();
	//static int nLoopClosetId =1, nNextClosetId;
	//
	//for (MAP_CLOSET_IT itCloset = m_mapCloset.begin(); itCloset != m_mapCloset.end(); itCloset++){
	//	vtClosetId.append(itCloset->first);
	//}
	//if (nClosetId == m_iCurrentCloset){
	//	//读取下一个柜子		
	//	nLoopClosetId++;
	//	if (nLoopClosetId > vtClosetId.size())
	//		nLoopClosetId = 1;
	//	if (nLoopClosetId == m_iCurrentCloset &&  nLoopClosetId <= vtClosetId.size() -1)
	//		nLoopClosetId++; 
	//	nNextClosetId = vtClosetId[nLoopClosetId - 1];
	//	scanOneBatteryState(nNextClosetId, stCommand::normal);
	//}
	//else
	{
		//读取当前柜子 
		scanOneBatteryState(m_iCurrentCloset, stCommand::normal);
	}  
}

void charging::appendChargingQueue(QString strBatteryId)
{
	//比较电池ID,保存申请未能充电的数据
	bool bSame = false;
	for (int i = 0; i < m_vtApplyDontCharge.size(); i++){
		if (m_vtApplyDontCharge[i].nBatteryID == strBatteryId.toInt())
		{
			bSame = true; //如果已经有相同的，不保存
			break;
		}
	}
	if (!bSame)//电池ID不相同，则保存
	{
		stApplyBatteryDontCharge obj;
		obj.nBatteryID = strBatteryId.toInt();
		obj.strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
		m_vtApplyDontCharge.append(obj);
	}
}

//取消待充电队列对应的id
void charging::removeChargingQueue(QString strBatteryId)
{
	for (int i = m_vtApplyDontCharge.size() - 1; i >=0 ; i--){
		if (m_vtApplyDontCharge[i].nBatteryID == strBatteryId.toInt())
		{
			m_vtApplyDontCharge.remove(i);
		}
	}
}

//保存到文件
void charging::writeApplyChargingQueueToFile()
{
	//比较 申请未充电的备份 是否有不同，不同则跟新到本地文件	
	 
	bool bSame = true;
	if (m_vtApplyDontChargeB.size() != m_vtApplyDontCharge.size()){
		bSame = false;
	}
	else{
		for (int i = 0; i < m_vtApplyDontCharge.size(); i++){
			if (!m_vtApplyDontCharge[i].compare(m_vtApplyDontChargeB[i]))
			{
				bSame = false;
				break;
			}
		}
	}
	if (bSame == false){
		int iError = 0;
		COperatorFile::GetInstance()->writeApplyBatteryToCharging(m_vtApplyDontCharge, &iError);
		if (iError == 0){
			m_vtApplyDontChargeB = m_vtApplyDontCharge;
		}
	}
	/*else if (m_vtApplyDontCharge.size() == 0){
	int iError = 0;
	COperatorFile::GetInstance()->writeApplyBatteryToCharging(m_vtApplyDontCharge, &iError);
	}*/
}
//提取数字
void charging::extractNumber(QString strSrouce, int& iNumber)  
{
	char a[50] = {0};
	char b[50];
	char*  ch;
	QByteArray ba = strSrouce.toLocal8Bit(); // must
	strncpy_s(a, 50, ba.data(), 50);//
	int cnt_index = 0, cnt_int = 0;
	//cnt_int 用于存放字符串中的数字.
	//cnt_index 作为字符串b的下标.

	for (int i = 0; i < strlen(a); ++i)
	{
		if (a[i] >= '0'&& a[i] <= '9') //如果是数字.
		{
			cnt_int *= 10;
			cnt_int += a[i] - '0'; //数字字符的ascii-字符'0'的ascii码就等于该数字.
		}

		else if ((a[i] >= 'a'&&a[i] <= 'z') || (a[i] >= 'A'&&a[i] <= 'Z')) //如果是字母.
		{
			b[cnt_index++] = a[i]; //如果是字符,则增加到b数组中.
		}
	}
	iNumber = cnt_int;
	b[cnt_index++] = '\0'; //增加字符串结束符.

}