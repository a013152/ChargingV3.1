#include "operatoreDBFile.h"
#pragma execution_character_set("utf-8")

COperatoreDBFile::COperatoreDBFile()
{
	m_database = QSqlDatabase::addDatabase("QSQLITE");
}


COperatoreDBFile::~COperatoreDBFile()
{
}

//��
bool COperatoreDBFile::onOpenDbFile()
{
	bool bResult = false;
	if (m_database.isOpen() == false)
	{
		
		m_database.setDatabaseName(QString(g_AppPath) + "\\chargedRecord.db");
		bResult = m_database.open();
	}
	QSqlError error1 = m_database.lastError();
	return bResult;
}
//�ر�
void COperatoreDBFile::onCloseDbFile()
{
	if (m_database.isOpen())
		m_database.close();
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
//��ӳ���¼������ 1 �����ID, ����2 ״̬0 ��ʼ��� 1ֹͣ��磬����3 ������
void COperatoreDBFile::onAddChargedRecord(int nChargerId, enChargeRecordStateType nState, float fvol, int &iError)
{
	QString strSql ,strTemp;
	if (nState == StateBegin){
		strSql = "insert into chargedRecord (chargerId, beginVol, state_, begin_charge) values (";
		strSql += QString::number(nChargerId);
		strTemp.sprintf(", %4.2f, 0, \'", fvol);
		strSql += strTemp;
		strSql += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss\');");
		 
	}
	else if(nState == StateEnd){
		//�Ȳ�ѯ�Ƿ���ڼ�¼
		strSql = "select * from chargedRecord where state_=0 and chargerId=" + QString::number(nChargerId);
		strSql += " order by id desc limit 1;";
		m_sql_query.prepare(strSql);
		if (!m_sql_query.exec()) 
		{
			//��ѯ�Ƿ��м�¼
			if (m_sql_query.isNull("chargerId")){
				//�������				
				QString chargerId, strbeginTime;
				chargerId = m_sql_query.value("chargerId").toString();
				strbeginTime = m_sql_query.value("begin_charge").toString();
				// 
				strSql = "UPDATE chargedRecord ( endVol, state_, end_charge, charge_interval) values (";
				strTemp.sprintf(" %4.2f, ", fvol);
				strSql += strTemp;  //������ѹ
				strSql += " 1, \' ";  //����״̬
				strSql += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss\')");  //����ʱ��
				strSql += ",";
				QDateTime beginTime = QDateTime::fromString(strbeginTime,"yyyy-MM-dd hh:mm:ss");
				//����ʱ����
			}
			else{
				//�������

			}
			
			
		}
	}
	if (strSql.isEmpty() != false){
		m_sql_query.prepare(strSql);
		if (!m_sql_query.exec()) //������߸����Ƿ�ɹ�  
		{
			qDebug() << QObject::tr("select failed");
			qDebug() << m_sql_query.lastError();
		}
	}
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