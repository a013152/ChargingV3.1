#include "chargingoption.h"
#include "BatteryModelPage.h"
#include "BatteryPage.h"
#include <QPushButton>
#include <QScrollBar>
#include "MyQComboBox.h"

QString g_AppPath;
QString g_winTitle;


chargingOption::chargingOption(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QString iConPath = g_AppPath; iConPath += "\\Icon\\Option.ico";	
	
	this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
	setWindowIcon(QIcon(iConPath));

	setWindowTitle(SETING_WINDOW_TITLE);


	COperatorFile::GetInstance()->setAppPath(g_AppPath);
	COperatorFile::GetInstance()->readAllConfig(m_mapCloset, m_mapBattery, m_mapBatteryModel, m_mapCharger, &m_iError);

	//读取提交服务器时间间隔
	m_SubmitInterval = CReadIniFile::getInstance()->readProfileInfo("SET", "SubmitInterval", g_AppPath + "\\set.ini", &iError).toInt();
	if (m_SubmitInterval == 0){
		m_SubmitInterval = ONE_SUBMIT_TIME;
	}

	//读取充电时限
	m_ChargeLimitTime = CReadIniFile::getInstance()->readProfileInfo("SET", "ChargeLimitTime", g_AppPath + "\\set.ini", &iError).toInt();
	if (m_ChargeLimitTime == 0){
		m_ChargeLimitTime = CHARGING_LIMIT_TIME;
	}

	//过热温度
	m_fOverHeatTemperature = CReadIniFile::getInstance()->readProfileInfo("SET", "OverHeatTemperature", g_AppPath + "\\set.ini", &iError).toInt();
	if (m_fOverHeatTemperature == 0){
		m_fOverHeatTemperature = DEFAUT_OVERHEATTEMPERATURE;
	}

	QRect rect = this->geometry();
	 
	this->setFixedSize(this->width(), this->height());

	isChanged = false;
	isChanged2 = false;
	iError = 0;
	//createTabPage();
	
	initWidget();

	initConnectWidget();
}

chargingOption::~chargingOption()
{

}

void chargingOption::initWidget()
{ 
	ui.tablePage->setCurrentIndex(0);
	ui.tablePage->removeTab(2);  //暂时屏蔽 “充电记录”功能 delete 20180723
	 
	ui.btnAdd_1->setVisible(false);
	ui.btnDel_1->setVisible(false);
	QRect rect = ui.btnSave_1->geometry();
	rect.setBottom(rect.bottom() - 120);
	rect.setTop(rect.top() - 120);
	ui.btnSave_1->setGeometry(rect);

	//初始化下拉框
	initComboBoxWidget();

	//设置表格的内容  
	initTabWidget(m_mapBattery, m_mapBatteryModel); 

	// 初始化Combo
	ui.cmbCloset->addItem("全部");
	ui.cmbCloset->setCurrentText("全部");
	for (MAP_CLOSET_IT it3 = m_mapCloset.begin(); it3 != m_mapCloset.end(); it3++){
		ui.cmbCloset->addItem(it3->second.id);
	}

	QRegExp regx("[0-9]+$");
	QValidator *validator = new QRegExpValidator(regx, ui.lineEdit);  //只能输入数字
	ui.lineEdit->setValidator(validator);
	ui.lineEdit_2->setValidator(validator);

	ui.lineEdit->setText(QString::number(m_ChargeLimitTime));
	ui.lineEdit_2->setText(QString::number(m_SubmitInterval));
	ui.lineEdit_3->setText(QString("%1").arg(m_fOverHeatTemperature));
}


void chargingOption::initComboBoxWidget()
{
	
	if (m_ComboBoxBatteryModel == NULL)
	{
		m_ComboBoxBatteryModel = new MyQComboBox(ui.tableBattery);
		m_ComboBoxBatteryModel->installEventFilter(this);
	}
	m_ComboBoxBatteryModel->setVisible(false);

	if (m_ComboBoxDroneModel == NULL)
		m_ComboBoxDroneModel = new MyQComboBox(ui.tableBattery);
	m_ComboBoxDroneModel->setVisible(false);

	if (m_ComboBoxBalance == NULL)
		m_ComboBoxBalance = new MyQComboBox(ui.tableBatteryModel);
	m_ComboBoxBalance->setVisible(false);
}

