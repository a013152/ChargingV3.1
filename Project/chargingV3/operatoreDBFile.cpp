#include "operatoreDBFile.h"
#include "CommonFunction.h"
#pragma execution_character_set("utf-8")

COperatoreDBFile::COperatoreDBFile()
{
	m_sqliteDB = NULL;
	m_database = QSqlDatabase::addDatabase("QSQLITE");
}


COperatoreDBFile::~COperatoreDBFile()
{
}

//��
bool COperatoreDBFile::onOpenDbFile()
{
	qDebug() << m_database.drivers();
	bool bResult = true;
	if (m_database.isOpen() == false)
	{		
		//m_database.setDatabaseName( QString::fromLocal8Bit(g_AppPath) + "\\chargedRecord.db");
		//bResult = m_database.open();
		
	}
	qDebug() << m_database.lastError();

	//sqlite3 
	char dbPath[256] = { 0 }; sprintf_s(dbPath, 256, "%s\\chargedRecord.db", g_AppPath);
	//תUtf-8
	char dbPathUtf8[256] = { 0 };
	strcpy_s(dbPathUtf8, 256,COM_F::UnicodeToUtf_8(COM_F::MBytesToWString(dbPath).c_str()).c_str());
	int nRes = sqlite3_open(dbPathUtf8, &m_sqliteDB);
	if (nRes != SQLITE_OK)
	{
		qDebug() << "Open database fail: " << sqlite3_errmsg(m_sqliteDB);
		bResult = false;
	}
	m_openSqliteFlag = bResult; 
	return bResult;
}
//�ر�
void COperatoreDBFile::onCloseDbFile()
{
	if (m_database.isOpen())
		m_database.close();

	sqlite3_close(m_sqliteDB);
}


//��ӳ���¼������ 1 ���ID, ����2 ��ѹ ����3 ��ע������4 ������
void COperatoreDBFile::onAddChargedRecord(int nChargerId, float fvol,QString strRemark, int &iError)
{
	if (m_openSqliteFlag == false)
	{
		iError = -1;
		return;
	}
	//QSqlQuery tmpQuery(m_database);
	//m_sql_query = tmpQuery;

	QString strSql, strTemp;
	strSql = "insert into chargedRecord (chargerId, beginVol, state_, begin_charge, remark) values (";
	strSql += QString::number(nChargerId);
	strTemp.sprintf(", %4.2f, 0, \'", fvol);
	strSql += strTemp;
	strSql += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	strSql += "\',\'";
	strSql += strRemark;
	strSql += "\');"; 
	//if (!m_sql_query.exec()){
	//	;//�������
	//	qDebug() << QObject::tr("insert failed");
	//	qDebug() << m_sql_query.lastError();
	//} 
	QByteArray ba = strSql.toLocal8Bit();
	char szSQL[512] = { 0 }; memcpy(szSQL, ba.data(),ba.size());
	char szSQLUtf8[256] = { 0 };
	strcpy_s(szSQLUtf8, 256, COM_F::UnicodeToUtf_8(COM_F::MBytesToWString(szSQL).c_str()).c_str());
	char* cErrMsg;
	int nRes = sqlite3_exec(m_sqliteDB, szSQLUtf8, 0, 0, &cErrMsg);
	if (nRes != SQLITE_OK)
	{
		qDebug() << "inset fail: " << cErrMsg ;		
	}
}

