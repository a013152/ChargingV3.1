#include "charging.h"
#pragma execution_character_set("utf-8")



void charging::init_now()
{ 	
	//���� ѡ�� ����ҳ��
	//ui.tabWidget->setCurrentIndex(1);
	QTime qtime1; qtime1.start(); 

	QObject::connect(&m_ConnectDBThread, &CConnectDBThread::connectDBResult, this, &charging::OnConnectDBResult);
	 
	//�����߳� �������ӷ��������ݿ�
	m_ConnectDBThread.start(QThread::NormalPriority);
	m_ConnectDBThread.doTestConnectDB();
	m_ConnectDBThread.setMainProcess(this);

	if(m_OperDB.onOpenDbFile() == false)
		printfDebugInfo("��Sqliteʧ�ܣ�����¼ʧЧ��", DebugInfoLevelOne, true);
	printfDebugInfo("01�����������ӷ������̣߳���ʱ��" + QString::number(qtime1.elapsed() / 1000) + "��" + QString::number(qtime1.elapsed() % 1000) + "����");
	qtime1.restart();

	//��ʼ����ʱ��
	initTimer();
	//printfDebugInfo("03��ʼ����ʱ������ʱ��" + QString::number(qtime1.elapsed() / 1000) + "��" + QString::number(qtime1.elapsed() % 1000) + "����");
	//qtime1.restart();

	//������������߳�
	m_CommandQueue.start(QThread::NormalPriority);   
	//printfDebugInfo("04������������̣߳���ʱ��" + QString::number(qtime1.elapsed() / 1000) + "��" + QString::number(qtime1.elapsed() % 1000) + "����");
	//qtime1.restart();

	//�������ӱ�ſؼ�
	createClosetRadio();
	//printfDebugInfo("05�������ӱ�ſؼ�����ʱ��" + QString::number(qtime1.elapsed() / 1000) + "��" + QString::number(qtime1.elapsed() % 1000) + "����");
	//qtime1.restart();
	//��������ؼ�+ ������㼶
	createChargGrid();
	//printfDebugInfo("06��������ؼ�����ʱ��" + QString::number(qtime1.elapsed() / 1000) + "��" + QString::number(qtime1.elapsed() % 1000) + "����");
	//qtime1.restart(); 

	//����������Ϣ����
	createDebugInfoUI();

	//�������������ڲ˵�
	QAction * actionBase = createMenus();
	//printfDebugInfo("07�������������ڲ˵�����ʱ��" + QString::number(qtime1.elapsed() / 1000) + "��" + QString::number(qtime1.elapsed() % 1000) + "����");
	//qtime1.restart();

	
	//����uiλ�ã������ͼƬ
	adjustUI(); 
	//printfDebugInfo("08����uiλ�ã������ͼƬ����ʱ��" + QString::number(qtime1.elapsed() / 1000) + "��" + QString::number(qtime1.elapsed() % 1000) + "����");
	//qtime1.restart();	
	
	//���ӿؼ���Ӧ����
	initConnectWidget();
	//printfDebugInfo("09���ӿؼ���Ӧ��������ʱ��" + QString::number(qtime1.elapsed() / 1000) + "��" + QString::number(qtime1.elapsed() % 1000) + "����");
	//qtime1.restart();
	printfDebugInfo("***********��ʼ�����**************");

	//�򿪴���
	if (actionBase != NULL){
		//�򿪴���
		OnClickMenuCom(actionBase);
	} 
	//��can����
	onOpenOrCloseCanDevice(true);
	if (isOpenSerialPort == false && isOpenCANProcess == false){
		printfDebugInfo("***********δ�򿪴��ڻ���CAN�豸**************", DebugInfoLevelOne, true);
	}
	else if (isOpenSerialPort || isOpenCANProcess){
		printfDebugInfo("***********��ʼɨ���豸**************");

		//��ʼɨ���豸
		beginScanBatteryState();
		//��ʼ��ʱ��
		if ((isOpenSerialPort || isOpenCANProcess) && !meTimer->isActive())
			meTimer->start();
	}
	
}

