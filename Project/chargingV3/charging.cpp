#include "charging.h"
#pragma execution_character_set("utf-8")

charging::charging(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//QRect rect = this->geometry();
	//this->setFixedSize(QSize(rect.width(), rect.height())); //�̶����ڴ�С

	//��󻯴�����ʾ
	//setWindowState(Qt::WindowMaximized);
	meTimer = NULL;	//��ʱ��ָ��

	m_bContinueScan = true;// ����ɨ���־

	m_bContinueSubmit = true;//�����ύ�����־

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
	qDebug() << "Close��1����رպ���";
	if (meTimer)
		meTimer->stop();
	qDebug() << "Close��2�رն�ʱ��"; 
	m_CommandQueue.stop();
	qDebug() << "Close��3�ر��������";
	SERIAL_PORT->ClosePort();
	//my_Serial.serialClose();
	qDebug() << "Close��4�رմ���";
	event->accept();  //�����˳��źţ������˳�
}



void charging::OnExitApp()
{
	this->close();
}

//ת�� ���id��Ӧ ����Index
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

//�����ID ���Ҷ�Ӧ�̵����պϵĵ��ID
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
				printfDebugInfo(strBatteryId + "���δƥ������", enDebugInfoPriority::DebugInfoLevelOne, true);
				return false;
			}
			itLevel = m_mapLevel.find(itCharger->second.nLevel);
			if (itLevel == m_mapLevel.end()){
				printfDebugInfo(strBatteryId + "���δƥ��㼶", enDebugInfoPriority::DebugInfoLevelOne, true);
				return false;
			}

			itBatteryModel = m_mapBatteryModel.find(itBattery->second.modelId);
			if (itBatteryModel == m_mapBatteryModel.end())
			{
				printfDebugInfo(strBatteryId + "���δƥ�����ͺ�", enDebugInfoPriority::DebugInfoLevelOne, true);
				return false;
			}  
		}
		else
		{
			printfDebugInfo(strBatteryId + "δƥ����", enDebugInfoPriority::DebugInfoLevelOne, true);
			return false;
		}
	}
	else{
		printfDebugInfo(strBatteryId + "δƥ�������", enDebugInfoPriority::DebugInfoLevelOne, true);
		return false;
	}
	return true;
}
//���������
bool charging::detectChargingCondition(QString strBatteryId, int* iResult, bool showDebugInfo)
{
	if (SERIAL_PORT->isOpen() == false)
	{
		if (showDebugInfo)
		{
			printfDebugInfo("����δ��", enDebugInfoPriority::DebugInfoLevelOne, true);
			showTipsMessagebox(2, "����δ�򿪣����������Ч!");
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
				printfDebugInfo(strBatteryId + "����������ߣ����������Ч!", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "����������ߣ����������Ч!");
			}
			*iResult = ERROR_NO_CHARGER;
			return false;
		}
		if (true == itCharger->second.isOverHeat){
			if (showDebugInfo)
			{
				printfDebugInfo(strBatteryId + "���ȣ����������Ч!", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "���ȣ����������Ч!");
			}
			*iResult = ERROR_OVER_HEAT;
			return false;
		}		 

		//�����ܵ�ظ��ݵ�ѹ�жϵ���Ƿ����			
		/*if (itBatteryModel->second.balance == false)
		{
			if (itCharger->second.getAverage(itCharger->second.fVoltage) <3.1)
			{
				if (showDebugInfo)
				{
					printfDebugInfo(strBatteryId + "���δ��⣬������Ч", enDebugInfoPriority::DebugInfoLevelOne, true);
					showTipsMessagebox(1, strBatteryId + "���δ��⣬���������Ч!");
				}
				*iResult = ERROR_NO_BATTERY;
				return false;
			}
		}*/

		if (itCharger->second.isCharging == true)
		{
			if (showDebugInfo)
			{
				printfDebugInfo("������ڳ�磬���������Ч", enDebugInfoPriority::DebugInfoLevelOne);
				showTipsMessagebox(1, strBatteryId + "������ڳ�磬���������Ч!");
			}
			return false;
		}
		if (itCharger->second.isDisCharging)
		{
			if (showDebugInfo){
				printfDebugInfo(strBatteryId + "���ڷŵ�", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "������ڷŵ磬���������Ч!");
			}
			*iResult = ERROR_BATTERY_CHARGING;
			return false;
		}
	}
	return true;
}

