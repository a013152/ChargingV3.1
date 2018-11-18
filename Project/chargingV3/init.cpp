#include "charging.h"
#pragma execution_character_set("utf-8")



void charging::init_now()
{ 	
	//设置 选中 充电柜页面
	//ui.tabWidget->setCurrentIndex(1);
	QTime qtime1; qtime1.start(); 

	QObject::connect(&m_ConnectDBThread, &CConnectDBThread::connectDBResult, this, &charging::OnConnectDBResult);
	 
	//运行线程 测试连接服务器数据库
	m_ConnectDBThread.start(QThread::NormalPriority);
	m_ConnectDBThread.doTestConnectDB();
	m_ConnectDBThread.setMainProcess(this);

	if(m_OperDB.onOpenDbFile() == false)
		printfDebugInfo("打开Sqlite失败，充电记录失效！", DebugInfoLevelOne, true);
	printfDebugInfo("01开启测试连接服务器线程，耗时：" + QString::number(qtime1.elapsed() / 1000) + "秒" + QString::number(qtime1.elapsed() % 1000) + "毫秒");
	qtime1.restart();

	//初始化定时器
	initTimer();
	//printfDebugInfo("03初始化定时器，耗时：" + QString::number(qtime1.elapsed() / 1000) + "秒" + QString::number(qtime1.elapsed() % 1000) + "毫秒");
	//qtime1.restart();

	//开启命令队列线程
	m_CommandQueue.start(QThread::NormalPriority);   
	//printfDebugInfo("04开启命令队列线程，耗时：" + QString::number(qtime1.elapsed() / 1000) + "秒" + QString::number(qtime1.elapsed() % 1000) + "毫秒");
	//qtime1.restart();

	//创建柜子编号控件
	createClosetRadio();
	//printfDebugInfo("05创建柜子编号控件，耗时：" + QString::number(qtime1.elapsed() / 1000) + "秒" + QString::number(qtime1.elapsed() % 1000) + "毫秒");
	//qtime1.restart();
	//创建充电格控件+ 充电器层级
	createChargGrid();
	//printfDebugInfo("06创建充电格控件，耗时：" + QString::number(qtime1.elapsed() / 1000) + "秒" + QString::number(qtime1.elapsed() % 1000) + "毫秒");
	//qtime1.restart(); 

	//创建调试信息窗口
	createDebugInfoUI();

	//创建操作、串口菜单
	QAction * actionBase = createMenus();
	//printfDebugInfo("07创建操作、串口菜单，耗时：" + QString::number(qtime1.elapsed() / 1000) + "秒" + QString::number(qtime1.elapsed() % 1000) + "毫秒");
	//qtime1.restart();

	
	//调整ui位置，与加载图片
	adjustUI(); 
	//printfDebugInfo("08调整ui位置，与加载图片，耗时：" + QString::number(qtime1.elapsed() / 1000) + "秒" + QString::number(qtime1.elapsed() % 1000) + "毫秒");
	//qtime1.restart();	
	
	//连接控件响应函数
	initConnectWidget();
	//printfDebugInfo("09连接控件响应函数，耗时：" + QString::number(qtime1.elapsed() / 1000) + "秒" + QString::number(qtime1.elapsed() % 1000) + "毫秒");
	//qtime1.restart();
	printfDebugInfo("***********初始化完成**************");

	//打开串口
	if (actionBase != NULL){
		//打开串口
		OnClickMenuCom(actionBase);
	} 
	//打开can进程
	onOpenOrCloseCanDevice(true);
	if (isOpenSerialPort == false && isOpenCANProcess == false){
		printfDebugInfo("***********未打开串口或者CAN设备**************", DebugInfoLevelOne, true);
	}
	else if (isOpenSerialPort || isOpenCANProcess){
		printfDebugInfo("***********开始扫描设备**************");

		//开始扫描设备
		beginScanBatteryState();
		//开始定时器
		if ((isOpenSerialPort || isOpenCANProcess) && !meTimer->isActive())
			meTimer->start();
	}
	
}