//初始化Tab控件
void chargingOption::initTabWidget(MAP_BATTERY &mapBattery, MAP_BATTERY_MODEL& mapBatteryModel)
{
	ui.tableBattery->blockSignals(true);
	ui.tableBatteryModel->blockSignals(true);
	m_pCurrentItemBattery = NULL;
	m_pCurrentItemDrone = NULL;
	m_pCurrentItemBalance = NULL;
	m_ComboBoxDroneModel->clear();
	m_ComboBoxBatteryModel->clear();
	m_ComboBoxBalance->clear();
	ui.tableBattery->clear();


	ui.tableBattery->setColumnCount(4);
	//ui.tableBattery->setSelectionMode(QAbstractItemView::SingleSelection);  //单选 
	ui.tableBattery->setSelectionBehavior(QAbstractItemView::SelectRows);//设置行选择。

	ui.tableBattery->setSelectionMode(QAbstractItemView::ContiguousSelection); //设置多行选择。
	
	//ui.tableBattery->setColumnWidth(0, 120);

	QStringList headerLabels;
	headerLabels << "电池编号" << "电池型号" << "无人机型号" << "数据库对应编号";
	ui.tableBattery->setHorizontalHeaderLabels(headerLabels);
	//设置编辑方式  
	ui.tableBattery->setEditTriggers(QAbstractItemView::DoubleClicked);
	////自动调整最后一列的宽度使它和表格的右边界对齐  
	ui.tableBattery->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);;

	ui.tableBatteryModel->setColumnCount(9);
	ui.tableBatteryModel->setSelectionMode(QAbstractItemView::SingleSelection);  //单选 
	headerLabels.clear();
	headerLabels << "型号编号"  << "无人机型号" << "容量" << "充电电压" << "充电电流" \
		<< "满电电压" << "储存电压" << "电池结构" << "是否平衡";
	ui.tableBatteryModel->setHorizontalHeaderLabels(headerLabels);

	int iRow = mapBattery.size();
	ui.tableBattery->setRowCount(iRow);
	auto pNewLabe = [](QWidget* parent, QString str) -> QLabel*	{
		QLabel *lab = new QLabel(str);
		return lab;
	};
	auto pNewTableWidgetItem = [](QString str) -> QTableWidgetItem*	{
		QTableWidgetItem *item0 = new QTableWidgetItem(str);
		return item0;
	};
	auto pSetDisEnable = [](QTableWidgetItem* pItemTemp) {
		pItemTemp->setFlags(pItemTemp->flags() & (~Qt::ItemIsEditable)); //不可编辑
		pItemTemp->setFlags(pItemTemp->flags() & (~Qt::ItemIsSelectable)); //不可选中
	};
	auto pNewQComboBox1 = [&](QWidget* parent, int index) -> QComboBox*	{
		QComboBox *ComboBox = new QComboBox();
		for (MAP_BATTERY_MODEL_IT it2 = m_mapBatteryModel.begin(); it2 != m_mapBatteryModel.end(); it2++)
		{

			ComboBox->addItem(QString::number(it2->first));
			if (index == it2->first)
				ComboBox->setCurrentText(QString::number(it2->first));
		}
		return ComboBox;

	};
	auto pNewQComboBox2 = [&](QWidget* parent, QString strDroneModel) -> MyQComboBox*	{
		MyQComboBox *ComboBox = new MyQComboBox(parent);
		for (MAP_BATTERY_MODEL_IT it2 = m_mapBatteryModel.begin(); it2 != m_mapBatteryModel.end(); it2++)
		{

			ComboBox->addItem(it2->second.droneModel);
			if (strDroneModel.compare(it2->second.droneModel) == 0)
				ComboBox->setCurrentText(strDroneModel);
		}
		return ComboBox;

	};
	//电池信息表
	char szTemp[256] = { 0 }; iRow = 0;
	for (MAP_BATTERY_IT it = mapBattery.begin(); it != mapBattery.end(); it++, iRow++)
	{ 
		ui.tableBattery->setItem(iRow, 0, pNewTableWidgetItem(it->second.id));
		pSetDisEnable(ui.tableBattery->item(iRow, 0));   //不可编辑，不可选中

		//电池型号
		ui.tableBattery->setItem(iRow, 1, pNewTableWidgetItem(itoa(it->second.modelId, szTemp, 10)));

		pSetDisEnable(ui.tableBattery->item(iRow, 1));   //不可编辑，不可选中

		//无人机型号
		MAP_BATTERY_MODEL_IT it2 = m_mapBatteryModel.find(it->second.modelId);
		if (it2 != m_mapBatteryModel.end())
		{
			ui.tableBattery->setItem(iRow, 2, pNewTableWidgetItem(it2->second.droneModel));
			pSetDisEnable(ui.tableBattery->item(iRow, 2));   //不可编辑，不可选中

		}
		//数据库编号
		ui.tableBattery->setItem(iRow, 3, pNewTableWidgetItem(QString::number(it->second.dbid)));
		pSetDisEnable(ui.tableBattery->item(iRow, 3));   //不可编辑，不可选中
		//购买时间
		//ui.tableBattery->setItem(iRow, 4, pNewTableWidgetItem(it->second.purchasingTime));
		  
	}
	
	//电池型号信息表
	//设置编辑方式  
	ui.tableBatteryModel->setEditTriggers(QAbstractItemView::DoubleClicked);

	//自动调整最后一列的宽度使它和表格的右边界对齐  
	ui.tableBatteryModel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	//设置电池型号表格的内容  
	iRow = mapBatteryModel.size();

	ui.tableBatteryModel->setRowCount(iRow);
	iRow = 0;
	for (MAP_BATTERY_MODEL_IT it = mapBatteryModel.begin(); it != mapBatteryModel.end(); it++, iRow++)
	{
		ui.tableBatteryModel->setItem(iRow, 0, pNewTableWidgetItem(itoa(it->second.id, szTemp, 10)));  //id
		pSetDisEnable(ui.tableBatteryModel->item(iRow, 0));   //不可编辑，不可选中
		m_ComboBoxBatteryModel->addItem(itoa(it->second.id, szTemp, 10));

		//ui.tableBatteryModel->setItem(iRow, 1, pNewTableWidgetItem(it->second.producer));   //制造商
		ui.tableBatteryModel->setItem(iRow, 1, pNewTableWidgetItem(it->second.droneModel));  //无人机型号
		m_ComboBoxDroneModel->addItem(it->second.droneModel);

		ui.tableBatteryModel->setItem(iRow, 2, pNewTableWidgetItem(itoa(it->second.capacity, szTemp, 10))); //容量
		sprintf(szTemp, "%5.2f", it->second.chargingVoltage);
		ui.tableBatteryModel->setItem(iRow, 3, pNewTableWidgetItem(szTemp));	//电压
		sprintf(szTemp, "%5.2f", it->second.chargingCurrent);
		ui.tableBatteryModel->setItem(iRow, 4, pNewTableWidgetItem(szTemp));  //电流
		sprintf(szTemp, "%5.2f", it->second.fullVoltage);
		ui.tableBatteryModel->setItem(iRow, 5, pNewTableWidgetItem(szTemp));  //满电电压
		sprintf(szTemp, "%5.2f", it->second.storageVoltage);
		ui.tableBatteryModel->setItem(iRow, 6, pNewTableWidgetItem(szTemp));  //储存电压
		sprintf(szTemp, "%s", it->second.connectType);
		ui.tableBatteryModel->setItem(iRow, 7, pNewTableWidgetItem(szTemp));  //电池结构
		sprintf(szTemp, "%s", it->second.balance == 1 ? "平衡" : "非平衡");
		ui.tableBatteryModel->setItem(iRow, 8, pNewTableWidgetItem(szTemp));  //是否平衡
	}
	

	m_ComboBoxBalance->addItem(QString("平衡"));
	m_ComboBoxBalance->addItem(QString("非平衡"));
	ui.tableBattery->blockSignals(false);
	ui.tableBatteryModel->blockSignals(false);
}