//���ŵ�����
bool charging::detectDisChargingCondition(QString strBatteryId, int* iResult, bool showDebugInfo)
{
	if (SERIAL_PORT->isOpen() == false)
	{
		if (showDebugInfo)
			printfDebugInfo("����δ��", enDebugInfoPriority::DebugInfoLevelOne, true);

		*iResult = ERROR_DONT_OPEN_SERIAL;
		return false;
	}
	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
	if (getBatteryIdRelatedInfo(strBatteryId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
	{
		if (false == itBattery->second.isExisted)
		{
			//printfDebugInfo(QString("<p><font size=\"%1\" color=\"%2\">%3</font></p>").arg(12).arg("red").arg(strBatteryId + "��ز����ڣ�������Ч"), enDebugInfoPriority::DebugInfoLevelOne);
			if (showDebugInfo)
			{
				printfDebugInfo(strBatteryId + "��ز����ڣ��ŵ�������Ч", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "����������ߣ��ŵ�������Ч!"); 
			}
			*iResult = ERROR_NO_BATTERY;
			return false;
		}
		if (itBatteryModel->second.balance)
		{
			if (showDebugInfo)
			{
				printfDebugInfo(strBatteryId + "�������ܵ�أ�Ŀǰ��֧�ַŵ�", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "�������ܵ�أ�Ŀǰ��֧�ַŵ�!");
			}
			*iResult = ERROR_NO_BATTERY;
			return false;
		}
		if (itCharger->second.isCharging)
		{
			if (showDebugInfo)
			{
				printfDebugInfo(strBatteryId + "���ڳ��", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "���ڳ�磬����ֹͣ���!");
			}
			*iResult = ERROR_BATTERY_CHARGING;
			return false;
		}
		if (itCharger->second.isDisCharging)
		{
			if (showDebugInfo)
			{
				printfDebugInfo(strBatteryId + "���ڷŵ�", enDebugInfoPriority::DebugInfoLevelOne, true);
				showTipsMessagebox(1, strBatteryId + "���ڷŵ�!");
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

//����ID��ƴװ�����������������
bool charging::chargingByLocalID(QString strBatteryId, int *iResult, bool showDebugInfo)
{

	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
	if (getBatteryIdRelatedInfo(strBatteryId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
	{
		if (!detectChargingCondition(strBatteryId, iResult, showDebugInfo))
		{
			return false;
		} 
		//���ó���־
		itCharger->second.isCharging = true;
		itCharger->second.timeLockChargingState.restart();
		//��ʼ����ʱ
		itCharger->second.updateBeginChargeTime();

		//��װ�����Ϣ
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

		//���ó���־
		itCharger->second.isCharging = true; 
	} 

	return true;
}
//���ݵ��ID��ƴװֹͣ��������������
bool charging::stopByLocalID(QString strBatteryId)
{
	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
	if (getBatteryIdRelatedInfo(strBatteryId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
	{
		if (itCharger->second.isCharging == false){
			return false;
		}
		QVector<stCommand> vtStCommand;
		//���ó���־:����
		itCharger->second.isCharging = false;
		itCharger->second.timeLockChargingState.restart();
		//��װֹͣ���� ��ӵ�������� 
		stCommand stComm(packageCommand("P," + QString::number(itCharger->second.id) + ","), stCommand::hight);
		vtStCommand.append(stComm);
		if (vtStCommand.size())
			m_CommandQueue.addVtCommand(vtStCommand);
	} 	
	return true;
}
//��δ��⵽��ء�δ���õ�ء�����������ʱ������С��� ��������Ҳ��ڳ��״̬�������ɡ�,����״̬��ʾ���ȴ��С�
QString charging::getBatteryState(int indexBattery)
{
	QString strResult;
	QString strBState = battery_state[indexBattery];//"�������" : "δ���õ��";
	float fvol = battery_voltage[indexBattery].toFloat();  //��ѹ
	QString strBatteryId = battery_local_id[indexBattery];//���id
	MAP_CLOSET_IT itCloset;	MAP_BATTERY_IT itBattery; MAP_BATTERY_MODEL_IT itBatteryModel; MAP_CHARGER_IT itCharger; MAP_LEVEL_IT itLevel;
	if (getBatteryIdRelatedInfo(strBatteryId, itCloset, itBattery, itBatteryModel, itCharger,itLevel))
	{
		if (itCharger->second.isCharging == true)//����� �����
		{
			strResult = "�����";
		}
		else  //����� δ���
		{
			if (strBState == "δ���õ��"){
				strResult = strBState;
			}
			else{
				if (fvol == 0 || fvol < itBatteryModel->second.fullVoltage - 0.2)  //�Ƚϵ�ѹ
					strResult = "�ȴ���";
				else if (fvol >= itBatteryModel->second.fullVoltage - 0.2){
					strResult = "������";
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

bool charging::nativeEvent(const QByteArray &eventType, void *message, long *result)//�¼�ץ�����������ͨ��
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
				//QMessageBox::information(this, QStringLiteral("��ʾ"), strMessage);
				printfDebugInfo( strMessage+",���¶�ȡ�С�", enDebugInfoPriority::DebugInfoLevelOne);
				//��ӵ���ͺŸ����ж� add20180418
				MAP_BATTERY mapBatteryOld = m_mapBattery;
				//m_vtUiChargGrid[uiIndex]->setChargerState([index]);				
				readConfig();
				m_nChargeLimitTime = stObj->nTime1; 
				m_SubmitInterval = stObj->nSubmitTime;	
				m_fOverHeatTemperature = stObj->fOverHeatTemperature;
				refreshCurrentUI();
				show_all_data_now(); 
				
				//�ԱȺ��ύһ���ͺ����ݵ�������
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
	return false;		//�����ж�
}



void charging::OnAddCommamdIntoQueue(stCommand strCommand)
{
	m_CommandQueue.addCommamd(strCommand);
}

//��ɶ�ȡһ�����ӱ�� ��������һ�����Ӷ�ȡ��Ϣ����
void charging::OnReadyGetBatteryState(int nClosetId)  
{
	//static QVector<unsigned int > vtClosetId;  vtClosetId.clear();
	//static int nLoopClosetId =1, nNextClosetId;
	//
	//for (MAP_CLOSET_IT itCloset = m_mapCloset.begin(); itCloset != m_mapCloset.end(); itCloset++){
	//	vtClosetId.append(itCloset->first);
	//}
	//if (nClosetId == m_iCurrentCloset){
	//	//��ȡ��һ������		
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
		//��ȡ��ǰ���� 
		scanOneBatteryState(m_iCurrentCloset, stCommand::normal);
	}  
}

void charging::appendChargingQueue(QString strBatteryId)
{
	//�Ƚϵ��ID,��������δ�ܳ�������
	bool bSame = false;
	for (int i = 0; i < m_vtApplyDontCharge.size(); i++){
		if (m_vtApplyDontCharge[i].nBatteryID == strBatteryId.toInt())
		{
			bSame = true; //����Ѿ�����ͬ�ģ�������
			break;
		}
	}
	if (!bSame)//���ID����ͬ���򱣴�
	{
		stApplyBatteryDontCharge obj;
		obj.nBatteryID = strBatteryId.toInt();
		obj.strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
		m_vtApplyDontCharge.append(obj);
	}
}

//ȡ���������ж�Ӧ��id
void charging::removeChargingQueue(QString strBatteryId)
{
	for (int i = m_vtApplyDontCharge.size() - 1; i >=0 ; i--){
		if (m_vtApplyDontCharge[i].nBatteryID == strBatteryId.toInt())
		{
			m_vtApplyDontCharge.remove(i);
		}
	}
}

//���浽�ļ�
void charging::writeApplyChargingQueueToFile()
{
	//�Ƚ� ����δ���ı��� �Ƿ��в�ͬ����ͬ����µ������ļ�	
	 
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
//��ȡ����
void charging::extractNumber(QString strSrouce, int& iNumber)  
{
	char a[50] = {0};
	char b[50];
	char*  ch;
	QByteArray ba = strSrouce.toLocal8Bit(); // must
	strncpy_s(a, 50, ba.data(), 50);//
	int cnt_index = 0, cnt_int = 0;
	//cnt_int ���ڴ���ַ����е�����.
	//cnt_index ��Ϊ�ַ���b���±�.

	for (int i = 0; i < strlen(a); ++i)
	{
		if (a[i] >= '0'&& a[i] <= '9') //���������.
		{
			cnt_int *= 10;
			cnt_int += a[i] - '0'; //�����ַ���ascii-�ַ�'0'��ascii��͵��ڸ�����.
		}

		else if ((a[i] >= 'a'&&a[i] <= 'z') || (a[i] >= 'A'&&a[i] <= 'Z')) //�������ĸ.
		{
			b[cnt_index++] = a[i]; //������ַ�,�����ӵ�b������.
		}
	}
	iNumber = cnt_int;
	b[cnt_index++] = '\0'; //�����ַ���������.

}