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

	//��ȡ�ύ������ʱ����
	m_SubmitInterval = CReadIniFile::getInstance()->readProfileInfo("SET", "SubmitInterval", g_AppPath + "\\set.ini", &iError).toInt();
	if (m_SubmitInterval == 0){
		m_SubmitInterval = ONE_SUBMIT_TIME;
	}

	//��ȡ���ʱ��
	m_ChargeLimitTime = CReadIniFile::getInstance()->readProfileInfo("SET", "ChargeLimitTime", g_AppPath + "\\set.ini", &iError).toInt();
	if (m_ChargeLimitTime == 0){
		m_ChargeLimitTime = CHARGING_LIMIT_TIME;
	}

	//�����¶�
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
	ui.tablePage->removeTab(2);  //��ʱ���� ������¼������ delete 20180723
	 
	ui.btnAdd_1->setVisible(false);
	ui.btnDel_1->setVisible(false);
	QRect rect = ui.btnSave_1->geometry();
	rect.setBottom(rect.bottom() - 120);
	rect.setTop(rect.top() - 120);
	ui.btnSave_1->setGeometry(rect);

	//��ʼ��������
	initComboBoxWidget();

	//���ñ�������  
	initTabWidget(m_mapBattery, m_mapBatteryModel); 

	// ��ʼ��Combo
	ui.cmbCloset->addItem("ȫ��");
	ui.cmbCloset->setCurrentText("ȫ��");
	for (MAP_CLOSET_IT it3 = m_mapCloset.begin(); it3 != m_mapCloset.end(); it3++){
		ui.cmbCloset->addItem(it3->second.id);
	}

	QRegExp regx("[0-9]+$");
	QValidator *validator = new QRegExpValidator(regx, ui.lineEdit);  //ֻ����������
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