void chargingOption::initConnectWidget()
{
	QObject::connect(ui.btnSave_1, &QPushButton::clicked, this, &chargingOption::OnBtnSave); 
	QObject::connect(ui.btnSave_2, &QPushButton::clicked, this, &chargingOption::OnBtnSave2);
	QObject::connect(ui.btnSave_3, &QPushButton::clicked, this, &chargingOption::OnBtnSave3);
	QObject::connect(ui.btnQuery_1, &QPushButton::clicked, this, &chargingOption::OnBtnQuery_1);

	QObject::connect(ui.btnRefresh_1, &QPushButton::clicked, this, &chargingOption::OnBtnRefresh1);
	QObject::connect(ui.btnRefresh_2, &QPushButton::clicked, this, &chargingOption::OnBtnRefresh2);
	QObject::connect(ui.btnDel_1, &QPushButton::clicked, this, &chargingOption::OnBtnDel1);
	QObject::connect(ui.btnDel_2, &QPushButton::clicked, this, &chargingOption::OnBtnDel2);
	QObject::connect(ui.btnAdd_1, &QPushButton::clicked, this, &chargingOption::OnBtnAdd1);
	QObject::connect(ui.btnAdd_2, &QPushButton::clicked, this, &chargingOption::OnBtnAdd2); 
	 
	QObject::connect(ui.tableBattery, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(cellchangeBattery(int, int, int, int)));
	QObject::connect(ui.tableBatteryModel, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(cellchangeBatteryModel(int, int, int, int)));
	QObject::connect(ui.tableBattery, SIGNAL(itemDoubleClicked(QTableWidget *)), this, SLOT(cellChangedBattery(int, int)));
	QObject::connect(ui.tableBatteryModel, SIGNAL(cellChanged(int, int)), this, SLOT(cellChangedBatteryModel(int, int)));
	QObject::connect(m_ComboBoxBatteryModel, SIGNAL(currentTextChanged(const QString &)), this, SLOT(textChangeBattery(const QString &)));
	QObject::connect(m_ComboBoxDroneModel, SIGNAL(currentTextChanged(const QString &)), this, SLOT(textChangeDrone(const QString &)));
	QObject::connect(m_ComboBoxBalance, SIGNAL(currentTextChanged(const QString &)), this, SLOT(textChangeBalance(const QString &)));
	QObject::connect(ui.cmbCloset, SIGNAL(currentTextChanged(const QString &)), this, SLOT(textChangeCloset(const QString &)));
	QObject::connect(ui.lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(textChangeScanTime(const QString &)));
	QObject::connect(ui.lineEdit_2, SIGNAL(textChanged(const QString &)), this, SLOT(textChangeSubmitTime(const QString &)));

	QScrollBar* vScrollBar = ui.tableBattery->verticalScrollBar();
	QObject::connect(vScrollBar, SIGNAL(valueChanged(int)), this, SLOT(srocllBattery(int)));
	vScrollBar = ui.tableBatteryModel->verticalScrollBar();
	QObject::connect(vScrollBar, SIGNAL(valueChanged(int)), this, SLOT(srocllBatteryModel(int)));
	 
}

void chargingOption::resizeEvent(QResizeEvent* size)
{
	/*QRect rect = this->geometry();
	rect.setLeft( 10);	rect.setTop( 30);
	rect.setHeight(rect.height() - 100);	rect.setWidth(rect.width() - 200);
	mainLayout->setGeometry(rect);*/
}

//保存（电池信息）
void chargingOption::OnBtnSave()
{ 
	initComboBoxWidget();
	if (isChanged)
	{
		if (collectUIInfoBattery())
		{
			isChanged = false; //还原
			COperatorFile::GetInstance()->writeChargingInfo(m_mapBattery, &iError);
			iError = 0;
			if (iError)
			{
				QString str; str.sprintf("<font color='red'>保存失败，错误码：%d</font>", iError); 
				QMessageBox::warning(this, "提示", str, QMessageBox::Ok  );
			}
			else{
				QMessageBox::information(this, "提示", "保存完成");
				 
				//进程通信通知《智能充电保护箱》程序更新数据
				SendChargingProgramToReadConfig();
				  
				/*QMessageBox message(QMessageBox::NoIcon, "提示", "保存完成");
				message.setIconPixmap(QPixmap("D:\\1.png"));
				message.exec();*/
			}
		} 
	}
	else{
		QMessageBox::information(this, "提示", "未修改！");
	}
	
	//ui.tableBattery->setFocus();
}