int tempChargerId = 0;
char szBeginVol[10] = {0};
static int checkChargerRecordResult(void *NotUsed, int argc, char **argv, char **azColName)
{
	tempChargerId = (argv[0] ? atoi(argv[0]) : 0);
	strcpy_s(szBeginVol, 10, (argv[1] ? argv[1] : ""));
	
	return 0;
} 
//���³��ֹͣ��¼�� ����1 ���ID, ����2 ��ѹ ,����3 ��ʼ ʱ�䣬����4 ������
void  COperatoreDBFile::onAddStopChargedRecord(int nChargerId, float fvol, QString strTime, int &iError)
{
	if (m_openSqliteFlag == false)
	{
		iError = -1;
		return;
	}

	//QSqlQuery tmpQuery(m_database);
	//m_sql_query = tmpQuery;

	QString strSql, strTemp; char szSQL[512] = { 0 };	char* cErrMsg;
	strSql = "SELECT id,beginVol FROM chargedRecord WHERE state_=0 AND chargerId=" + QString::number(nChargerId);
	strSql += " ORDER BY id DESC LIMIT 1;";
	QByteArray ba = strSql.toLocal8Bit();
	memcpy(szSQL, ba.data(), ba.size());	
	int nRes = sqlite3_exec(m_sqliteDB, szSQL, checkChargerRecordResult, 0, &cErrMsg);
	if (nRes != SQLITE_OK)
	{
		qDebug() << "select fail: " << cErrMsg;
	}
	//m_sql_query.prepare(strSql);
	//if (m_sql_query.exec())
	{
		//��ѯ�Ƿ��м�¼
		//if (m_sql_query.isNull("chargerId"))
		{
			//�������		
			strSql = "UPDATE chargedRecord  SET endVol=";
			if (fvol != 0)
				strTemp.sprintf(" %4.2f, ", fvol);
			else
				strTemp += QString::fromLocal8Bit(szBeginVol);
			strSql += strTemp;  //������ѹ
			strSql += " state_=1, end_charge=\'";  //����״̬
			strSql += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");  //����ʱ��
			strSql += "\', charge_interval=\'";
			//����ʱ����
			QDateTime beginTime = QDateTime::fromString(strTime, "yyyy-MM-dd hh:mm:ss");
			QDateTime endTime = QDateTime::currentDateTime();			
			QDate date_2 = endTime.date(); QDate date_1 = beginTime.date();
			QTime time_2 = endTime.time(); QTime time_1 = beginTime.time();			 
			unsigned int temp1 = time_1.hour() * 3600 + time_1.minute() * 60 + time_1.second();
			unsigned int temp2 = (time_2.hour() + (date_2.day() - date_1.day()) * 24) * 3600 + time_2.minute() * 60 + time_2.second();
			unsigned int temp3 = temp2 - temp1;
			QTime time_3; time_3.setHMS(temp3/3600, (temp3/60)%60, temp3%60);
			strSql += time_3.toString("hh:mm:ss");  //ʱ����
			strSql += "\' WHERE chargerId=";
			strSql += QString::number(nChargerId) + " AND id=" + QString::number(tempChargerId) + ";";
			
			//if (!m_sql_query.exec())
			//{
			//	;//���´���
			//	qDebug() << QObject::tr("update failed");
			//	qDebug() << m_sql_query.lastError();
			//}
			ba = strSql.toLocal8Bit();
			memcpy(szSQL, ba.data(), ba.size());
			char szSQLUtf8[256] = { 0 };
			strcpy_s(szSQLUtf8, 256, COM_F::UnicodeToUtf_8(COM_F::MBytesToWString(szSQL).c_str()).c_str());
		
			int nRes = sqlite3_exec(m_sqliteDB, szSQLUtf8, 0, 0, &cErrMsg);
			if (nRes != SQLITE_OK)
			{
				qDebug() << "update fail: " << cErrMsg;
			}
		}
	}
}
QVector<stChargeRecord> vtRecordTemp;
static int checkChargerRecordResultTotal(void *NotUsed, int argc, char **argv, char **azColName)
{
	stChargeRecord obj;
	obj.batteryId = (atoi(argv[1] ? argv[1] : 0));
	obj.beginVol = QString(argv[2] ? argv[2] : "").toFloat();
	obj.endVol = QString(argv[3] ? argv[3] : "").toFloat();
	obj.beginTime = QString((argv[5] ? argv[5] : ""));
	obj.endTime = QString((argv[6] ? argv[6] : ""));
	obj.pendingTime = QString(argv[7] ? argv[7] : "");
	obj.strRemrk = QString(argv[8] ? argv[8] : "");
	vtRecordTemp.append(obj);
	return 0;
}
//��ѯ��¼������1 �����ID , ����2��ʼʱ�䣬 ����3����ʱ��
bool COperatoreDBFile::onQueryChargedRecord(QVector<stChargeRecord>& vtRecord, int nBatteryId, QDateTime* beginTime, QDateTime *endTime)
{
	if (m_openSqliteFlag == false)
		return false;
	vtRecordTemp.clear();
	bool bResult = false;
	 
	QString strSql = "SELECT * FROM chargedRecord WHERE state_ = 1"; 
	if (nBatteryId != 0){
		strSql += " AND  chargerId=" + QString::number(nBatteryId);
	}
	if (beginTime != nullptr){ 
		strSql += " AND  begin_charge >=\'" + beginTime->toString("yyyy-MM-dd hh:mm:ss") +"\'";
	}
	if (endTime != nullptr){		
		QDate date = endTime->date(); date.setDate(date.year(), date.month(), date.day() + 1); endTime->setDate(date);
		strSql += " AND  end_charge <=\'" + endTime->toString("yyyy-MM-dd hh:mm:ss") + "\'";
	}

	char szSQL[512] = { 0 };	char* cErrMsg;
	//strSql = "select id from chargedRecord where state_=0 and chargerId=" + QString::number(nChargerId);
	strSql += " ORDER BY id DESC ";   //����id��ȡ
	if (nBatteryId == 0 && beginTime == nullptr&& endTime == nullptr)
		strSql += "LIMIT 20";		//���û��ɸѡ������ֻ��ȡǰ500��
	strSql += ";";
	QByteArray ba = strSql.toLocal8Bit();
	memcpy(szSQL, ba.data(), ba.size());
	int nRes = sqlite3_exec(m_sqliteDB, szSQL, checkChargerRecordResultTotal, 0, &cErrMsg);
	if (nRes != SQLITE_OK)
	{
		qDebug() << "select fail: " << cErrMsg;
	}else
		bResult = true;
	if (bResult){
		vtRecordTemp.swap(vtRecord);
	}

	return bResult;
}