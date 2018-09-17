#include "charging.h"
#include "CommandQueue.h"
#pragma execution_character_set("utf-8")
 
  

auto pSetQPushButtonChecked = [](QPushButton *pBtn, bool checked){
	if (checked)
	{
		pBtn->setStyleSheet("QPushButton{font-size:16px; color:white;"
			"border-image: url(" + g_AppPath + "/img/radioChecked_pressd.png);}"
			"QPushButton:hover{border-image: url(" + g_AppPath + "/img/radioChecked_pressd.png);}"
			"QPushButton:pressed{border-image: url(" + g_AppPath + "/img/radioChecked_pressd.png);}");
	}
	else
	{
		pBtn->setStyleSheet("QPushButton{font-size:16px; color:white;"
			"border-image: url(" + g_AppPath + "/img/radioChecked_bg.png);}"
			"QPushButton:hover{border-image: url(" + g_AppPath + "/img/radioChecked_hover.png);}"
			"QPushButton:pressed{border-image: url(" + g_AppPath + "/img/radioChecked_hover.png);}");
	}
};

//�ı���ӱ��
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
//��һҳ
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
//��һҳ
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

//ת����ѹ �� Pro
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

// ���µ�ѹ int ���±�
void charging::show_a_voltage(QString message, int index)
{ 
	if (m_vtUiChargGrid.size()>0 && index < m_vtUiChargGrid.size())
		m_vtUiChargGrid[index]->setVol(message.toFloat());  	  //���õ�ѹ	
}