//��ʼ��Tab�ؼ�
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
	//ui.tableBattery->setSelectionMode(QAbstractItemView::SingleSelection);  //��ѡ 
	ui.tableBattery->setSelectionBehavior(QAbstractItemView::SelectRows);//������ѡ��

	ui.tableBattery->setSelectionMode(QAbstractItemView::ContiguousSelection); //���ö���ѡ��
	
	//ui.tableBattery->setColumnWidth(0, 120);

	QStringList headerLabels;
	headerLabels << "��ر��" << "����ͺ�" << "���˻��ͺ�" << "���ݿ��Ӧ���";
	ui.tableBattery->setHorizontalHeaderLabels(headerLabels);
	//���ñ༭��ʽ  
	ui.tableBattery->setEditTriggers(QAbstractItemView::DoubleClicked);
	////�Զ��������һ�еĿ��ʹ���ͱ����ұ߽����  
	ui.tableBattery->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);;

	ui.tableBatteryModel->setColumnCount(9);
	ui.tableBatteryModel->setSelectionMode(QAbstractItemView::SingleSelection);  //��ѡ 
	headerLabels.clear();
	headerLabels << "�ͺű��"  << "���˻��ͺ�" << "����" << "����ѹ" << "������" \
		<< "�����ѹ" << "�����ѹ" << "��ؽṹ" << "�Ƿ�ƽ��";
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
		pItemTemp->setFlags(pItemTemp->flags() & (~Qt::ItemIsEditable)); //���ɱ༭
		pItemTemp->setFlags(pItemTemp->flags() & (~Qt::ItemIsSelectable)); //����ѡ��
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
	//�����Ϣ��
	char szTemp[256] = { 0 }; iRow = 0;
	for (MAP_BATTERY_IT it = mapBattery.begin(); it != mapBattery.end(); it++, iRow++)
	{ 
		ui.tableBattery->setItem(iRow, 0, pNewTableWidgetItem(it->second.id));
		pSetDisEnable(ui.tableBattery->item(iRow, 0));   //���ɱ༭������ѡ��

		//����ͺ�
		ui.tableBattery->setItem(iRow, 1, pNewTableWidgetItem(itoa(it->second.modelId, szTemp, 10)));

		pSetDisEnable(ui.tableBattery->item(iRow, 1));   //���ɱ༭������ѡ��

		//���˻��ͺ�
		MAP_BATTERY_MODEL_IT it2 = m_mapBatteryModel.find(it->second.modelId);
		if (it2 != m_mapBatteryModel.end())
		{
			ui.tableBattery->setItem(iRow, 2, pNewTableWidgetItem(it2->second.droneModel));
			pSetDisEnable(ui.tableBattery->item(iRow, 2));   //���ɱ༭������ѡ��

		}
		//���ݿ���
		ui.tableBattery->setItem(iRow, 3, pNewTableWidgetItem(QString::number(it->second.dbid)));
		pSetDisEnable(ui.tableBattery->item(iRow, 3));   //���ɱ༭������ѡ��
		//����ʱ��
		//ui.tableBattery->setItem(iRow, 4, pNewTableWidgetItem(it->second.purchasingTime));
		  
	}
	
	//����ͺ���Ϣ��
	//���ñ༭��ʽ  
	ui.tableBatteryModel->setEditTriggers(QAbstractItemView::DoubleClicked);

	//�Զ��������һ�еĿ��ʹ���ͱ����ұ߽����  
	ui.tableBatteryModel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	//���õ���ͺű�������  
	iRow = mapBatteryModel.size();

	ui.tableBatteryModel->setRowCount(iRow);
	iRow = 0;
	for (MAP_BATTERY_MODEL_IT it = mapBatteryModel.begin(); it != mapBatteryModel.end(); it++, iRow++)
	{
		ui.tableBatteryModel->setItem(iRow, 0, pNewTableWidgetItem(itoa(it->second.id, szTemp, 10)));  //id
		pSetDisEnable(ui.tableBatteryModel->item(iRow, 0));   //���ɱ༭������ѡ��
		m_ComboBoxBatteryModel->addItem(itoa(it->second.id, szTemp, 10));

		//ui.tableBatteryModel->setItem(iRow, 1, pNewTableWidgetItem(it->second.producer));   //������
		ui.tableBatteryModel->setItem(iRow, 1, pNewTableWidgetItem(it->second.droneModel));  //���˻��ͺ�
		m_ComboBoxDroneModel->addItem(it->second.droneModel);

		ui.tableBatteryModel->setItem(iRow, 2, pNewTableWidgetItem(itoa(it->second.capacity, szTemp, 10))); //����
		sprintf(szTemp, "%5.2f", it->second.chargingVoltage);
		ui.tableBatteryModel->setItem(iRow, 3, pNewTableWidgetItem(szTemp));	//��ѹ
		sprintf(szTemp, "%5.2f", it->second.chargingCurrent);
		ui.tableBatteryModel->setItem(iRow, 4, pNewTableWidgetItem(szTemp));  //����
		sprintf(szTemp, "%5.2f", it->second.fullVoltage);
		ui.tableBatteryModel->setItem(iRow, 5, pNewTableWidgetItem(szTemp));  //�����ѹ
		sprintf(szTemp, "%5.2f", it->second.storageVoltage);
		ui.tableBatteryModel->setItem(iRow, 6, pNewTableWidgetItem(szTemp));  //�����ѹ
		sprintf(szTemp, "%s", it->second.connectType);
		ui.tableBatteryModel->setItem(iRow, 7, pNewTableWidgetItem(szTemp));  //��ؽṹ
		sprintf(szTemp, "%s", it->second.balance == 1 ? "ƽ��" : "��ƽ��");
		ui.tableBatteryModel->setItem(iRow, 8, pNewTableWidgetItem(szTemp));  //�Ƿ�ƽ��
	}
	

	m_ComboBoxBalance->addItem(QString("ƽ��"));
	m_ComboBoxBalance->addItem(QString("��ƽ��"));
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

