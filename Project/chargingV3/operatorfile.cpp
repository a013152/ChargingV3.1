#include "operatorfile.h"
#include "ReadIniFile.h"
#pragma execution_character_set("utf-8")
static COperatorFile* s_this = nullptr;
COperatorFile::COperatorFile()
{
	m_pTextLogin = nullptr;
	m_pLogfile = nullptr;
	m_pTextDebug = nullptr;
	m_pTextApplyBatteryLog = nullptr;
	m_pLogfileApplyBattery = nullptr;
	m_pDebugLogfile = nullptr;
}


COperatorFile::~COperatorFile()
{
	if (m_pTextLogin)
	{
		delete m_pTextLogin;
		m_pTextLogin = nullptr;
	}
	if (m_pTextApplyBatteryLog)
	{
		delete m_pTextApplyBatteryLog;
	}
}
COperatorFile* COperatorFile::GetInstance() //��ȡʵ��
{
	if (s_this == nullptr)
	{
		s_this = new COperatorFile();
	}
	return s_this;
}
//��ȡ�������ã����񡢵�ء�����ͺ�
void COperatorFile::readAllConfig(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery, MAP_BATTERY_MODEL& mapBatteryModel, MAP_CHARGER& mapCharger,   int* iError)
{
	//��ȡ������Ϣ
	readChargingClosetInfo(mapCloset, iError);

	//��ȡ�����Ϣ
	readBatteryInfo(mapBattery, iError);

	//��ȡ������ͺ���Ϣ
	readBatteryModel(mapBatteryModel, iError);

	//��ȡ�̵�����Ϣ
	//readRelayInfo(mapRelay, iError);

	//��ȡ�������Ϣ
	readChargerInfo(mapCharger, iError);

	return;
}