//ת������ 
int transfCurrent(QString strCur)
{
	// ��ʱ�涨������8A
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
 
//ת���¶� -25��C ��125��C
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

//״̬
void charging::show_a_state(QString message, int indexArray, bool balence, QString chargerState, QString strVol)
{
	emit RefreshState(enRefreshType::BatteryState, indexArray);	
} 

//��ͣɨ��
void charging::onPauseScand(bool checked)
{
	checked;
	QAction * action = (QAction *)QObject::sender();
	if (SERIAL_PORT->isOpen() ){
		m_bContinueScan = !action->isChecked() ;
		//m_bContinueScan Ϊ false ������ɨ�� 
		m_CommandQueue.setPause(m_bContinueScan);
	}
	else{
		action->blockSignals(true);
		action->setChecked(!checked);
		action->blockSignals(false);
		printfDebugInfo("����δ��", enDebugInfoPriority::DebugInfoLevelOne, true);
	}	
	CReadIniFile::getInstance()->writeProfileInfo("SET", "ContinueScan", QString("%1").arg(m_bContinueScan), g_AppPath + "\\set.ini", &iError);

}
//��ͣɨ��
void charging::onPauseSubmit(bool checked)
{
	if (checked) 
		m_bContinueSubmit = false;// ������ɨ��   
	else 
		m_bContinueSubmit = true;// ����ɨ�� 

	CReadIniFile::getInstance()->writeProfileInfo("SET", "ContinueSubmit", QString("%1").arg(m_bContinueSubmit), g_AppPath + "\\set.ini", &iError);

	QString temp = (checked ? "��ͣ" : "����");
	printfDebugInfo(temp + "���紫��", enDebugInfoPriority::DebugInfoLevelOne);
}
 
//�����ô��ڽ���
void charging::onOpenOptionWin( )
{
	QString strExe, strPath, strTemp;
	
	if (m_pProcess == NULL)
		m_pProcess = new QProcess(this);

#if defined(QT_DEBUG)
	strPath = g_AppPath + "/chargingOption.exe";
	strExe = "chargingOption.exe";
#   else
	strExe = "chargingOption.exe";
#  endif

	// �жϽ����Ƿ����
	QProcess tasklist;
	tasklist.start("tasklist",
		QStringList() << "/NH"<< "/FO" << "CSV"	<< "/FI" << QString("IMAGENAME eq %1").arg(strExe));
	tasklist.waitForFinished();
	QByteArray byteArrayOutput = tasklist.readAllStandardOutput();
	QString strOutput = byteArrayOutput;
	if (strOutput.startsWith(QString("\"%1").arg(strExe))) //������ڽ��̣�
	{
		QString strWin = SETING_WINDOW_TITLE;
		QByteArray ba = strWin.toLocal8Bit(); // must
		 
		char* sz = ba.data();
		SetWindowTop(sz);  //�����ö�
		//QProcess taskkill;
		//taskkill.execute("taskkill -f -fi \"imagename eq " + strExe + "\" ");  //�رս���
	}
	else{
		QStringList arguments;
		//arguments << byteArray;
		m_pProcess->startDetached(strExe, arguments); //������������
		//m_pProcess->execute(strPath);//������������
	} 
}
//�򿪵�¼����
void charging::onOpenLoginDialog()
{
	m_loginDlg->show();
}


//��ʾ��ʾ�� 0��ʾ��1���桢2����
void charging::showTipsMessagebox(int iType , QString strMsg)
{
	if (m_msgDlg != nullptr 
		&& m_msgDlg->isActiveWindow() == false   //��ʾ��δ��ʾ
		&& m_bShowDebugInfo == false			//������Ϣ����δ��ʾ
		){
		m_msgDlg->setMessageText(strMsg);
		m_msgDlg->setTypeShow(iType);
	}
}

//������
void charging::OnBtnChargingOrStopCharging1()
{
	if (SERIAL_PORT->isOpen()/*my_Serial.isOpen()*/ == false){

		printfDebugInfo("����δ��", enDebugInfoPriority::DebugInfoLevelOne, true);

		showTipsMessagebox(1, "����δ��!");
		return;
	}
	QPushButton* btn = qobject_cast<QPushButton*>(sender()); 
	ui_charg_grid *groupBox = (ui_charg_grid *)btn->parent(); 
	qDebug() << groupBox->title() + " clicked: " + (groupBox->getCharging() ? "Stop" : "Charging");
	QString strId = groupBox->title(); 

	 
	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger;
	if (getBatteryIdRelatedInfo(strId, itCloset, itBattery, itBatteryModel, itCharger))
	{		
		bool isCharging = itCharger->second.isCharging;
		if (!isCharging)
		{
			// �жϳ����������ȡ��Ӧ�ĵ�ؽṹ������ѹ������
			int iResult = 0;
			if (chargingByLocalID(strId, &iResult))
			{
				int indexArray = batteryIDtoArrayIndex(strId);
				battery_state_enable_refresh[indexArray] = false;
				itBattery->second.timeLockUI.restart(); 
				charger_state[indexArray] = STATE_CHARGING;//"�����";
				emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
				printfDebugInfo(" "+strId + "�ֶ����", enDebugInfoPriority::DebugInfoLevelOne);
				COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "�ֶ����\n");
			}
			else
			{
				//Ԥ�� 
			}
		}
		else {
			//2 �ж�ֹͣ����
			if (stopByLocalID(strId))
			{
				int indexArray = batteryIDtoArrayIndex(strId);
				battery_state_enable_refresh[indexArray] = false;
				itBattery->second.timeLockUI.restart();
				charger_state[indexArray] = STATE_FREE;//"���������"; 
				emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
				printfDebugInfo(strId + "�ֶ�ֹͣ", enDebugInfoPriority::DebugInfoLevelOne);
				COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "�ֶ�ֹͣ\n");
				//ȡ��Ԥ��
				itBattery->second.isApplyCharging = false;
				removeChargingQueue(strId);
			}
		}
	}  
	
}
//���յ��ŵ�
void charging::OnBtnDisChargingOrStop1()
{
	if (SERIAL_PORT->isOpen()/*my_Serial.isOpen()*/ == false){
		printfDebugInfo("����δ��", enDebugInfoPriority::DebugInfoLevelOne, true);
		return;
	}
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	ui_charg_grid *groupBox = (ui_charg_grid *)btn->parent();	 
	QString strId = groupBox->title();
	int iResult = 0;
	if (detectDisChargingCondition(strId, &iResult))
	{
		MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger;
		if (getBatteryIdRelatedInfo(strId, itCloset, itBattery, itBatteryModel, itCharger))
		{
			if (!(itCharger->second.isDisCharging))
			{
				QString strConnectType = itBatteryModel->second.connectType;//��ؽṹ
				QString strStorageVol; strStorageVol.sprintf("%5.2f", itBatteryModel->second.storageVoltage);//�洢��ѹ
				toSend("Q," + QString::number(itCharger->second.id) + "," + strConnectType.left(2) + "," + strStorageVol, stCommand::hight);
				itCharger->second.isDisCharging = true;
				int indexArray = batteryIDtoArrayIndex(strId);
				battery_state_enable_refresh[indexArray] = false;
				charger_state[indexArray] = STATE_DISCHARGING;//"�ŵ���";
				emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
				printfDebugInfo(strId + "�ֶ��ŵ�", enDebugInfoPriority::DebugInfoLevelOne);
				COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "�ֶ��ŵ�\n");
			}
			else
			{
				toSend("P," + QString::number(itCharger->second.id), stCommand::hight);
				itCharger->second.isDisCharging = false;
				int indexArray = batteryIDtoArrayIndex(strId);
				battery_state_enable_refresh[indexArray] = false;
				charger_state[indexArray] = STATE_FREE;//"���������";
				emit RefreshState(enRefreshType::ChargerOnlineState, indexArray);
				printfDebugInfo(strId + "�ֶ�ֹͣ", enDebugInfoPriority::DebugInfoLevelOne);
				COperatorFile::GetInstance()->writeLog((QDateTime::currentDateTime()).toString("hh:mm:ss ") + strId + "�ֶ�ֹͣ�ŵ�\n");
			}
		}
	}
}