//���棨�����Ϣ��
void chargingOption::OnBtnSave()
{ 
	initComboBoxWidget();
	if (isChanged)
	{
		if (collectUIInfoBattery())
		{
			isChanged = false; //��ԭ
			COperatorFile::GetInstance()->writeChargingInfo(m_mapBattery, &iError);
			iError = 0;
			if (iError)
			{
				QString str; str.sprintf("<font color='red'>����ʧ�ܣ������룺%d</font>", iError); 
				QMessageBox::warning(this, "��ʾ", str, QMessageBox::Ok  );
			}
			else{
				QMessageBox::information(this, "��ʾ", "�������");
				 
				//����ͨ��֪ͨ�����ܳ�籣���䡷�����������
				SendChargingProgramToReadConfig();
				  
				/*QMessageBox message(QMessageBox::NoIcon, "��ʾ", "�������");
				message.setIconPixmap(QPixmap("D:\\1.png"));
				message.exec();*/
			}
		} 
	}
	else{
		QMessageBox::information(this, "��ʾ", "δ�޸ģ�");
	}
	
	//ui.tableBattery->setFocus();
}

//���棨����ͺ���Ϣ)
void chargingOption::OnBtnSave2()
{
	if (isChanged2)
	{
		if (collectUIInfoBatteryModel()){
			isChanged2 = false; //��ԭ 
			COperatorFile::GetInstance()->writeChargingModelInfo(m_mapBatteryModel, &iError);
			if (iError)
			{
				QString str; str.sprintf("<font color='red'>����ʧ�ܣ������룺%d</font>", iError);
				QMessageBox::warning(this, "��ʾ", str, QMessageBox::Ok);
			}
			else{
				QMessageBox::information(this, "��ʾ", "�������");
				//�ͺű�����ɺ� 
				initTabWidget(m_mapBattery, m_mapBatteryModel);
				ui.btnRefresh_2->setText("ˢ��");
				//����ͨ��֪ͨ�����ܳ�籣���䡷�����������
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
		QMessageBox::information(this, "��ʾ", "���ʱ�䲻����С��60���ӡ�");
		ui.lineEdit->setText(QString::number(m_ChargeLimitTime));
		return;
	}
	else{
		m_ChargeLimitTime = temp;
	}
	str = ui.lineEdit_2->text();
	temp = str.toInt();
	if (temp < 10000){
		QMessageBox::information(this, "��ʾ", "�����ύʱ����������С��10000���롣");
		ui.lineEdit_2->setText(QString::number(m_SubmitInterval));

		return;
	}
	else{
		m_SubmitInterval = temp;
	}
	//�¶�
	str = ui.lineEdit_3->text();
	float ftemp = str.toFloat();
	if (ftemp >60 || ftemp < 0){
		QMessageBox::information(this, "��ʾ", "�����¶ȼ���������0~60֮�䡣");
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
		QMessageBox::information(this, "��ʾ", "�������");
		//����ͨ��֪ͨ�����ܳ�籣���䡷�����������
		SendChargingProgramToReadConfig();
	}
}

void chargingOption::OnBtnQuery_1()
{
	m_OperatoreDB.onOpenDbFile();
	bool ff = m_OperatoreDB.onCreateChargedRecordTable();
	ff = false;
}

//������
void chargingOption::OnBtnAdd1()
{
	ui.btnRefresh_1->setText("����");
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
	ui.btnRefresh_2->setText("����");
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
	ui.tableBatteryModel->setItem(row, 0, new QTableWidgetItem(str)); //���
	item = ui.tableBatteryModel->item(row - 1, 2); 	 
	ui.tableBatteryModel->setItem(row, 2, new QTableWidgetItem(item->text()));// �������
	item = ui.tableBatteryModel->item(row - 1, 3);
	ui.tableBatteryModel->setItem(row, 3, new QTableWidgetItem(item->text()));//����ѹ
	item = ui.tableBatteryModel->item(row - 1, 4);
	ui.tableBatteryModel->setItem(row, 4, new QTableWidgetItem(item->text()));//������
	item = ui.tableBatteryModel->item(row - 1, 5);
	ui.tableBatteryModel->setItem(row, 5, new QTableWidgetItem(item->text()));//�����ѹ
	item = ui.tableBatteryModel->item(row - 1, 6);
	ui.tableBatteryModel->setItem(row, 6, new QTableWidgetItem(item->text()));//
	item = ui.tableBatteryModel->item(row - 1, 7);
	ui.tableBatteryModel->setItem(row, 7, new QTableWidgetItem(item->text()));//��ؽṹ 
}

void chargingOption::OnBtnRefresh1()
{	
	QString str = ui.cmbCloset->currentText();
	textChangeCloset(str);
	if (ui.btnRefresh_1->text() == "����")
	{
		ui.btnRefresh_1->setText("ˢ��");
		isChanged = false;
	}
}

void chargingOption::OnBtnRefresh2()
{
	initComboBoxWidget();
	initTabWidget(m_mapBattery, m_mapBatteryModel);
	if (ui.btnRefresh_2->text() == "����")
	{
		ui.btnRefresh_2->setText("ˢ��");
		isChanged2 = false;
	}
}

void chargingOption::OnBtnDel1()
{	
	ui.tableBattery->removeRow(ui.tableBattery->currentRow());
	ui.btnRefresh_1->setText("����");
	m_ComboBoxBatteryModel->setVisible(false);
	m_ComboBoxDroneModel->setVisible(false);
	isChanged = true;
}

void chargingOption::OnBtnDel2()
{
	initComboBoxWidget();
	ui.tableBatteryModel->removeRow(ui.tableBatteryModel->currentRow());
	ui.btnRefresh_2->setText("����");
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
		strcpy(stObj.strInfo, "������Ϣ�������Ѿ��޸�");
		stObj.nTime1 = m_ChargeLimitTime;
		stObj.nSubmitTime = m_SubmitInterval;
		stObj.fOverHeatTemperature = m_fOverHeatTemperature;


		COPYDATASTRUCT copydata;
		copydata.dwData = CUSTOM_TYPE;  // �û���������
		//copydata.lpData = data.data();  //���ݴ�С
		//copydata.cbData = data.size();  // ָ�����ݵ�ָ��
		copydata.lpData = &stObj;
		copydata.cbData = sizeof(stProcessSimpltData);

		HWND sender = (HWND)effectiveWinId();

		::SendMessage(hwnd, WM_COPYDATA, reinterpret_cast<WPARAM>(sender), reinterpret_cast<LPARAM>(&copydata));
	}
	else{
		;
		//QMessageBox::warning(this, "��ʾ", "δ�ҵ���" + g_winTitle + "�����ڡ�\n��鿴�����Ƿ�رա�", QMessageBox::Ok);
	}
}

//�ر���� :���һ���Ƿ���Ҫ����
void chargingOption::closeEvent(QCloseEvent *event)
{
	if (isChanged&& isChanged2)
	{
		QMessageBox::StandardButton button;
		button = (QMessageBox::StandardButton)QMessageBox::question(this, "�˳�����",
			"�������ݻ�δ���棬�Ƿ񱣴�?", 
			"����", "������" );

		if (button == 0) { //����
			
			//д������Ϣ
			if (isChanged)
				COperatorFile::GetInstance()->writeChargingInfo(m_mapBattery, &iError); 

			//д�����ͺ���Ϣ
			if (isChanged2)
				COperatorFile::GetInstance()->writeChargingModelInfo( m_mapBatteryModel, &iError);
			event->ignore();  
		}
		else if (button == 1) {
			event->accept();  //���� 
		} 
	}else
		event->accept();   
}

//�ռ�UI�����Ϣ ���浽 m_mapBattery
bool chargingOption::collectUIInfoBattery()
{
	QTableWidgetItem *item = nullptr;  
	QString str, str2, str3 ,str4;
	QByteArray byteArray;
	int row = ui.tableBattery->rowCount(), colum=ui.tableBattery->columnCount();
	bool ret = true;
	do 
	{
		//UIѭ������
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < colum; j++)
			{
				item = ui.tableBattery->item(i, j);
				 if ( item == NULL || item->text().isEmpty()){
				
					QString str = QString("<font color='red'>���%1��%2���в�������</font>").arg(i).arg(j); 
					QMessageBox::warning(this, "��ʾ", str, QMessageBox::Ok);				 
					return false; 
				}
			}  
		}
		//����UIѭ�����
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

			//�����ڴ�ѭ�����
			for (MAP_BATTERY_IT it = m_mapBattery.begin(); it != m_mapBattery.end(); it++)
			{	
				//��ر����ͬ
				if (str.compare(it->second.id) == 0)  
				{
					it->second.modelId = str2.toInt();  
					break;  // ���������ڴ���
				} 
				//else if (iMemMax <str.toInt())  //��Ŵ����ڴ� �������ڴ��map
				//{
				//	stBatteryInfo batteryInfo;				
				//	batteryInfo.dbid = 0;  //dbid ��ֵ�д����飬 ��Ҫ�������ݵĵ��id��ֵ��
				//	strncpy(batteryInfo.id, (str.toLocal8Bit()).data(), 4);
				//	batteryInfo.modelId = ui.tableBattery->item(i, 1)->text().toInt();
				//	str = ui.tableBattery->item(i, 3)->text();
				//	strncpy(batteryInfo.user, str.toLocal8Bit().data(), str.toLocal8Bit().length());
				//	str = ui.tableBattery->item(i, 4)->text();
				//	//strncpy(batteryInfo.purchasingTime, str.toLatin1().data(), str.toLatin1().length());
				//	m_mapBattery.insert(MAP_BATTERY::value_type(atoi(batteryInfo.id), batteryInfo));
				//	break;  // ���������ڴ���
				//}
			}		 
		} 
	} while (0);
	
	return ret;
}