//初始化读取配置
void charging::readConfig()
{ 
	//清理内存
	m_mapCloset.clear(); m_mapBattery.clear(); m_mapBatteryModel.clear(); m_mapCharger.clear(); 
	//if (init_)
	{
		battery_my_id.clear();	battery_local_id.clear();	battery_type.clear();
		battery_state.clear();	charger_state.clear();	battery_temperature.clear();
		battery_voltage.clear();	battery_current.clear();	battery_balance.clear();
		battery_state_enable_refresh.clear();  //能否刷新充电器状态
		battery_charging_record.clear(); //充电记录记录， 当下位机返回充电成功后，设置true， 提交到数据库后设置false 
		battery_apply_charging.clear();  //充电情况
		m_vtApplyDontCharge.clear();  //申请还没充电的记录
	}
	

	COperatorFile::GetInstance()->setAppPath(g_AppPath);
	COperatorFile::GetInstance()->readAllConfig(m_mapCloset, m_mapBattery, m_mapBatteryModel, m_mapCharger,    &m_iError);
	
	COperatorFile::GetInstance()->tryCreateLogFile();
	if (m_iError == 0)
	{
		
		//整合配置内容
		combineConfig(m_mapCloset, m_mapBattery, m_mapBatteryModel, m_mapCharger, m_mapLevel);
		//if (init_)
		{
			for (MAP_BATTERY_IT it = m_mapBattery.begin(); it != m_mapBattery.end(); it++ )
			{ 
				stApplyInfo applyInfo;
				applyInfo.myID = it->second.dbid;
				battery_apply_charging.append(applyInfo);

				QString temp = QString::number(it->second.dbid); 
				battery_my_id.append(temp);
				temp = it->second.id; temp = temp.trimmed(); temp = temp.left(4);
				battery_local_id.append(temp);
				MAP_BATTERY_MODEL_IT it2 = m_mapBatteryModel.find(it->second.modelId);
				if (it2 != m_mapBatteryModel.end()) 
					battery_type.append(it2->second.droneModel);   //根据电池型号确定无人机型号
				if (it2->second.balance)
					battery_state.append("未放置电池");
				else
					battery_state.append("未放置电池");
				charger_state.append(STATE_OFFLINE);
				battery_temperature.append("0.0");		  //温度		 	
				battery_voltage.append("0.0");//电压			
				battery_current.append("0.0");
				if (it2!= m_mapBatteryModel.end())
					battery_balance.append(it2->second.balance);

				battery_charging_record.append(false); //充电记录
				battery_state_enable_refresh.append(true);
			} 
		}
	
	} 
	COperatorFile::GetInstance()->readApplyBatteryToCharging(m_vtApplyDontCharge, &m_iError);
	m_vtApplyDontChargeB = m_vtApplyDontCharge;
	//读取继续扫描标志
	m_bContinueScan = CReadIniFile::getInstance()->readProfileInfo("SET", "ContinueScan", QString(g_AppPath) + "\\set.ini", &iError).toInt() == 1;
	//读取继续网络提交标志
	m_bContinueSubmit = CReadIniFile::getInstance()->readProfileInfo("SET", "ContinueSubmit", QString(g_AppPath) + "\\set.ini", &iError).toInt() == 1;

	//提交间隔时间
	m_SubmitInterval = CReadIniFile::getInstance()->readProfileInfo("SET", "SubmitInterval", QString(g_AppPath) + "\\set.ini", &iError).toInt();
	if (m_SubmitInterval == 0){
		m_SubmitInterval = ONE_SUBMIT_TIME;
		//printfDebugInfo("SET SubmitInterval 未设置提交时间，默认15秒提交一次", enDebugInfoPriority::DebugInfoLevelOne, true);
	}

	//扫描间隔时间
	m_ScanDeviceInterval = CReadIniFile::getInstance()->readProfileInfo("SET", "ScanDeviceInterval", QString(g_AppPath) + "\\set.ini", &iError).toInt();
	if (m_ScanDeviceInterval == 0){
		m_ScanDeviceInterval = ONE_CLOSET_SCAN_TIME;
		//printfDebugInfo("SET ScanDeviceInterval 未设置扫描时间，默认5秒一柜一次", enDebugInfoPriority::DebugInfoLevelOne, true);
	} 
	//过热温度
	m_fOverHeatTemperature = CReadIniFile::getInstance()->readProfileInfo("SET", "OverHeatTemperature", QString(g_AppPath) + "\\set.ini", &iError).toInt();
	if (m_fOverHeatTemperature == 0){
		m_fOverHeatTemperature = ONE_CLOSET_SCAN_TIME;
		//printfDebugInfo("SET OverHeatTemperature 未设置过热温度，默认45度", enDebugInfoPriority::DebugInfoLevelOne, true);
	}
	//自动放电天数
	m_nDischargeDay = CReadIniFile::getInstance()->readProfileInfo("SET", "DisChargerDay", QString(g_AppPath) + "\\set.ini", &iError).toInt();
	if (m_nDischargeDay == 0){
		m_nDischargeDay = DISCHARGING_DAY;
	}

	//读取调试信息显示等级
	m_enShowDebugInfoLevel = (enDebugInfoPriority)(CReadIniFile::getInstance()->readProfileInfo("WidgetShow", "debugInfoLevel", QString(g_AppPath) + "\\set.ini", &iError).toInt());

	//读取调试信息自动隐藏标志
	m_bAutoHideDebugInfo = (CReadIniFile::getInstance()->readProfileInfo("WidgetShow", "AutoHideDebugInfo", QString(g_AppPath )+ "\\set.ini", &iError).toInt()) == 1;

	m_bCanMoveWindow = (CReadIniFile::getInstance()->readProfileInfo("WidgetShow", "moveWindow", QString(g_AppPath) + "\\set.ini", &iError).toInt()) == 1;

	//读取充电时限
	m_nChargeLimitTime = (CReadIniFile::getInstance()->readProfileInfo("SET", "ChargeLimitTime", QString(g_AppPath) + "\\set.ini", &iError).toInt());
	if (m_nChargeLimitTime == 0){
		m_nChargeLimitTime = CHARGING_LIMIT_TIME;
		//printfDebugInfo("SET m_nChargeLimitTime 未设置充电时限，默认360分钟", enDebugInfoPriority::DebugInfoLevelOne, true);
	}
}