//ˢ��UI
void charging::onRefreshUI(QString strID)
{
	// ���� strID ˢ�¶�Ӧ�ĵ��UI��Ϣ

	show_all_data_now();
}


// ����textEdit �������ù�굽ĩβ
void charging::updateTextEdit(QString strContent)
{ 
	printfDebugInfo(strContent, enDebugInfoPriority::DebugInfoLevelThree); 
}

//ˢ�½�������������ˢ��
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
	else if (m_mapCloset.size() == 1 && count == 5)  //һ������ 5����ѯ�ͷ�һ��
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
	QAction * action = new QAction("��ͣ����ͨ��", m_menuSys);
	action->setCheckable(true);	
	connect(action, SIGNAL(triggered(bool )), this, SLOT(onPauseScand(bool)));
	action->setChecked(!m_bContinueScan);
	m_CommandQueue.setPause(m_bContinueScan);
	m_menuSys->addAction(action);

	QAction * action9 = new QAction("��ͣ���紫��", m_menuSys);
	action9->setCheckable(true);
	action9->setChecked(!m_bContinueSubmit);
	connect(action9, SIGNAL(triggered(bool)), this, SLOT(onPauseSubmit(bool)));
	m_menuSys->addAction(action9);
	
	QAction * action8 = new QAction("��ʾϵͳ����", m_menuSys);
	m_menuSys->addAction(action8);
	connect(action8, SIGNAL(triggered()), this, SLOT(onOpenOptionWin()));

	QAction * action10 = new QAction("��ʾ������Ϣ", m_menuSys);
	m_menuSys->addAction(action10);
	connect(action10, SIGNAL(triggered()), this, SLOT(OnBtnShowDebugInfo()));
	

	QAction * action12 = new QAction("CAN�豸", m_menuSys);
	m_menuSys->addAction(action12);
	action12->setCheckable(true);
	connect(action12, SIGNAL(triggered(bool)), this, SLOT(onOpenCanDevice(bool)));

	//ɨ�贮�ڣ���ɨ��������˵������Զ��򿪴��ں�����һ������
	QAction * action11 = new QAction("�����豸", m_menuSys);
	m_menuSys->addAction(action11);

	QAction * action4 = new QAction("��ʾ��Ϣ�ȼ�", m_menuSys);
	m_menuSys->addAction(action4);
	m_menuDebugInfoLevel = new QMenu(m_menuSys);	
	QAction * action5 = new QAction("һ��", m_menuDebugInfoLevel); action5->setCheckable(true);
	m_menuDebugInfoLevel->addAction(action5);
	QAction * action6 = new QAction("����", m_menuDebugInfoLevel); action6->setCheckable(true);
	m_menuDebugInfoLevel->addAction(action6);
	QAction * action7 = new QAction("����", m_menuDebugInfoLevel); action7->setCheckable(true);
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
	if (str == "һ��")
		m_enShowDebugInfoLevel = DebugInfoLevelOne;
	if (str == "����")
		m_enShowDebugInfoLevel = DebugInfoLevelTwo;
	if (str == "����")
		m_enShowDebugInfoLevel = DebugInfoLevelThree;
	QList<QAction*> listAction = m_menuDebugInfoLevel->actions();
	for (int i = 0; i < listAction.size(); i++){
		if (listAction[i] != action)
			listAction[i]->setChecked(false);
	}
}