//��ȡ��������
void COperatorFile::readChargingClosetInfo(MAP_CLOSET& mapCloset, int* iError)
{
	QString strChangingClosetPath = m_strAppPath; strChangingClosetPath += "\\chargingCloset.txt";
	QFile file(strChangingClosetPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!" << endl;
		*iError = ChargingClosetError::filePathError;
		return;
	}
	QByteArray qba1, qba2; QString qstr1, qstr2; QStringList qstrlist; char szTemp[256] = { 0 };
	while (!file.atEnd())
	{
		qba1 = file.readLine();
		qba1.replace(0x0B, 0x0D);
		qstr1 = QString::fromLocal8Bit(qba1, qba1.length());//QByteArrayתQString ��ȡ����   
		qstrlist = qstr1.split(",");
		if (qstrlist.size() == 1)
			continue;
		stChargingCloset chargingCloset;
		transitionContent1(qstrlist[0], qstr2, qba2, szTemp); //id	 
		strncpy(chargingCloset.id, szTemp, strlen(szTemp));

		mapCloset.insert(MAP_CLOSET::value_type(atoi(chargingCloset.id), chargingCloset));
	}

}
//��ȡ�����Ϣ
void COperatorFile::readBatteryInfo(MAP_BATTERY& mapBattery, int* iError)
{
	QString strBatteryPath = m_strAppPath; strBatteryPath += "\\battery.txt";
	QFile file(strBatteryPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!" << endl;
		*iError = ChargingClosetError::filePathError;
		return;
	}
	int nLevelLimit = CReadIniFile::getInstance()->readProfileInfo("SET", "nlevelLimit",  QString::fromLocal8Bit(g_AppPath) + "\\set.ini", iError).toInt();
	QByteArray qba1, qba2; QString qstr1, qstr2; QStringList qstrlist; char szTemp[256] = { 0 }; int nIndexMap = 0;
	 
	while (!file.atEnd())
	{
		qba1 = file.readLine();
		qba1.replace(0x0B, 0x0D);
		qstr1 = QString::fromLocal8Bit(qba1, qba1.length());//QByteArrayתQString ��ȡ����   
		qstrlist = qstr1.split(",");
		if (qstrlist.size() == 1)
			continue;
		stBatteryInfo batteryInfo;
		transitionContent1(qstrlist[0], qstr2, qba2, szTemp); //dbid	 
		batteryInfo.dbid = atoi(&szTemp[0]);

		transitionContent1(qstrlist[1], qstr2, qba2, szTemp); //id	 
		strncpy(batteryInfo.id, szTemp, strlen(szTemp));

		transitionContent1(qstrlist[2], qstr2, qba2, szTemp);  //online	 
		batteryInfo.online = atoi(&szTemp[0]);

		transitionContent1(qstrlist[3], qstr2, qba2, szTemp);  //modelId	 
		batteryInfo.modelId = atoi(&szTemp[0]); 

		transitionContent1(qstrlist[4], qstr2, qba2, szTemp);  //relatedCharger;  //������������ư� ��ַ 		
		batteryInfo.relatedCharger = atoi(&szTemp[0]);

		transitionContent1(qstrlist[5], qstr2, qba2, szTemp);  //nlevel	//�㼶 
		batteryInfo.nlevel = atoi(&szTemp[0]); 
		if (nLevelLimit < batteryInfo.nlevel && nLevelLimit > 0)
			break;
		 
		mapBattery.insert(MAP_BATTERY::value_type(atoi(batteryInfo.id), batteryInfo));		
	}
}
//��ȡ������ͺ���Ϣ
void COperatorFile::readBatteryModel(MAP_BATTERY_MODEL& mapBatteryModel, int* iError)
{
	QString strBatteryModelPath = m_strAppPath; strBatteryModelPath += "\\batteryModel.txt";
	QFile file(strBatteryModelPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!" << endl;
		*iError = ChargingClosetError::filePathError;
		return;
	}
	QByteArray qba1, qba2; QString qstr1, qstr2; QStringList qstrlist; char szTemp[256] = { 0 };
	while (!file.atEnd())
	{
		qba1 = file.readLine();
		qba1.replace(0x0B, 0x0D);
		qstr1 = QString::fromLocal8Bit(qba1, qba1.length());//QByteArrayתQString ��ȡ����   
		qstrlist = qstr1.split(",");
		if (qstrlist.size() == 1  )
			continue;
		else if (qstrlist.size() < 9){
			QString text = qstr1 + "\n����ͺ�������Ϣ����9��,���ؼ�顣";
			QMessageBox::warning(NULL, "��ʾ", text);
			*iError = ChargingClosetError::configInfoError;
			return;
		}
		stBatteryModelInfo batteryModel;
		transitionContent1(qstrlist[0], qstr2, qba2, szTemp); //id	 
		batteryModel.id = atoi(&szTemp[0]);

		transitionContent1(qstrlist[1], qstr2, qba2, szTemp); //droneModel	 
		strncpy(batteryModel.droneModel, szTemp, strlen(szTemp)); 

		transitionContent1(qstrlist[2], qstr2, qba2, szTemp);  //capacity ���� 
		batteryModel.capacity = atoi(&szTemp[0]);	

		transitionContent1(qstrlist[3], qstr2, qba2, szTemp);  //charging_voltage	//����ѹ 
		batteryModel.chargingVoltage = atof(szTemp);

		transitionContent1(qstrlist[4], qstr2, qba2, szTemp);  //charging_current  ������	 
		batteryModel.chargingCurrent = atof(szTemp);

		transitionContent1(qstrlist[5], qstr2, qba2, szTemp);  //storage_voltage  �����ѹ	 
		batteryModel.storageVoltage = atof(szTemp);

		transitionContent1(qstrlist[6], qstr2, qba2, szTemp);  //full_voltage  �����ѹ	 
		batteryModel.fullVoltage = atof(szTemp);

		transitionContent1(qstrlist[7], qstr2, qba2, szTemp);  //connect_type  �������͵�ѹ	 
		strncpy(batteryModel.connectType, szTemp, strlen(szTemp));

		transitionContent1(qstrlist[8], qstr2, qba2, szTemp);  //balance  �Ƿ�ƽ�� 
		batteryModel.balance = atof(szTemp);


		mapBatteryModel.insert(MAP_BATTERY_MODEL::value_type(batteryModel.id, batteryModel));
	}
}

void COperatorFile::readChargerInfo(MAP_CHARGER& mapCharger, int* iError)
{
	QString strChangertPath = m_strAppPath; strChangertPath += "\\charger.txt";
	QFile file(strChangertPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!" << endl;
		QMessageBox::warning(NULL, "��ʾ", strChangertPath+"\n�ļ�·������");
		*iError = ChargingClosetError::filePathError;
		return;
	}
	int nLevelLimit = CReadIniFile::getInstance()->readProfileInfo("SET", "nlevelLimit",  QString::fromLocal8Bit(g_AppPath) + "\\set.ini", iError).toInt();
	QByteArray qba1, qba2; QString qstr1, qstr2; QStringList qstrlist; char szTemp[256] = { 0 };
	while (!file.atEnd())
	{
		qba1 = file.readLine();
		qba1.replace(0x0B, 0x0D);
		qstr1 = QString::fromLocal8Bit(qba1, qba1.length());//QByteArrayתQString ��ȡ����   
		qstrlist = qstr1.split(",");
		if (qstrlist.size() == 1)
			continue;
		stCharger obj;
		transitionContent1(qstrlist[0], qstr2, qba2, szTemp); //id	 
		obj.id = atoi(szTemp);
		transitionContent1(qstrlist[1], qstr2, qba2, szTemp); // �㼶
		obj.nLevel = atoi(szTemp);
		if (obj.nLevel > nLevelLimit && nLevelLimit > 0)
			break;
		transitionContent1(qstrlist[2], qstr2, qba2, szTemp); // ����
		obj.chargerType = (enChargerType)atoi(szTemp);
		

		mapCharger.insert(MAP_CHARGER::value_type(obj.id, obj));
	}
}