//保存（电池型号信息)
void chargingOption::OnBtnSave2()
{
	if (isChanged2)
	{
		if (collectUIInfoBatteryModel()){
			isChanged2 = false; //还原 
			COperatorFile::GetInstance()->writeChargingModelInfo(m_mapBatteryModel, &iError);
			if (iError)
			{
				QString str; str.sprintf("<font color='red'>保存失败，错误码：%d</font>", iError);
				QMessageBox::warning(this, "提示", str, QMessageBox::Ok);
			}
			else{
				QMessageBox::information(this, "提示", "保存完成");
				//型号保存完成后 
				initTabWidget(m_mapBattery, m_mapBatteryModel);
				ui.btnRefresh_2->setText("刷新");
				//进程通信通知《智能充电保护箱》程序更新数据
				SendChargingProgramToReadConfig();
				/*QMessageBox message(QMessageBox::NoIcon, "Title", "Content with icon.");
				message.setIconPixmap(QPixmap("D:\\1.png"));
				message.exec();*/
			}
		} 
	} 
}

void chargingOption::OnBtnSave3()
{
	int iError = 0, iError2 = 0;
	QString str = ui.lineEdit->text();
	int temp = str.toInt();
	if (temp < 60){
		QMessageBox::information(this, "提示", "充电时间不建议小于60分钟。");
		ui.lineEdit->setText(QString::number(m_ChargeLimitTime));
		return;
	}
	else{
		m_ChargeLimitTime = temp;
	}
	str = ui.lineEdit_2->text();
	temp = str.toInt();
	if (temp < 10000){
		QMessageBox::information(this, "提示", "网络提交时间间隔不建议小于10000毫秒。");
		ui.lineEdit_2->setText(QString::number(m_SubmitInterval));

		return;
	}
	else{
		m_SubmitInterval = temp;
	}
	//温度
	str = ui.lineEdit_3->text();
	float ftemp = str.toFloat();
	if (ftemp >60 || ftemp < 0){
		QMessageBox::information(this, "提示", "过热温度简易设置在0~60之间。");
		ui.lineEdit_3->setText(QString("%1").arg(m_fOverHeatTemperature));

		return;
	}
	else{
		m_fOverHeatTemperature = ftemp;
	}

	CReadIniFile::getInstance()->writeProfileInfo("SET", "SubmitInterval",QString::number(m_SubmitInterval), g_AppPath + "\\set.ini", &iError);
	CReadIniFile::getInstance()->writeProfileInfo("SET", "ChargeLimitTime", QString::number(m_ChargeLimitTime), g_AppPath + "\\set.ini", &iError2);
	CReadIniFile::getInstance()->writeProfileInfo("SET", "OverHeatTemperature", QString("%1").arg(m_fOverHeatTemperature), g_AppPath + "\\set.ini", &iError2);
	if (iError2 == 0 && iError == 0){
		QMessageBox::information(this, "提示", "保存完成");
		//进程通信通知《智能充电保护箱》程序更新数据
		SendChargingProgramToReadConfig();
	}
}

void chargingOption::OnBtnQuery_1()
{
	m_OperatoreDB.onOpenDbFile();
	bool ff = m_OperatoreDB.onCreateChargedRecordTable();
	ff = false;
}

//新增加
void chargingOption::OnBtnAdd1()
{
	ui.btnRefresh_1->setText("撤销");
	int row = ui.tableBattery->rowCount();
	ui.tableBattery->setRowCount(row+1);
	ui.tableBattery->scrollToBottom();
	m_ComboBoxBatteryModel->setVisible(false);
	m_ComboBoxDroneModel->setVisible(false);
	QTableWidgetItem *item = ui.tableBattery->item(row-1, 0);
	QString str = item->text();
	str = str.left(4);
	int localId = str.toInt();
	int hundred = localId / 100;
	if (localId % 100 == MAX_CHARGER){  //
		localId = (hundred +1 ) *100 +1;
	}
	else{
		localId++;
	}
	str = QString::number(localId);
	if (str.length() == 3){
		str = "0" + str;
	}
	ui.tableBattery->setItem(row, 0, new QTableWidgetItem(str));
	item = ui.tableBattery->item(row-1, 1);
	ui.tableBattery->setItem(row , 1, new QTableWidgetItem(item->text()));
	item = ui.tableBattery->item(row-1, 2);
	ui.tableBattery->setItem(row , 2, new QTableWidgetItem(item->text()));
	item = ui.tableBattery->item(row-1, 3);
	ui.tableBattery->setItem(row , 3, new QTableWidgetItem(item->text()));
	item = ui.tableBattery->item(row-1, 4);
	ui.tableBattery->setItem(row , 4, new QTableWidgetItem(item->text()));
	isChanged = true;

}