//���е��
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
//��ǰ�������е�س��
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


//�������UI��
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
		p_charge_grid->setBalence(i < 41);
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

	 
	// ����ˮƽ���
	m_gridLayout->setHorizontalSpacing(1);
	// ���ô�ֱ���
	m_gridLayout->setVerticalSpacing(1);

	m_groupBox->setLayout(m_gridLayout);

	//��ʱ����Ҫ��ʾ�����ܵ��
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

	//	//���Ʒ�����״̬
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
}

//����ui��������Ϣ����
void charging::createDebugInfoUI()
{
	m_GroupBox_DebugInfo = new QGroupBox("", this);
	tablePage_DebugInfo = new QTabWidget(m_GroupBox_DebugInfo);
	tab_1 = new QWidget();	
	m_TextEdit_DebugInfo = new QTextEdit(tab_1);   //������Ϣ�ı���
	tablePage_DebugInfo->addTab(tab_1, QString("������Ϣ"));
	m_CheckBox_DebugInfo = new QCheckBox("�Զ�����", m_GroupBox_DebugInfo);   //������Ϣ�Զ�����
	m_BtnMin_DebugInfo = new QPushButton(m_GroupBox_DebugInfo);   //������Ϣ���ذ�ť
	m_Label_DebugInfo = new QLabel("��Ϣ��������", m_GroupBox_DebugInfo);
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
			str.sprintf("��أ�%d���¶ȣ�00.0�㣬��ѹ��0.00v��������0.00a", (itBattery->first));
			strList << str;
		}
	}
	if (m_listview_Battery_model == nullptr){
		m_listview_Battery_model = new QStringListModel(strList);
	}
	m_listview_Battery->setModel(m_listview_Battery_model); 
	tablePage_DebugInfo->addTab(tab_2, QString("�����Ϣ"));
}