//��ʼ����ȡ����
void charging::readConfig()
{ 
	//�����ڴ�
	m_mapCloset.clear(); m_mapBattery.clear(); m_mapBatteryModel.clear(); m_mapCharger.clear(); 
	//if (init_)
	{
		battery_my_id.clear();	battery_local_id.clear();	battery_type.clear();
		battery_state.clear();	charger_state.clear();	battery_temperature.clear();
		battery_voltage.clear();	battery_current.clear();	battery_balance.clear();
		battery_state_enable_refresh.clear();  //�ܷ�ˢ�³����״̬
		battery_charging_record.clear(); //����¼��¼�� ����λ�����س��ɹ�������true�� �ύ�����ݿ������false 
		battery_apply_charging.clear();  //������
		m_vtApplyDontCharge.clear();  //���뻹û���ļ�¼
	}
	

	COperatorFile::GetInstance()->setAppPath(g_AppPath);
	COperatorFile::GetInstance()->readAllConfig(m_mapCloset, m_mapBattery, m_mapBatteryModel, m_mapCharger,    &m_iError);
	
	COperatorFile::GetInstance()->tryCreateLogFile();
	if (m_iError == 0)
	{
		
		//������������
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
					battery_type.append(it2->second.droneModel);   //���ݵ���ͺ�ȷ�����˻��ͺ�
				if (it2->second.balance)
					battery_state.append("δ���õ��");
				else
					battery_state.append("δ���õ��");
				charger_state.append(STATE_OFFLINE);
				battery_temperature.append("0.0");		  //�¶�		 	
				battery_voltage.append("0.0");//��ѹ			
				battery_current.append("0.0");
				if (it2!= m_mapBatteryModel.end())
					battery_balance.append(it2->second.balance);

				battery_charging_record.append(false); //����¼
				battery_state_enable_refresh.append(true);
			} 
		}
	
	} 
	COperatorFile::GetInstance()->readApplyBatteryToCharging(m_vtApplyDontCharge, &m_iError);
	m_vtApplyDontChargeB = m_vtApplyDontCharge;
	//��ȡ����ɨ���־
	m_bContinueScan = CReadIniFile::getInstance()->readProfileInfo("SET", "ContinueScan", QString(g_AppPath) + "\\set.ini", &iError).toInt() == 1;
	//��ȡ���������ύ��־
	m_bContinueSubmit = CReadIniFile::getInstance()->readProfileInfo("SET", "ContinueSubmit", QString(g_AppPath) + "\\set.ini", &iError).toInt() == 1;

	//�ύ���ʱ��
	m_SubmitInterval = CReadIniFile::getInstance()->readProfileInfo("SET", "SubmitInterval", QString(g_AppPath) + "\\set.ini", &iError).toInt();
	if (m_SubmitInterval == 0){
		m_SubmitInterval = ONE_SUBMIT_TIME;
		//printfDebugInfo("SET SubmitInterval δ�����ύʱ�䣬Ĭ��15���ύһ��", enDebugInfoPriority::DebugInfoLevelOne, true);
	}

	//ɨ����ʱ��
	m_ScanDeviceInterval = CReadIniFile::getInstance()->readProfileInfo("SET", "ScanDeviceInterval", QString(g_AppPath) + "\\set.ini", &iError).toInt();
	if (m_ScanDeviceInterval == 0){
		m_ScanDeviceInterval = ONE_CLOSET_SCAN_TIME;
		//printfDebugInfo("SET ScanDeviceInterval δ����ɨ��ʱ�䣬Ĭ��5��һ��һ��", enDebugInfoPriority::DebugInfoLevelOne, true);
	} 
	//�����¶�
	m_fOverHeatTemperature = CReadIniFile::getInstance()->readProfileInfo("SET", "OverHeatTemperature", QString(g_AppPath) + "\\set.ini", &iError).toInt();
	if (m_fOverHeatTemperature == 0){
		m_fOverHeatTemperature = ONE_CLOSET_SCAN_TIME;
		//printfDebugInfo("SET OverHeatTemperature δ���ù����¶ȣ�Ĭ��45��", enDebugInfoPriority::DebugInfoLevelOne, true);
	}
	//�Զ��ŵ�����
	m_nDischargeDay = CReadIniFile::getInstance()->readProfileInfo("SET", "DisChargerDay", QString(g_AppPath) + "\\set.ini", &iError).toInt();
	if (m_nDischargeDay == 0){
		m_nDischargeDay = DISCHARGING_DAY;
	}

	//��ȡ������Ϣ��ʾ�ȼ�
	m_enShowDebugInfoLevel = (enDebugInfoPriority)(CReadIniFile::getInstance()->readProfileInfo("WidgetShow", "debugInfoLevel", QString(g_AppPath) + "\\set.ini", &iError).toInt());

	//��ȡ������Ϣ�Զ����ر�־
	m_bAutoHideDebugInfo = (CReadIniFile::getInstance()->readProfileInfo("WidgetShow", "AutoHideDebugInfo", QString(g_AppPath )+ "\\set.ini", &iError).toInt()) == 1;

	m_bCanMoveWindow = (CReadIniFile::getInstance()->readProfileInfo("WidgetShow", "moveWindow", QString(g_AppPath) + "\\set.ini", &iError).toInt()) == 1;

	//��ȡ���ʱ��
	m_nChargeLimitTime = (CReadIniFile::getInstance()->readProfileInfo("SET", "ChargeLimitTime", QString(g_AppPath) + "\\set.ini", &iError).toInt());
	if (m_nChargeLimitTime == 0){
		m_nChargeLimitTime = CHARGING_LIMIT_TIME;
		//printfDebugInfo("SET m_nChargeLimitTime δ���ó��ʱ�ޣ�Ĭ��360����", enDebugInfoPriority::DebugInfoLevelOne, true);
	}
}