void chargingOption::OnBtnAdd2()
{
	initComboBoxWidget();
	isChanged2 = true;
	ui.btnRefresh_2->setText("撤销");
	int row = ui.tableBatteryModel->rowCount();
	ui.tableBatteryModel->setRowCount(row + 1);
	ui.tableBatteryModel->scrollToBottom();
	m_ComboBoxBalance->setVisible(false);
	QTableWidgetItem *item = ui.tableBatteryModel->item(row - 1, 0);
	QString str = item->text();
	str = str.left(4);
	str = str.left(4);
	int localId = str.toInt();
	str = QString::number(++localId);
	ui.tableBatteryModel->setItem(row, 0, new QTableWidgetItem(str)); //编号
	item = ui.tableBatteryModel->item(row - 1, 2); 	 
	ui.tableBatteryModel->setItem(row, 2, new QTableWidgetItem(item->text()));// 电池容量
	item = ui.tableBatteryModel->item(row - 1, 3);
	ui.tableBatteryModel->setItem(row, 3, new QTableWidgetItem(item->text()));//充电电压
	item = ui.tableBatteryModel->item(row - 1, 4);
	ui.tableBatteryModel->setItem(row, 4, new QTableWidgetItem(item->text()));//充电电流
	item = ui.tableBatteryModel->item(row - 1, 5);
	ui.tableBatteryModel->setItem(row, 5, new QTableWidgetItem(item->text()));//满电电压
	item = ui.tableBatteryModel->item(row - 1, 6);
	ui.tableBatteryModel->setItem(row, 6, new QTableWidgetItem(item->text()));//
	item = ui.tableBatteryModel->item(row - 1, 7);
	ui.tableBatteryModel->setItem(row, 7, new QTableWidgetItem(item->text()));//电池结构 
}

void chargingOption::OnBtnRefresh1()
{	
	QString str = ui.cmbCloset->currentText();
	textChangeCloset(str);
	if (ui.btnRefresh_1->text() == "撤销")
	{
		ui.btnRefresh_1->setText("刷新");
		isChanged = false;
	}
}

void chargingOption::OnBtnRefresh2()
{
	initComboBoxWidget();
	initTabWidget(m_mapBattery, m_mapBatteryModel);
	if (ui.btnRefresh_2->text() == "撤销")
	{
		ui.btnRefresh_2->setText("刷新");
		isChanged2 = false;
	}
}

void chargingOption::OnBtnDel1()
{	
	ui.tableBattery->removeRow(ui.tableBattery->currentRow());
	ui.btnRefresh_1->setText("撤销");
	m_ComboBoxBatteryModel->setVisible(false);
	m_ComboBoxDroneModel->setVisible(false);
	isChanged = true;
}

void chargingOption::OnBtnDel2()
{
	initComboBoxWidget();
	ui.tableBatteryModel->removeRow(ui.tableBatteryModel->currentRow());
	ui.btnRefresh_2->setText("撤销");
	m_ComboBoxBalance->setVisible(false); 
	isChanged2 = true;
}

void chargingOption::SendChargingProgramToReadConfig()
{
	HWND hwnd = NULL;
	int iError = 0;
	g_winTitle = CReadIniFile::getInstance()->readProfileInfo("SET", "windowTitle", g_AppPath + "\\set.ini", &iError);
	if (iError != 0)
		g_winTitle = MAIN_WINDOW_TITLE;
	LPWSTR path = (LPWSTR)g_winTitle.utf16();    
	hwnd = ::FindWindowW(NULL, path);
	if (::IsWindow(hwnd))
	{		 
		stProcessSimpltData stObj ;
		strcpy(stObj.strInfo, "进程消息：参数已经修改");
		stObj.nTime1 = m_ChargeLimitTime;
		stObj.nSubmitTime = m_SubmitInterval;
		stObj.fOverHeatTemperature = m_fOverHeatTemperature;


		COPYDATASTRUCT copydata;
		copydata.dwData = CUSTOM_TYPE;  // 用户定义数据
		//copydata.lpData = data.data();  //数据大小
		//copydata.cbData = data.size();  // 指向数据的指针
		copydata.lpData = &stObj;
		copydata.cbData = sizeof(stProcessSimpltData);

		HWND sender = (HWND)effectiveWinId();

		::SendMessage(hwnd, WM_COPYDATA, reinterpret_cast<WPARAM>(sender), reinterpret_cast<LPARAM>(&copydata));
	}
	else{
		;
		//QMessageBox::warning(this, "提示", "未找到《" + g_winTitle + "》窗口。\n请查看进程是否关闭。", QMessageBox::Ok);
	}
}

//关闭软件 :检测一下是否需要保存
void chargingOption::closeEvent(QCloseEvent *event)
{
	if (isChanged&& isChanged2)
	{
		QMessageBox::StandardButton button;
		button = (QMessageBox::StandardButton)QMessageBox::question(this, "退出程序",
			"程序数据还未保存，是否保存?", 
			"保存", "不保存" );

		if (button == 0) { //保存
			
			//写入电池信息
			if (isChanged)
				COperatorFile::GetInstance()->writeChargingInfo(m_mapBattery, &iError); 

			//写入电池型号信息
			if (isChanged2)
				COperatorFile::GetInstance()->writeChargingModelInfo( m_mapBatteryModel, &iError);
			event->ignore();  
		}
		else if (button == 1) {
			event->accept();  //忽略 
		} 
	}else
		event->accept();   
}