//����ui�� ����λ�á����ؿؼ�������ͼƬ��
void charging::adjustUI()
{ 
	//����������С ������������
	QString str = "QMainWindow{background-image: url(" + g_AppPath + "/img/mainwin_background.png);}";
	this->setStyleSheet(str);
	this->setFixedSize(800, 600); 
	this->setWindowTitle(g_winTitle);
	ui.lab_logo->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/logo.png);}");
	ui.lab_logo->setText("");  
	ui.lab_title->setText(g_winTitle);
	ui.lab_title->setStyleSheet("QLabel{font-size:24px;font-weight:bold;color:white}");

	//���������� �������ޱ߿�������������ť�Ҽ��˵���������С���뻹ԭ��
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setWindowIcon(QIcon(g_AppPath + "/ICon/MainWindow.ico"));

	//���� ��������� �ߴ�
	//m_groupBox->setGeometry(QRect(1, 60, 798, 484));
	m_groupBox->setGeometry(QRect(1, 60, 798, 557));
	m_groupBox->setStyleSheet("QGroupBox{border: 0px }");
	

	//���ò����˵�
	m_menuSys->setStyleSheet(
		"QMenu {background-color: white; border: 1px solid white; }\
		 QMenu::item {background-color: transparent;padding:8px 20px;margin:0px 8px;border-bottom:1px solid #DBDBDB;}");
	m_menuCom->setStyleSheet(
		"QMenu {background-color: white; border: 1px solid white; }\
			QMenu::item {background-color: transparent;padding:8px 20px;margin:0px 8px;border-bottom:1px solid #DBDBDB;}");
	//																																																																			QMenu::item:selected{background - color: #2dabf9;}");
	/* sets background of the menu ���������˵�����ı���ɫ�����õ��ǰ�ɫ��white*/
	//   /*�����˵�����ı߿��ϸ����ʽ����ɫ*/
	//"   " +  /* sets background of menu item. set this to something non-transparent if you want menu color and menu item color to be different */
	//" "/*���ò˵����������º����ҵ��ڱ߾࣬Ч�����ǲ˵��е���Ŀ�����������˼��*/
	//"" +/*���ò˵������߾�*/
	//    ""+/*Ϊ�˵���֮����Ӻ��߼��*/
	//""+ /* when user selects item using mouse or keyboard */
	//""+/*��һ�������ò˵�����꾭��ѡ�е���ʽ*/
	//"" 		

	//��һҳ����һҳ��ͼƬ
	ui.btnPrePage->setText("");
	ui.btnPrePage->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btnPrePage.png);}" //
		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnPrePage_hover.png);}"
		"QPushButton:pressed{border-image: url(" + g_AppPath + "/img/btnPrePage.png);}"); //

	ui.btnNextPage->setText("");
	ui.btnNextPage->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btnNextPage.png);}" //
		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnNextPage_hover.png);}"
		"QPushButton:pressed{border-image: url(" + g_AppPath + "/img/btnNextPage.png);}"); //
	 

	
	//���ò�����ťλ�á���С������ͼ
	ui.btnMenuSys->setGeometry(QRect(530, 3, 94, 62));
	ui.btnMenuSys->setText("");
	ui.btnMenuSys->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btn_operator.png);}" //
			"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btn_operator_hover.png);}"
			"QPushButton:pressed{border-image: url(" + g_AppPath + "/img/btn_operator.png);}"); //
	
	//���ò�����ťλ�á���С������ͼ
	//ui.btnMenuCom->setGeometry(QRect(605, 3, 94, 62));
	//ui.btnMenuCom->setText("");
	//ui.btnMenuCom->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btn_serial_port.png);}"
	//		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btn_serial_port_hover.png);}"
	//		"QPushButton:pressed{border-image: url(" + g_AppPath + "/img/btn_serial_port.png);}"); //

	//���ò�����ťλ�á���С������ͼ
	ui.btnSysClose->setGeometry(QRect(650, 3, 94, 62));
	ui.btnSysClose->setText("");
	ui.btnSysClose->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btn_close.png);}"
			"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btn_close_hover.png);}"
			"QPushButton:pressed{border-image: url(" + g_AppPath + "/img/btn_close.png);}"); //

	//������С����ť����
	str = "QPushButton{border-image: url(" + g_AppPath + "/img/btn_min_normal.png);}"
		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btn_min_hover.png);}"
		"QPushButton:pressed{border-image: url(" + g_AppPath + "/img/btn_min_normal.png);}";
	ui.btnSysMin->setStyleSheet(str);
	ui.btnSysMin->setVisible(false);


	//�¶ȿؼ�
	//ui.lab_temperature->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/test001.png);}");
	ui.lab_temperature->setStyleSheet("QLabel{ border: 4px solid rgb(27,118,189);border-radius:4px; border-top-width:25px; background-color: rgb(10,38,60,255); }");
	ui.lab_temperature_title->setStyleSheet("QLabel{font-size:16px;font-weight:bold;color:white}");
	ui.lab_temperature_number->setStyleSheet("QLabel{font-size:24px;font-weight:bold;color:white}");
	ui.lab_temperature_imge->setText("");
	ui.lab_temperature_number->setText("0.0��");
	ui.lab_temperature_imge->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/temperature_disenable.png);}");


	//���õ�����Ϣ����
	m_GroupBox_DebugInfo->setGeometry(QRect(800, 70, 350, 530));
	m_GroupBox_DebugInfo->setStyleSheet("QGroupBox {   background-color: white } ");
	m_CheckBox_DebugInfo->setGeometry(QRect(200, 10, 100, 20));
	m_CheckBox_DebugInfo->setStyleSheet("QCheckBox { font-size:16px;font-weight:bold;color:black } ");
	m_CheckBox_DebugInfo->setCheckState(m_bAutoHideDebugInfo ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
	m_Label_DebugInfo->setGeometry(QRect(10, 10, 150, 20));
	m_Label_DebugInfo->setStyleSheet("QLabel{font-size:16px;font-weight:bold;color:black} ");
	m_BtnMin_DebugInfo->setGeometry(QRect(309, 1, 40, 40));
	str = "QPushButton{border-image: url(" + g_AppPath + "/img/btnDebugInfoMinNomarl.png);}"
		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnDebugInfoMinHover.png);}"
		"QPushButton:pressed{border-image: url(" + g_AppPath + "/img/btnDebugInfoMinPressed.png);}";
	m_BtnMin_DebugInfo->setStyleSheet(str);

	tablePage_DebugInfo->setGeometry(QRect(0, 40, 350, 490));
	m_TextEdit_DebugInfo->setGeometry(QRect(0, 0, 348, 466));
	m_listview_Battery->setGeometry(QRect(0, 0, 348, 466));


	//���ó������� ��������ʽ����background-color: white; border: 1px solid white;
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
	//	//����ȫ����ʾ
	//	setWindowState(Qt::WindowFullScreen);
	//	//�����ޱ߿����ö�
	//	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	//}
	//else{
	//	//�̶���С������ʾ
	//	//this->setFixedSize(this->width(), this->height()); 
	//}
	//����ҳ���������������һҳ������ʾ��һҳ��һҳ 
	//ui.btnPrePage->setVisible(m_mapCloset.size() > ONE_PAGE_CLOSET);
	//ui.btnNextPage->setVisible(m_mapCloset.size() > ONE_PAGE_CLOSET);
}