void charging::initConnectWidget()
{	 
	//�ۺ������� 
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
//������������,
void charging::combineConfig(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery, MAP_BATTERY_MODEL& mapBatteryModel, 
	MAP_CHARGER& mapCharger, MAP_LEVEL& mapLevel)
{
	int nTempId = 0;
	QString strTemp; 
	MAP_CHARGER_IT itCharger;
	MAP_BATTERY_IT itBattery;
	MAP_CLOSET_IT itCloset;
	MAP_LEVEL_IT itLevel;

	//��������
	if (m_mapCloset.size() == 1)
		itCloset = m_mapCloset.begin();

	for (itCharger = m_mapCharger.begin(); itCharger != m_mapCharger.end(); itCharger++)
	{
		// ��������������	
		stCharger obj(itCharger->second);
		itCloset->second.mapCharger.insert(MAP_CHARGER::value_type(obj.id, obj));
		
		//���ݳ������nLevel�����㼶
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
		//�������������
		itCharger = m_mapCharger.find(itBattery->second.relatedCharger);
		if (itCharger != m_mapCharger.end()){
			
			//����ٸ��ݳ�����������㼶
			itLevel = mapLevel.find(itCharger->second.nLevel);
			if (itLevel != mapLevel.end()){
				stBatteryInfo obj(itBattery->second);
				itLevel->second.mapBattery.insert(MAP_BATTERY::value_type(atoi(obj.id), obj));
			}

		}
		// ������������   
		stBatteryInfo obj(itBattery->second);
		itCloset->second.mapBattery.insert(MAP_BATTERY::value_type(atoi(obj.id), obj));
	}
	
	return; 
}  

void  charging::initTimer()
{ 
	//��ʼ����ʱ��
	if (meTimer == NULL)
		meTimer = new QTimer();
	meTimer->setInterval(ONE_CLOSET_SCAN_TIME); 
	QObject::connect(meTimer, SIGNAL(timeout()), this, SLOT(timer_out())); 
	//�˵� ���ڵ���¼�OnClickMenuCom �п�ʼ��ʱ����meTimer->start();
}

void charging::OnConnectDBResult(bool  bResult)
{
	QString strInfo;
	if (bResult)
	{
		m_bConnectServerIsSeccuss = true; 
		strInfo += "���糩ͨ��������"; strInfo += DBHOSTNAME; strInfo += "���ӳɹ���";
		printfDebugInfo(strInfo, enDebugInfoPriority::DebugInfoLevelOne);
		m_submitServer.msg_open();
		//�ύһ�ε���ͺ�
		static bool frist = true;
		if (frist){
			frist = false;
			MAP_BATTERY mapOld;
			submitBatteryModel(m_mapBattery, mapOld);
		}
	}
	else
	{ 
		strInfo += "�����쳣��������"; strInfo += DBHOSTNAME; strInfo += "����ʧ�ܣ�";
		printfDebugInfo(strInfo, enDebugInfoPriority::DebugInfoLevelOne, true);
	}
}