//收集UI电池信息 保存到 m_mapBattery
bool chargingOption::collectUIInfoBattery()
{
	QTableWidgetItem *item = nullptr;  
	QString str, str2, str3 ,str4;
	QByteArray byteArray;
	int row = ui.tableBattery->rowCount(), colum=ui.tableBattery->columnCount();
	bool ret = true;
	do 
	{
		//UI循环检测空
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < colum; j++)
			{
				item = ui.tableBattery->item(i, j);
				 if ( item == NULL || item->text().isEmpty()){
				
					QString str = QString("<font color='red'>表格%1行%2列列不能留空</font>").arg(i).arg(j); 
					QMessageBox::warning(this, "提示", str, QMessageBox::Ok);				 
					return false; 
				}
			}  
		}
		//进入UI循环检测
		for (int i = 0; i < row; i++)
		{
			item = ui.tableBattery->item(i,0);
			str = item->text();
			item = ui.tableBattery->item(i, 1);
			str2 = item->text();
			item = ui.tableBattery->item(i, 2);
			str3 = item->text();
			item = ui.tableBattery->item(i, 3);
			str4 = item->text();

			//进入内存循环检测
			for (MAP_BATTERY_IT it = m_mapBattery.begin(); it != m_mapBattery.end(); it++)
			{	
				//电池编号相同
				if (str.compare(it->second.id) == 0)  
				{
					it->second.modelId = str2.toInt();  
					break;  // 跳出本次内存检测
				} 
				//else if (iMemMax <str.toInt())  //编号大于内存 新增到内存的map
				//{
				//	stBatteryInfo batteryInfo;				
				//	batteryInfo.dbid = 0;  //dbid 赋值有带商议， 需要根据数据的电池id赋值，
				//	strncpy(batteryInfo.id, (str.toLocal8Bit()).data(), 4);
				//	batteryInfo.modelId = ui.tableBattery->item(i, 1)->text().toInt();
				//	str = ui.tableBattery->item(i, 3)->text();
				//	strncpy(batteryInfo.user, str.toLocal8Bit().data(), str.toLocal8Bit().length());
				//	str = ui.tableBattery->item(i, 4)->text();
				//	//strncpy(batteryInfo.purchasingTime, str.toLatin1().data(), str.toLatin1().length());
				//	m_mapBattery.insert(MAP_BATTERY::value_type(atoi(batteryInfo.id), batteryInfo));
				//	break;  // 跳出本次内存检测
				//}
			}		 
		} 
	} while (0);
	
	return ret;
}

//收集UI电池型号信息 保存到 m_mapBatteryModel
bool chargingOption::collectUIInfoBatteryModel()
{
	QTableWidgetItem *item = nullptr;
	QString str;
	char szTemp[4] = { 0 };
	int row = ui.tableBatteryModel->rowCount(), colum = ui.tableBatteryModel->columnCount();
	bool ret = false;
	unsigned int iMemMax = 0;

	auto pRound = [](double d, unsigned char p)->double{
		if (d == 0)
			return 0;
		double n = 1;
		for (; p > 0; p--)
			n *= 10;
		if (d > 0.0)
		if (d - (__int64)(d*n) / n >= 5)
			return (__int64)(d*n + 0.5) / n;
		else
			return (__int64)(d*n + 0.5000001) / n;
		else
		if (d - (__int64)(d*n) / n >= 5)
			return (__int64)(d*n - 0.5) / n;
		else
			return (__int64)(d*n - 0.5000001) / n;
	};
	m_mapBatteryModel.clear();
	//内存中最大
	/*for (MAP_BATTERY_MODEL_IT it = m_mapBatteryModel.begin(); it != m_mapBatteryModel.end(); it++)
	{
		if (iMemMax < it->first)
			iMemMax = it->first;
	}*/
	//UI循环检测空
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < colum; j++)
		{
			item = ui.tableBatteryModel->item(i, j);			
			if (item == NULL || item->text().isEmpty()){
				QString str; str.sprintf("<font color='red'>表格中不能留空</font>", iError);
				QMessageBox::warning(this, "提示", str, QMessageBox::Ok);
				return ret ; 
			}
		}
		 
		
	}
	
	//进入UI循环检测
	for (int i = 0; i < row; i++)
	{
		item = ui.tableBatteryModel->item(i, 0);
		str = item->text(); 
		//进入内存循环检测
		//for (MAP_BATTERY_MODEL_IT it = m_mapBatteryModel.begin(); it != m_mapBatteryModel.end(); it++)
		//{
		//	if (str.toInt() ==it->second.id) //编号相同 更新所有电池型号信息
		//	{
		//		//编号
		//		it->second.id = ui.tableBatteryModel->item(i, 0)->text().toInt();
		//		//制造商
		//		str = ui.tableBatteryModel->item(i, 1)->text();
		//		strncpy(it->second.producer, str.toLatin1().data(),str.toLatin1().length());
		//		//无人机型号
		//		str = ui.tableBatteryModel->item(i, 2)->text();
		//		strncpy(it->second.droneModel, str.toLatin1().data(), str.toLatin1().length());
		//		//容量
		//		str = ui.tableBatteryModel->item(i, 3)->text();
		//		it->second.capacity = str.toInt();
		//		//充电电压
		//		str = ui.tableBatteryModel->item(i, 4)->text();
		//		it->second.chargingVoltage = pRound(str.toFloat(),2);
		//		//充电电流
		//		str = ui.tableBatteryModel->item(i, 5)->text();
		//		it->second.chargingCurrent = pRound(str.toFloat(), 2);
		//		//满电电压
		//		str = ui.tableBatteryModel->item(i, 6)->text();
		//		it->second.fullVoltage = pRound(str.toFloat(), 2);
		//		//电池结构
		//		str = ui.tableBatteryModel->item(i, 7)->text();
		//		strncpy(it->second.connectType ,str.toLatin1().data(), str.toLatin1().length());
		//		//是否平衡
		//		str = ui.tableBatteryModel->item(i, 8)->text();
		//		it->second.balance= (str.compare("平衡")==0);

		//		ret = true;
		//		break;  // 跳出本次内存检测
		//	}
		//	else if (iMemMax < str.toInt())  //编号大于内存 新增到内存的map
			{
				stBatteryModelInfo batteryModelInfo;
				batteryModelInfo.id = ui.tableBatteryModel->item(i, 0)->text().toInt();
				//制造商
				//str = ui.tableBatteryModel->item(i, 1)->text();
				//strncpy(batteryModelInfo.producer, str.toLatin1().data(), str.toLatin1().length());
				//无人机型号
				str = ui.tableBatteryModel->item(i, 1)->text();
				strncpy(batteryModelInfo.droneModel, str.toLocal8Bit().data(), str.toLocal8Bit().length());
				//容量
				str = ui.tableBatteryModel->item(i, 2)->text();
				batteryModelInfo.capacity = str.toInt();
				//充电电压
				str = ui.tableBatteryModel->item(i, 3)->text();
				batteryModelInfo.chargingVoltage = pRound(str.toFloat(), 2);
				//充电电流
				str = ui.tableBatteryModel->item(i, 4)->text();
				batteryModelInfo.chargingCurrent = pRound(str.toFloat(), 2);
				//满电电压
				str = ui.tableBatteryModel->item(i, 5)->text();
				batteryModelInfo.fullVoltage = pRound(str.toFloat(), 2);

				//储存电压
				str = ui.tableBatteryModel->item(i, 6)->text();
				batteryModelInfo.storageVoltage = pRound(str.toFloat(), 2);

				//电池结构
				str = ui.tableBatteryModel->item(i, 7)->text();
				strncpy(batteryModelInfo.connectType, str.toLatin1().data(), str.toLatin1().length());
				//是否平衡
				str = ui.tableBatteryModel->item(i, 8)->text();
				batteryModelInfo.balance = (str.compare("平衡") == 0);
				m_mapBatteryModel.insert(MAP_BATTERY_MODEL::value_type(batteryModelInfo.id, batteryModelInfo));
				//break;  // 跳出本次内存检测
			}
		//}
	}

	return true;
}