void charging::OnBtnShowSysMenu()
{
	QRect rect = ui.btnMenuSys->geometry(); 
	m_menuSys->move(mapToGlobal(QPoint(rect.left() + 10, rect.bottom())));
	//menu->move(cursor().pos()); //�ò˵���ʾ��λ��������������
	m_menuSys->show();
}

//��ʾ
void charging::OnBtnShowComMenu()
{
	QRect rect = ui.btnMenuCom->geometry();
	m_menuCom->move(mapToGlobal(QPoint(rect.left() + 10, rect.bottom())));
	//menu->move(cursor().pos()); //�ò˵���ʾ��λ��������������
	m_menuCom->show();
}

//��С��
void charging::OnBtnSysMin()
{
	setWindowState(Qt::WindowMinimized);
}

//�ر�
void charging::OnBtnSysClose()
{
	QMessageBox::StandardButton button;
	button = (QMessageBox::StandardButton)QMessageBox::question(this, "��ʾ",
		"�����������У��Ƿ��˳�?",
		"��", "��");
	if (button == 0) { //����
		if (meTimer)
			meTimer->stop();
		m_CommandQueue.stop();
		m_CommandQueue.clearCommand();
		while (!m_CommandQueue.isFinished()){
			Sleep(10); 
		}
		//my_Serial.serialClose();
		SERIAL_PORT->ClosePort();
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
	m_bAutoHideDebugInfo = (nState == 2);//��nState = 2, ����0
	(CReadIniFile::getInstance()->writeProfileInfo("WidgetShow", "AutoHideDebugInfo", QString::number(m_bAutoHideDebugInfo), g_AppPath + "\\set.ini", &iError));
}
void charging::printfDebugInfo(QString strInfo, enDebugInfoPriority enLevel, bool bErrorInfo )
{
	if (m_TextEdit_DebugInfo == nullptr)
		return;
	QTextCursor cursor = m_TextEdit_DebugInfo->textCursor();
	cursor.movePosition(QTextCursor::End);
	m_TextEdit_DebugInfo->setTextCursor(cursor);  //�ƶ���굽ĩβ

	if (enLevel <= m_enShowDebugInfoLevel){
		QString strTemp;
		if (bErrorInfo)		//������Ϣ
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
	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger;
	if (getBatteryIdRelatedInfo(strLocalId, itCloset, itBattery, itBatteryModel, itCharger))
	{
		QString strTitle;
		//�Ƚ��Ƿ��Ӧ��ǰ����;��ֹ�����Ĺ��ӵ�س��ˢ�µ�����
		QString strClosetUiNo = m_iCurrentCloset < 10 ? "0" + QString::number(m_iCurrentCloset) : QString::number(m_iCurrentCloset);
		if (strClosetUiNo == strLocalId.left(2) )
		{
			//strTitle = m_vtUiChargGrid[index2]->title();
			if (index < MAX_BATTERY)
				uiIndex = index;
			else if (index >= MAX_BATTERY)
				uiIndex = index % MAX_BATTERY;

			if (type == enRefreshType::ChargerState && battery_state_enable_refresh[index])
			{
				if (itCharger->second.isOverHeat )
					m_vtUiChargGrid[uiIndex]->setChargerState(STATE_OVER_HEAT);
				else
					m_vtUiChargGrid[uiIndex]->setChargerState(charger_state[index]);
				updateListviewBatteryModel(index);
			}
			else if (type == enRefreshType::BatteryState  && battery_state_enable_refresh[index]){
				m_vtUiChargGrid[uiIndex]->setBatteryState(battery_state[index], battery_voltage[index]);

			}
			else if (type == enRefreshType::ChargerOnlineState)
			{ 
				if (itCharger->second.isOverHeat)
					m_vtUiChargGrid[uiIndex]->setChargerState(STATE_OVER_HEAT);
				else
					m_vtUiChargGrid[uiIndex]->setChargerState(charger_state[index]);
				updateListviewBatteryModel(index);
			}
			else if (enRefreshType::BatteryVol == type){
				m_vtUiChargGrid[uiIndex]->setVol(battery_voltage[index].toFloat());
				updateListviewBatteryModel(index);
			}
		}
	}
}
//�����������RadioUI
void charging::createClosetRadio()
{ 
	m_vtUiClosetRadio.clear();
	for (MAP_CLOSET_IT itCloset = m_mapCloset.begin(); itCloset != m_mapCloset.end(); itCloset++){
		QPushButton* qRadioBtn = new QPushButton(ui.tab);
		qRadioBtn->setText("����"+ QString::fromLocal8Bit(itCloset->second.id));
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
//ˢ�µ�ǰ�ĵ����Ϣ
void charging::refreshCurrentUI()
{ 
	QString strClosetUiNo = m_iCurrentCloset < 10 ? "0" + QString::number(m_iCurrentCloset) : QString::number(m_iCurrentCloset);
		
	QString strClosetNo;
	QVector<QString> vtTemp;	
	for (MAP_BATTERY_IT it = m_mapBattery.begin(); m_mapBattery.end() != it; it++){
		for (int i = 0; i < MAX_BATTERY; i++){
			strClosetNo = it->second.id;
			if (i + 1 == strClosetNo.right(2).toInt() 
				&& strClosetNo.left(2) == strClosetUiNo)
			{ //��غ�
				vtTemp.append(strClosetNo);
				break;
			}
		} 
	}
	for (int i = 0; i < MAX_BATTERY; i++)
	{		
		if (m_vtUiChargGrid.size() > 0)
		{
			m_vtUiChargGrid[i]->setTitle(vtTemp[i]);  	  //���ñ��� 
			MAP_BATTERY_IT itBattery = m_mapBattery.find(vtTemp[i].toInt());
			if (itBattery != m_mapBattery.end()){
				MAP_BATTERY_MODEL_IT itBatteryModel = m_mapBatteryModel.find(itBattery->second.modelId);
				if (itBatteryModel != m_mapBatteryModel.end()){//���õ���ͺ�
					m_vtUiChargGrid[i]->setBatteryModel(QString::fromLocal8Bit(itBatteryModel->second.droneModel));
				
				}
			} 
		} 
	}
	
}
//�������
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
//����ƶ� 
void charging::mouseMoveEvent(QMouseEvent *event)
{
	if (m_LButtonPress && m_bCanMoveWindow)
	{
		QPoint movePos = event->globalPos();
		this->move(movePos - m_MousePrePoint);  //ʵ�ִ����ƶ�
	}
}
//����ɿ�
void charging::mouseReleaseEvent(QMouseEvent * event)
{
	if (m_LButtonPress)
	{
		m_LButtonPress = false;
		this->setCursor(Qt::ArrowCursor);
	}
}

//���õ�����Ϣ�����λ��
void charging::OnAdjustDebugInfoRect(QRect rect)
{
	m_GroupBox_DebugInfo->setGeometry(rect);
}

void charging::updateListviewBatteryModel(int indexMem )
{
	QString str; QModelIndex modelIndex;
	if (indexMem == -1){
		//���µ�ǰ�������Ե������
		m_listview_Battery_model->removeRows(0, m_listview_Battery_model->rowCount()); //ɾ������
		MAP_CLOSET_IT itCloset = m_mapCloset.find(m_iCurrentCloset);		
		int indexArray = 0;  int rowCount = 0; 
		for (MAP_BATTERY_IT itBattery = itCloset->second.mapBattery.begin();
			itBattery != itCloset->second.mapBattery.end();	itBattery++){
			m_listview_Battery_model->insertRows(rowCount, 1); //����
			modelIndex = m_listview_Battery_model->index(rowCount++);
			indexArray = batteryIDtoArrayIndex(QString::fromLocal8Bit(itBattery->second.id));
			str.sprintf("��أ�%s���¶ȣ�%03.1f�㣬��ѹ��%0.2fv��������%0.2fa"\
				,(itBattery->second.id)
				,(battery_temperature[indexArray].toFloat())
				,(battery_voltage[indexArray].toFloat())
				,(battery_current[indexArray].toFloat()));

			m_listview_Battery_model->setData(modelIndex, str);
			//m_listview_Battery->setCurrentIndex(index);
		}
	}
	else
	{
		//����index��Ӧ������
		QString strBatteryId = battery_local_id[indexMem];
		MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger;
		if (getBatteryIdRelatedInfo(strBatteryId, itCloset, itBattery, itBatteryModel, itCharger))
		{
			int uiInsex = indexMem % MAX_BATTERY;
			QModelIndex modelIndex = m_listview_Battery_model->index(uiInsex);
			QVariant variant = m_listview_Battery_model->data(modelIndex, Qt::DisplayRole);  //��ȡ��ǰѡ�������ı�  
			str.sprintf("��أ�%s���¶ȣ�%03.1f�㣬��ѹ��%0.2fv��������%0.2fa"\
				, (itBattery->second.id)
				, (itCharger->second.getAverage(itCharger->second.fTemperature))
				, (itCharger->second.getAverage(itCharger->second.fVoltage))
				, (itCharger->second.fCurrent));
			m_listview_Battery_model->setData(modelIndex, str);
		} 
	}
}