//�ռ�UI����ͺ���Ϣ ���浽 m_mapBatteryModel
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
	//�ڴ������
	/*for (MAP_BATTERY_MODEL_IT it = m_mapBatteryModel.begin(); it != m_mapBatteryModel.end(); it++)
	{
		if (iMemMax < it->first)
			iMemMax = it->first;
	}*/
	//UIѭ������
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < colum; j++)
		{
			item = ui.tableBatteryModel->item(i, j);			
			if (item == NULL || item->text().isEmpty()){
				QString str; str.sprintf("<font color='red'>����в�������</font>", iError);
				QMessageBox::warning(this, "��ʾ", str, QMessageBox::Ok);
				return ret ; 
			}
		}
		 
		
	}
	
	//����UIѭ�����
	for (int i = 0; i < row; i++)
	{
		item = ui.tableBatteryModel->item(i, 0);
		str = item->text(); 
		//�����ڴ�ѭ�����
		//for (MAP_BATTERY_MODEL_IT it = m_mapBatteryModel.begin(); it != m_mapBatteryModel.end(); it++)
		//{
		//	if (str.toInt() ==it->second.id) //�����ͬ �������е���ͺ���Ϣ
		//	{
		//		//���
		//		it->second.id = ui.tableBatteryModel->item(i, 0)->text().toInt();
		//		//������
		//		str = ui.tableBatteryModel->item(i, 1)->text();
		//		strncpy(it->second.producer, str.toLatin1().data(),str.toLatin1().length());
		//		//���˻��ͺ�
		//		str = ui.tableBatteryModel->item(i, 2)->text();
		//		strncpy(it->second.droneModel, str.toLatin1().data(), str.toLatin1().length());
		//		//����
		//		str = ui.tableBatteryModel->item(i, 3)->text();
		//		it->second.capacity = str.toInt();
		//		//����ѹ
		//		str = ui.tableBatteryModel->item(i, 4)->text();
		//		it->second.chargingVoltage = pRound(str.toFloat(),2);
		//		//������
		//		str = ui.tableBatteryModel->item(i, 5)->text();
		//		it->second.chargingCurrent = pRound(str.toFloat(), 2);
		//		//�����ѹ
		//		str = ui.tableBatteryModel->item(i, 6)->text();
		//		it->second.fullVoltage = pRound(str.toFloat(), 2);
		//		//��ؽṹ
		//		str = ui.tableBatteryModel->item(i, 7)->text();
		//		strncpy(it->second.connectType ,str.toLatin1().data(), str.toLatin1().length());
		//		//�Ƿ�ƽ��
		//		str = ui.tableBatteryModel->item(i, 8)->text();
		//		it->second.balance= (str.compare("ƽ��")==0);

		//		ret = true;
		//		break;  // ���������ڴ���
		//	}
		//	else if (iMemMax < str.toInt())  //��Ŵ����ڴ� �������ڴ��map
			{
				stBatteryModelInfo batteryModelInfo;
				batteryModelInfo.id = ui.tableBatteryModel->item(i, 0)->text().toInt();
				//������
				//str = ui.tableBatteryModel->item(i, 1)->text();
				//strncpy(batteryModelInfo.producer, str.toLatin1().data(), str.toLatin1().length());
				//���˻��ͺ�
				str = ui.tableBatteryModel->item(i, 1)->text();
				strncpy(batteryModelInfo.droneModel, str.toLocal8Bit().data(), str.toLocal8Bit().length());
				//����
				str = ui.tableBatteryModel->item(i, 2)->text();
				batteryModelInfo.capacity = str.toInt();
				//����ѹ
				str = ui.tableBatteryModel->item(i, 3)->text();
				batteryModelInfo.chargingVoltage = pRound(str.toFloat(), 2);
				//������
				str = ui.tableBatteryModel->item(i, 4)->text();
				batteryModelInfo.chargingCurrent = pRound(str.toFloat(), 2);
				//�����ѹ
				str = ui.tableBatteryModel->item(i, 5)->text();
				batteryModelInfo.fullVoltage = pRound(str.toFloat(), 2);

				//�����ѹ
				str = ui.tableBatteryModel->item(i, 6)->text();
				batteryModelInfo.storageVoltage = pRound(str.toFloat(), 2);

				//��ؽṹ
				str = ui.tableBatteryModel->item(i, 7)->text();
				strncpy(batteryModelInfo.connectType, str.toLatin1().data(), str.toLatin1().length());
				//�Ƿ�ƽ��
				str = ui.tableBatteryModel->item(i, 8)->text();
				batteryModelInfo.balance = (str.compare("ƽ��") == 0);
				m_mapBatteryModel.insert(MAP_BATTERY_MODEL::value_type(batteryModelInfo.id, batteryModelInfo));
				//break;  // ���������ڴ���
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
		if (2 == col)//�ɻ��ͺ�
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
			m_ComboBoxDroneModel->blockSignals(true);  //��ʼ�����������textChange�ź�
			m_ComboBoxDroneModel->setCurrentText(str);
			m_ComboBoxDroneModel->blockSignals(false);
			ui.tableBattery->setItemSelected(pItem, true);
		 
		}
		else if (1 == col) //����ͺ�
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
			m_ComboBoxBatteryModel->blockSignals(true);//��ʼ�����������textChange�ź�
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
		if (8 == col)//ƽ��
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
			m_ComboBoxBalance->blockSignals(true);  //��ʼ�����������textChange�ź�
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
	ui.btnRefresh_2->setText("����");
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
		ui.btnRefresh_1->setText("����");
		//m_ComboBoxBatteryModel->setVisible(false);
		//���ö�Ӧ���˻�
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
		ui.btnRefresh_1->setText("����");
		//m_ComboBoxDroneModel->setVisible(false);
		//�����ͺ�
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
		ui.btnRefresh_2->setText("����");
	}
}

void chargingOption::textChangeCloset(const QString & str)
{ 
	initComboBoxWidget();
	if (str.compare("ȫ��") == 0){
		initTabWidget(m_mapBattery, m_mapBatteryModel);
	}
	else{
		MAP_BATTERY mapBattery;

		unsigned int localid = 0;
		for (MAP_BATTERY_IT it = m_mapBattery.begin(); it != m_mapBattery.end(); it++)
		{
			localid = it->first /100;
			if (str.toInt() == localid){  //��ǰ���ӵĵ����Ϣ
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
		QMessageBox::information(this, "��ʾ", "ɨ��ʱ����������С��3000���롣");
	}
	else{
		m_ScanDeviceInterval = temp; 
	} */
}
void chargingOption::textChangeSubmitTime(const QString & str){
	/*int temp = str.toInt();
	if (temp < 10000){
		QMessageBox::information(this, "��ʾ", "�����ύʱ����������С��10000���롣");
	}
	else{
		m_SubmitInterval = temp; 
	} */
}