void chargingOption::cellchangeBattery(int row, int col, int prow, int pcol)
{
	QString ptemp = "";
	QTableWidgetItem *pItem = ui.tableBattery->item(row, col);
	if (pItem)
	{
		ptemp = pItem->text();
		QRect rect;
		int rowCount = ui.tableBattery->rowCount();
		int iMove = rect.left() + (ui.tableBattery->rowCount() > 99 ? 30 : 22);
		if (2 == col)//飞机型号
		{
			m_pCurrentItemDrone = pItem;
			m_pCurrentItemBattery = ui.tableBattery->item(row, 1);
			rect = ui.tableBattery->visualItemRect(pItem);
			rect.setLeft(rect.left() + iMove);
			rect.setRight(rect.right() + iMove);
			rect.setTop(rect.top() + 26);
			rect.setBottom(rect.bottom() + 26);
			m_ComboBoxDroneModel->setGeometry(rect);
			m_ComboBoxDroneModel->setVisible(true);
			m_ComboBoxBatteryModel->setVisible(false);
			QString str = pItem->text();
			m_ComboBoxDroneModel->blockSignals(true);  //初始点击，不发送textChange信号
			m_ComboBoxDroneModel->setCurrentText(str);
			m_ComboBoxDroneModel->blockSignals(false);
			ui.tableBattery->setItemSelected(pItem, true);
		 
		}
		else if (1 == col) //电池型号
		{
			m_pCurrentItemBattery = pItem;
			m_pCurrentItemDrone   = ui.tableBattery->item(row, 2);
			rect = ui.tableBattery->visualItemRect(pItem);
			
			rect.setLeft(rect.left() + iMove);
			rect.setRight(rect.right() + iMove);
			rect.setTop(rect.top() + 26);
			rect.setBottom(rect.bottom() + 26);
			m_ComboBoxBatteryModel->setGeometry(rect);
			m_ComboBoxBatteryModel->setVisible(true);
			m_ComboBoxDroneModel->setVisible(false);
			QString str = pItem->text();
			m_ComboBoxBatteryModel->blockSignals(true);//初始点击，不发送textChange信号
			m_ComboBoxBatteryModel->setCurrentText(str);
			m_ComboBoxBatteryModel->blockSignals(false);
			
		}
		else{
			m_ComboBoxBatteryModel->setVisible(false);
			m_ComboBoxDroneModel->setVisible(false);
		}
	}
}