//void COperatorFile::readRelayInfo(MAP_RELAY& mapRelay, int* iError)
//{
//	QString strChangertPath = m_strAppPath; strChangertPath += "\\relay.txt";
//	QFile file(strChangertPath);
//	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//	{
//		qDebug() << "Can't open the file!" << endl;
//		QMessageBox::warning(NULL, "��ʾ", strChangertPath + "\n�ļ�·������");
//		*iError = ChargingClosetError::filePathError;
//		return;
//	}
//	QByteArray qba1, qba2; QString qstr1, qstr2; QStringList qstrlist; char szTemp[256] = { 0 };
//	while (!file.atEnd())
//	{
//		qba1 = file.readLine();
//		qba1.replace(0x0B, 0x0D);
//		qstr1 = QString::fromLocal8Bit(qba1, qba1.length());//QByteArrayתQString ��ȡ����   
//		qstrlist = qstr1.split(",");
//		if (qstrlist.size() == 1 && qstrlist[0].isEmpty())
//			continue;
//		stRelay obj;
//		transitionContent1(qstrlist[0], qstr2, qba2, szTemp); //id	 
//		obj.id = atoi(szTemp); 
//
//		mapRelay.insert(MAP_RELAY::value_type(obj.id, obj));
//	}
//}

//��ȡ����""֮�������
QString COperatorFile::getContent(QString strSource)
{
	int posL = strSource.indexOf("\"");
	int posR = strSource.lastIndexOf("\"");
	QString strContent = strSource.mid(posL + 1, posR - (posL + 1));
	return strContent;
}
//ת������
void COperatorFile::transitionContent1(QString strSource, QString strTemp1, QByteArray baTemp1, char* szTemp)
{
	strTemp1 = getContent(strSource);
	baTemp1 = strTemp1.toLocal8Bit();
	memset(szTemp, 0, 256);
	memcpy(szTemp, baTemp1.data(), baTemp1.length());
	//szTemp = baTemp1.data();
}
//����log�ļ�
void COperatorFile::tryCreateLogFile()
{
	//�ж� ��û�н����log�� û�оʹ������оʹ�
	static QString  s_strPath;
	QDateTime time = QDateTime::currentDateTime();//��ȡϵͳ���ڵ�ʱ��
	QString str = g_AppPath;
	str += "/LOG/" + time.toString("logyyyy_MM_dd");//������ʾ��ʽ
	str += ".txt";	 
	if (!s_strPath.isEmpty() && s_strPath != str) //��� �����ڶ��죬�رյ�һ����ļ�
	{
		if (m_pLogfile)
		{
			m_pLogfile->close();
			delete m_pLogfile;
			m_pLogfile = nullptr;
		}}
	if (s_strPath != str)
	{
		if (m_pLogfile == nullptr)
			m_pLogfile = new QFile(str);
		s_strPath = str;
		bool flag = m_pLogfile->exists();

		if (!m_pLogfile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
		{
			qDebug() << "m_pLogfile��ʧ�ܣ�·����" << str;
			return;
		}
		else{
			m_pTextLogin = new QTextStream(m_pLogfile);
		} 
	}  
}
//дlog
void COperatorFile::writeLog(QString str)
{
	
	if (m_pTextLogin)
	{
		//if (str.indexOf("\r\n") == -1)
		//	str = str + "\r\n";
		if (str.indexOf("\r\n") != -1)//ȥ�����У�������
			str = str.remove(str.indexOf("\r\n"),2);
		/*if (str.lastIndexOf("\r\n") == -1)
			str += "\r\n"*/;
		//QDateTime time = QDateTime::currentDateTime();//��ȡϵͳ���ڵ�ʱ�� 
		//QString strTime; strTime.sprintf("ʱ�䣺");
		//strTime += time.toString("yyyy-MM-dd hh:mm:ss");//������ʾ��ʽ 
		//str = strTime + str;
		*m_pTextLogin << str;
		m_pTextLogin->flush();//����
				
	}
	else
		qDebug() << "Log�ļ�ָ��Ϊ�գ�д��ʧ��";

}
//дDebug log����
void COperatorFile::writeDebugLog(QString str)
{
	if (m_pDebugLogfile == nullptr){
		QString str = g_AppPath;
		str += "/LOG/debugLog.txt";
		m_pDebugLogfile = new QFile(str);
		if (!m_pDebugLogfile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
		{
			qDebug() << "��debug logʧ�ܣ�·����" << str;
			return;
		}
		else{
			m_pTextDebug = new QTextStream(m_pDebugLogfile);
		}
	}
	
	if (m_pDebugLogfile){
		//����
		bool bLockRet = m_Mutex.tryLock(1000);
		if (bLockRet){
			//�ж��ļ���С���������MAX_SIZE�����
			long long fileSize = m_pDebugLogfile->size();
			static int MAX_SIZE = 1024 * 1024 *5; //5mb
			
			if (fileSize > MAX_SIZE){
				m_pDebugLogfile->resize(0);
			}

			//д��
			if (m_pTextDebug){
				*m_pTextDebug <<  str;
				m_pTextDebug->flush();
			}
			m_Mutex.unlock();
		} 
	} 
}
//д���������ã����񡢵�ء������
void COperatorFile::writeAllConfig(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery, MAP_BATTERY_MODEL& mapBatteryModel, int* iError)
{
	//д�������Ϣ
	writeChargingClosetInfo(mapCloset, iError);
}

void COperatorFile::writeChargingClosetInfo(MAP_CLOSET& mapCloset, int* iError)
{
	QString strChangingClosetPath = m_strAppPath; strChangingClosetPath += "\\chargingCloset.txt";
	QFile file(strChangingClosetPath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!" << endl;
		*iError = ChargingClosetError::filePathError;
		return;
	}
	QTextStream in(&file);
	QString str;
	//��ʽid="01"
	for (MAP_CLOSET_IT it = mapCloset.begin(); it != mapCloset.end(); it++)
	{
		str += "id=\"";
		str += it->second.id;		
		str += "\"\n";
	}
	in << str;

}

//д������Ϣ
void COperatorFile::writeChargingInfo(MAP_BATTERY& mapBattery, int* iError)
{
	QString strChangingClosetPath = m_strAppPath; strChangingClosetPath += "\\battery.txt";
	QFile file(strChangingClosetPath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!" << endl;
		*iError = ChargingClosetError::filePathError;
		return;
	}
	QTextStream in(&file);
	QString str, strTemp;
	//dbid="1",id="0101",gridNO="0101",online="1",model="1002",purchasing_time="2016-06-12",elatedLoop="0"
	for (MAP_BATTERY_IT it = mapBattery.begin(); it != mapBattery.end(); it++)
	{
		str += "dbid=\"";
		str += QString::number(it->second.dbid);
		str += "\",id=\"";		 
		strTemp = QString::fromLocal8Bit(it->second.id).left(4);
		str += strTemp;//QString(it->second.id).trimmed();	
		str += "\",online=\"";
		str += QString::number(it->second.online);
		str += "\",model=\"";
		str += QString::number(it->second.modelId);  
		str += "\",relatedCharger=\"";
		strTemp = QString::number(it->second.relatedCharger);
		str += "\",nlevel=\"";
		str += QString::number(it->second.nlevel);
		str += strTemp;
		//str += "\",relatedRelay=\"";
		//strTemp = QString::number(it->second.relatedRelay);
		//str += strTemp;
		//str += "\",relatedLoop=\"";
		//str += QString::number(it->second.relatedLoop);
		str += "\"\n";
	}
	in << str;
}

//д�����ͺ���Ϣ
void COperatorFile::writeChargingModelInfo(MAP_BATTERY_MODEL& mapBatteryModel, int* iError)
{
	QString strChangingClosetPath = m_strAppPath; strChangingClosetPath += "\\batteryModel.txt";
	QFile file(strChangingClosetPath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!" << endl;
		*iError = ChargingClosetError::filePathError;
		return;
	}
	QTextStream in(&file);
	QString str, strTemp;
	//�ı���ʽ�� id="1000",drone_model="M600",    capacity="4500",charging_voltage=" 4.35",charging_current=" 3.83",storage_voltage=" 0.00",full_voltage=" 4.20",connect_type="6S0P",balance="1"
	for (MAP_BATTERY_MODEL_IT it = mapBatteryModel.begin(); it != mapBatteryModel.end(); it++)
	{
		str += "id=\"";
		str += QString::number(it->second.id);
		str += "\",drone_model=\"";  
		str += QString::fromLocal8Bit(it->second.droneModel);//QString(it->second.id).trimmed();	
		str += "\",capacity=\"";
		str += QString::number(it->second.capacity);
		
		str += "\",charging_voltage=\"";
		strTemp.sprintf("%5.2f", it->second.chargingVoltage);
		str += strTemp;
		str += "\",charging_current=\"";
		strTemp.sprintf("%5.2f", it->second.chargingCurrent);
		str += strTemp;
		str += "\",storage_voltage=\"";
		strTemp.sprintf("%5.2f", it->second.storageVoltage);
		str += strTemp;

		// connect_type="6S0P", balance="1"
		str += "\",full_voltage=\"";
		strTemp.sprintf("%5.2f", it->second.fullVoltage);
		str += strTemp;

		str += "\",connect_type=\""; 
		str += QString::fromLocal8Bit(it->second.connectType);
		
		str += "\",balance=\"";
		str += QString::number(it->second.balance );
		 
		str += "\"\n";
	}
	in << str;
}

//��ȡ������δ������Ϣ
void COperatorFile::readApplyBatteryToCharging(QVector<stApplyBatteryDontCharge>& vtApplyBattery, int* iError)
{
	QString str = m_strAppPath;
	str += "/ApplyBatteryToCharging.txt";
	 
	QFile file(str);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!" << endl;
		QMessageBox::warning(NULL, "��ʾ", str + "\n�����ؼ�¼�ļ�·����������ϵ������Ա��");
		*iError = ChargingClosetError::filePathError;
		return;
	}
	QByteArray qba1, qba2; QString qstr1, qstr2; QStringList qstrlist; char szTemp[256] = { 0 };
	while (!file.atEnd())
	{
		qba1 = file.readLine();
		qba1.replace(0x0B, 0x0D);
		qstr1 = QString::fromLocal8Bit(qba1, qba1.length());//QByteArrayתQString ��ȡ����   
		qstrlist = qstr1.split(",");
		if (qstrlist.size() == 1 || qstrlist.size() > 2|| qstrlist[0].isEmpty())
			continue; 
		stApplyBatteryDontCharge obj;
		transitionContent1(qstrlist[0], qstr2, qba2, szTemp); //id	 
		obj.nBatteryID = atoi(szTemp);
		
		transitionContent1(qstrlist[1], qstr2, qba2, szTemp); //time
		obj.strTime = QString::fromLocal8Bit(szTemp);
		vtApplyBattery.append(obj);
	}
}

//д��������δ������Ϣ
void COperatorFile::writeApplyBatteryToCharging(QVector<stApplyBatteryDontCharge>& vtApplyBattery, int* iError)
{ 
	QString str = g_AppPath;
	str += "/ApplyBatteryToCharging.txt";
	  
	if (m_pLogfileApplyBattery == nullptr)
		m_pLogfileApplyBattery = new QFile(str);

	bool flag = m_pLogfileApplyBattery->exists();
	m_pLogfileApplyBattery->open(QIODevice::ReadOnly);
	//QTextStream io(&file);
	m_pLogfileApplyBattery->close();

	m_pLogfileApplyBattery->open(QIODevice::Truncate);
	m_pLogfileApplyBattery->close();
	 
	if (m_pTextApplyBatteryLog == nullptr){
		if (!m_pLogfileApplyBattery->open(QIODevice::WriteOnly | QIODevice::Text))
		{
			*iError = ChargingClosetError::filePathError;
			qDebug() << "m_pLogfileApplyBattery��ʧ�ܣ�·����" << str;
			return;
		}
		else{
			m_pTextApplyBatteryLog = new QTextStream(m_pLogfileApplyBattery);
		}
	} 
	str = "";
	for (int i = vtApplyBattery.size()-1; i >=0 ; i--){
		str += "ApplyBatteryId=\"";
		str += QString::number(vtApplyBattery[i].nBatteryID);
		str += "\",Time=\"";
		str += vtApplyBattery[i].strTime;//QString(it->second.id).trimmed();	
		str += "\"\n";
	}
	if (str.isEmpty() == false)
		*m_pTextApplyBatteryLog << str;
	else
		*m_pTextApplyBatteryLog << " ";

	m_pTextApplyBatteryLog->flush();//���� 
	//m_pTextApplyBatteryLog->close();
}
#define DELETE_LOG_FILE(pfile) {if(pfile){pfile->close();delete pfile;pfile= nullptr;}}
void COperatorFile::onCloseFile()
{
	DELETE_LOG_FILE(m_pLogfile);
	DELETE_LOG_FILE(m_pLogfileApplyBattery);
	DELETE_LOG_FILE(m_pDebugLogfile);

}
