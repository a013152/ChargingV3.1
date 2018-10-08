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
		//m_database.setDatabaseName(QString(g_AppPath) + "\\chargedRecord.db");
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

	QSqlError error1 = m_database.lastError();
	return bResult;
}
//�ر�
void COperatoreDBFile::onCloseDbFile()
{
	if (m_database.isOpen())
		m_database.close();

	sqlite3_close(m_sqliteDB);
}

//������
bool COperatoreDBFile::onCreateChargedRecordTable()
{ 
	bool bResult = false;
	if (m_database.open())
	{
		qDebug() << "Database Opened";
		QSqlQuery sql_query;
		QString create_sql = "create table chargedRecord (id int primary key, chargerId int, beginVol numeric(4, 2),\
			endVol numeric(4, 2), state_ int, begin_charge datetime, end_charge datetime, charge_interval datetime,remark varchar(256))"; //�������ݱ�  
		//QString insert_sql = "insert into chargedRecord values(?,?,?)";    //��������   
	//	QString select_all_sql = "select * from member";

		sql_query.prepare(create_sql); //������ 
		if (!sql_query.exec()) //�鿴�������Ƿ�ɹ�  
		{
			qDebug() << QObject::tr("Table Create failed");
			qDebug() << sql_query.lastError();
		}
		else
			bResult = true;
	}
	return bResult;
}
//��ӳ���¼������ 1 ���ID, ����2 ��ѹ ����3 ��ע������4 ������
void COperatoreDBFile::onAddChargedRecord(int nChargerId, float fvol,QString strRemark, int &iError)
{
	QSqlQuery tmpQuery(m_database);
	m_sql_query = tmpQuery;

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
static int checkChargerRecordResult(void *NotUsed, int argc, char **argv, char **azColName)
{
	for (int i = 0; i < argc; i++)
	{
		tempChargerId = (argv[i] ? atoi(argv[i]) : 0);
	}
	return 0;
} 
//���³��ֹͣ��¼�� ����1 ���ID, ����2 ��ѹ ,����3 ��ʼ ʱ�䣬����4 ������
void  COperatoreDBFile::onAddStopChargedRecord(int nChargerId, float fvol, QString strTime, int &iError)
{
	//QSqlQuery tmpQuery(m_database);
	//m_sql_query = tmpQuery;

	QString strSql, strTemp; char szSQL[512] = { 0 };	char* cErrMsg;
	strSql = "select id from chargedRecord where state_=0 and chargerId=" + QString::number(nChargerId);
	strSql += " order by id desc limit 1;";
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
			strTemp.sprintf(" %4.2f, ", fvol);
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

//��ӳ���¼������ 1 �����ID, ����2 ״̬0 ��ʼ��� 1ֹͣ��磬����3 ������
void onAddChargedRecord(int nChargerId, enChargeRecordStateType nState, float fvol, int &iError)
{
	//QString strSql ,strTemp;
	//if (nState == StateBegin){
	//	strSql = "insert into chargedRecord (chargerId, beginVol, state_, begin_charge) values (";
	//	strSql += QString::number(nChargerId);
	//	strTemp.sprintf(", %4.2f, 0, \'", fvol);
	//	strSql += strTemp;
	//	strSql += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss\');");
	//	 
	//}
	//else if(nState == StateEnd){
	//	//�Ȳ�ѯ�Ƿ���ڼ�¼
	//	strSql = "select * from chargedRecord where state_=0 and chargerId=" + QString::number(nChargerId);
	//	strSql += " order by id desc limit 1;";
	//	m_sql_query.prepare(strSql);
	//	if (!m_sql_query.exec()) 
	//	{
	//		//��ѯ�Ƿ��м�¼
	//		if (m_sql_query.isNull("chargerId")){
	//			//�������				
	//			QString chargerId, strbeginTime;
	//			chargerId = m_sql_query.value("chargerId").toString();
	//			strbeginTime = m_sql_query.value("begin_charge").toString();
	//			// 
	//			strSql = "UPDATE chargedRecord ( endVol, state_, end_charge, charge_interval) values (";
	//			strTemp.sprintf(" %4.2f, ", fvol);
	//			strSql += strTemp;  //������ѹ
	//			strSql += " 1, \' ";  //����״̬
	//			strSql += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss\')");  //����ʱ��
	//			strSql += ",";
	//			QDateTime beginTime = QDateTime::fromString(strbeginTime,"yyyy-MM-dd hh:mm:ss");
	//			//����ʱ����
	//		}
	//		else{
	//			//�������
	//		}
	//		
	//		
	//	}
	//}
	//if (strSql.isEmpty() != false){
	//	m_sql_query.prepare(strSql);
	//	if (!m_sql_query.exec()) //������߸����Ƿ�ɹ�  
	//	{
	//		qDebug() << QObject::tr("select failed");
	//		qDebug() << m_sql_query.lastError();
	//	}
	//}
	return ;
}
//��ѯ��¼������1 �����ID , ����2��ʼʱ�䣬 ����3����ʱ��
bool COperatoreDBFile::onQueryChargedRecord(int nChargerId , QDateTime* beginTime, QDateTime *endTime)
{
	bool bResult = false;
	if (m_database.open())
	{
		bool bWhereFlag = false;
		QString strSql = "select * from chargedRecord ";
		if (nChargerId != 0){
			if (bWhereFlag == false){
				strSql += "where ";
				bWhereFlag = true;
			}
			strSql += " chargerId=" + QString::number(nChargerId);
		}
		if (beginTime != nullptr){
			if (bWhereFlag == false){
				strSql += "where ";
				bWhereFlag = true;
			}
			strSql += " begin_charge>=" + beginTime->toString("yyyy-MM-dd hh:mm:ss");
		}
		if (endTime != nullptr){
			if (bWhereFlag == false){
				strSql += "where ";
				bWhereFlag = true;
			}
			strSql += " end_charge<=" +endTime->toString("yyyy-MM-dd hh:mm:ss");
		}
		m_sql_query.prepare(strSql); 
		if (!m_sql_query.exec()) //�鿴�Ƿ�ɹ�  
		{
			qDebug() << QObject::tr("select failed");
			qDebug() << m_sql_query.lastError();
		}
		else
			bResult = true;
	}
	return bResult;
}