void chargingOption::cellchangeBatteryModel(int row, int col, int prow, int pcol)
{
	QString ptemp = "";
	QTableWidgetItem *pItem = ui.tableBatteryModel->item(row, col);
	if (pItem)
	{
		ptemp = pItem->text();
		QRect rect; 
		if (8 == col)//平衡
		{
			m_pCurrentItemBalance = pItem; 
			rect = ui.tableBatteryModel->visualItemRect(pItem);
			if (ui.tableBatteryModel->rowCount() > 9)
			{
				rect.setLeft(rect.left() + 23);
				rect.setRight(rect.right() + 23);				
			}
			else if (ui.tableBatteryModel->rowCount() <= 9){
				rect.setLeft(rect.left() + 16);
				rect.setRight(rect.right() + 17);			
			}
			rect.setTop(rect.top() + 26);
			rect.setBottom(rect.bottom() + 26);
			m_ComboBoxBalance->setGeometry(rect);
			m_ComboBoxBalance->setVisible(true); 
			QString str = pItem->text();
			m_ComboBoxBalance->blockSignals(true);  //初始点击，不发送textChange信号
			m_ComboBoxBalance->setCurrentText(str);
			m_ComboBoxBalance->blockSignals(false);
		}
		else{
			m_ComboBoxBalance->setVisible(false);
		}
	}
}

void chargingOption::cellChangedBattery(int row, int col)
{
	isChanged = true;

}

void chargingOption::cellChangedBatteryModel(int row, int col)
{
	ui.btnRefresh_2->setText("撤销");
	isChanged2 = true;
}

void chargingOption::srocllBattery(int pos)
{
	m_ComboBoxBatteryModel->setVisible(false);
	m_ComboBoxDroneModel->setVisible(false);
}

void chargingOption::srocllBatteryModel(int pos)
{
	m_ComboBoxBalance->setVisible(false);
}

void chargingOption::textChangeBattery(const QString & str)
{ 
	if (m_pCurrentItemBattery && m_pCurrentItemDrone){
		ui.tableBattery->blockSignals(true);
		m_pCurrentItemBattery->setText(str);
		isChanged = true;
		ui.btnRefresh_1->setText("撤销");
		//m_ComboBoxBatteryModel->setVisible(false);
		//设置对应无人机
		MAP_BATTERY_MODEL_IT it2 = m_mapBatteryModel.begin();
		for (; it2 != m_mapBatteryModel.end(); it2++)
		{
			if (str.compare(QString::number(it2->second.id)) == 0){
				m_pCurrentItemDrone->setText((it2->second.droneModel));
				break;
			}
		}
		ui.tableBattery->blockSignals(false);
	} 
}

void chargingOption::textChangeDrone(const QString & str)
{
	if (m_pCurrentItemBattery && m_pCurrentItemDrone){
		ui.tableBattery->blockSignals(true);
		m_pCurrentItemDrone->setText(str);
		isChanged = true;
		ui.btnRefresh_1->setText("撤销");
		//m_ComboBoxDroneModel->setVisible(false);
		//设置型号
		MAP_BATTERY_MODEL_IT it2 = m_mapBatteryModel.begin();
		for (; it2 != m_mapBatteryModel.end(); it2++)
		{
			if (str.compare(it2->second.droneModel) == 0){
				m_pCurrentItemBattery->setText(QString::number(it2->first));
				break;
			}
		}
		ui.tableBattery->blockSignals(false); 
	} 
}

void chargingOption::textChangeBalance(const QString & str)
{
	if (m_ComboBoxBalance && m_pCurrentItemBalance)
	{
		ui.tableBatteryModel->blockSignals(true);
		m_pCurrentItemBalance->setText(str);
		m_ComboBoxBalance->setVisible(false);
		ui.tableBatteryModel->blockSignals(false);
		isChanged2 = true;
		ui.btnRefresh_2->setText("撤销");
	}
}

void chargingOption::textChangeCloset(const QString & str)
{ 
	initComboBoxWidget();
	if (str.compare("全部") == 0){
		initTabWidget(m_mapBattery, m_mapBatteryModel);
	}
	else{
		MAP_BATTERY mapBattery;

		unsigned int localid = 0;
		for (MAP_BATTERY_IT it = m_mapBattery.begin(); it != m_mapBattery.end(); it++)
		{
			localid = it->first /100;
			if (str.toInt() == localid){  //当前柜子的电池信息
				stBatteryInfo batteryInfo;
				batteryInfo.dbid = it->second.dbid;
				//strcpy(batteryInfo.enterTime , it->second.enterTime);
				//strcpy(batteryInfo.gridNO , it->second.gridNO);
				strcpy(batteryInfo.id , it->second.id);
				batteryInfo.modelId = it->second.modelId;
				batteryInfo.online = it->second.online;
				//strcpy(batteryInfo.purchasingTime , it->second.purchasingTime);
				batteryInfo.relatedCharger = it->second.relatedCharger;
			/*	batteryInfo.relatedRelay = it->second.relatedRelay;
				batteryInfo.relatedLoop = it->second.relatedLoop;*/
				strcpy(batteryInfo.user , it->second.user);
				mapBattery.insert(MAP_BATTERY::value_type(atoi(batteryInfo.id), batteryInfo));
			}
		}
		initTabWidget(mapBattery, m_mapBatteryModel);
	}
}

void chargingOption::textChangeScanTime(const QString &str)
{
	/*int temp = str.toInt();
	if (temp < 3000){
		QMessageBox::information(this, "提示", "扫描时间间隔不建议小于3000毫秒。");
	}
	else{
		m_ScanDeviceInterval = temp; 
	} */
}
void chargingOption::textChangeSubmitTime(const QString & str){
	/*int temp = str.toInt();
	if (temp < 10000){
		QMessageBox::information(this, "提示", "网络提交时间间隔不建议小于10000毫秒。");
	}
	else{
		m_SubmitInterval = temp; 
	} */
}