void charging::initConnectWidget()
{	 
	//槽函数连接 
	//QObject::connect(ui.btnPauseScand, &QPushButton::clicked, this, &charging::OnBtnPauseScand);	
	QObject::connect(ui.btnPrePage, &QPushButton::clicked, this, &charging::OnClickPrePage);
	QObject::connect(ui.btnNextPage, &QPushButton::clicked, this, &charging::OnClickNextPage);
	
	QObject::connect(ui.btnMenuSys, &QPushButton::clicked, this, &charging::OnBtnShowSysMenu);
	QObject::connect(ui.btnOneKey, &QPushButton::clicked, this, &charging::onOneKeyCharger);
	QObject::connect(ui.btnSysMin, &QPushButton::clicked, this, &charging::OnBtnSysMin);
	QObject::connect(ui.btnSysClose, &QPushButton::clicked, this, &charging::OnBtnSysClose);

	QObject::connect(m_BtnMin_DebugInfo, &QPushButton::clicked, this, &charging::OnBtnHideDebugInfo);
	QObject::connect(m_CheckBox_DebugInfo, &QCheckBox::stateChanged, this, &charging::OnCheckAutoShowDebugInfo);



	QObject::connect(&m_CommandQueue, SIGNAL(readed(QString, QString, int)), this, SLOT(readSerial(QString, QString, int)));
	QObject::connect(&m_CommandQueue, SIGNAL(readedCAN(QString)), this, SLOT(onReadCAN(QString)));
	QObject::connect(&m_CommandQueue, SIGNAL(printfed(QString)), this, SLOT(updateTextEdit(QString)));
	QObject::connect(&m_CommandQueue, SIGNAL(readyGetBatteryState( int)), this, SLOT(OnReadyGetBatteryState( int)));
	QObject::connect(this, SIGNAL(printfed(QString)), this, SLOT(updateTextEdit(QString)));
	QObject::connect(this, SIGNAL(refreshUI(QString)), this, SLOT(onRefreshUI(QString)));
	 
	QObject::connect(this, &charging::ExitApp, this, &charging::OnExitApp);

	QObject::connect(this, &charging::RefreshState, this, &charging::OnRefreshState);

	QObject::connect(this, SIGNAL(AddCommamdIntoQueue(stCommand)), this, SLOT(OnAddCommamdIntoQueue(stCommand)));

	QObject::connect(&m_ThreadHideOrShowDebugInfo, &CThreadShowOrHideDebugInfo::signalAdjustRect, this, &charging::OnAdjustDebugInfoRect);

	
}
//整合配置内容,
void charging::combineConfig(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery, MAP_BATTERY_MODEL& mapBatteryModel, 
	MAP_CHARGER& mapCharger, MAP_LEVEL& mapLevel)
{
	int nTempId = 0;
	QString strTemp; 
	MAP_CHARGER_IT itCharger;
	MAP_BATTERY_IT itBattery;
	MAP_CLOSET_IT itCloset;
	MAP_LEVEL_IT itLevel;

	//单个柜子
	if (m_mapCloset.size() == 1)
		itCloset = m_mapCloset.begin();

	for (itCharger = m_mapCharger.begin(); itCharger != m_mapCharger.end(); itCharger++)
	{
		// 关联充电器与充电柜	
		stCharger obj(itCharger->second);
		itCloset->second.mapCharger.insert(MAP_CHARGER::value_type(obj.id, obj));
		
		//根据充电器的nLevel创建层级
		itLevel = mapLevel.find(itCharger->second.nLevel);
		if (itLevel != mapLevel.end()){
			stCharger obj(itCharger->second);
			itLevel->second.mapCharger.insert(MAP_CHARGER::value_type(obj.id, obj));
		}
		else{
			stLevel objLevel; stCharger objCharger(itCharger->second);
			objLevel.nLevel = itCharger->second.nLevel;			
			objLevel.mapCharger.insert(MAP_CHARGER::value_type(objCharger.id, objCharger));
			mapLevel.insert(MAP_LEVEL::value_type(objLevel.nLevel, objLevel));
		}
	}

	for (itBattery = m_mapBattery.begin(); itBattery != m_mapBattery.end(); itBattery++)
	{
		//关联电池与充电器
		itCharger = m_mapCharger.find(itBattery->second.relatedCharger);
		if (itCharger != m_mapCharger.end()){
			
			//电池再根据充电器，关联层级
			itLevel = mapLevel.find(itCharger->second.nLevel);
			if (itLevel != mapLevel.end()){
				stBatteryInfo obj(itBattery->second);
				itLevel->second.mapBattery.insert(MAP_BATTERY::value_type(atoi(obj.id), obj));
			}

		}
		// 关联电池与充电柜   
		stBatteryInfo obj(itBattery->second);
		itCloset->second.mapBattery.insert(MAP_BATTERY::value_type(atoi(obj.id), obj));
	}
	
	return; 
}  

void  charging::initTimer()
{ 
	//初始化定时器
	if (meTimer == NULL)
		meTimer = new QTimer();
	meTimer->setInterval(ONE_CLOSET_SCAN_TIME); 
	QObject::connect(meTimer, SIGNAL(timeout()), this, SLOT(timer_out())); 
	//菜单 串口点击事件OnClickMenuCom 中开始定时器。meTimer->start();
}

void charging::OnConnectDBResult(bool  bResult)
{
	QString strInfo;
	if (bResult)
	{
		m_bConnectServerIsSeccuss = true; 
		strInfo += "网络畅通，服务器"; strInfo += DBHOSTNAME; strInfo += "连接成功！";
		printfDebugInfo(strInfo, enDebugInfoPriority::DebugInfoLevelOne);
		m_submitServer.msg_open();
		//提交一次电池型号
		static bool frist = true;
		if (frist){
			frist = false;
			MAP_BATTERY mapOld;
			submitBatteryModel(m_mapBattery, mapOld);
		}
	}
	else
	{ 
		strInfo += "网络异常，服务器"; strInfo += DBHOSTNAME; strInfo += "连接失败！";
		printfDebugInfo(strInfo, enDebugInfoPriority::DebugInfoLevelOne, true);
	}
}



