#include "operatoreDBFile.h"
#pragma execution_character_set("utf-8")

COperatoreDBFile::COperatoreDBFile()
{
	m_database = QSqlDatabase::addDatabase("QSQLITE");
}


COperatoreDBFile::~COperatoreDBFile()
{
}

//打开
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
//关闭
void COperatoreDBFile::onCloseDbFile()
{
	if (m_database.isOpen())
		m_database.close();
}

//创建表
bool COperatoreDBFile::onCreateChargedRecordTable()
{ 
	bool bResult = false;
	if (m_database.open())
	{
		qDebug() << "Database Opened";
		QSqlQuery sql_query;
		QString create_sql = "create table chargedRecord (id int primary key, chargerId int, beginVol numeric(4, 2),\
			endVol numeric(4, 2), state_ int, begin_charge datetime, end_charge datetime, charge_interval datetime,remark varchar(256))"; //创建数据表  
		//QString insert_sql = "insert into chargedRecord values(?,?,?)";    //插入数据   
	//	QString select_all_sql = "select * from member";

		sql_query.prepare(create_sql); //创建表 
		if (!sql_query.exec()) //查看创建表是否成功  
		{
			qDebug() << QObject::tr("Table Create failed");
			qDebug() << sql_query.lastError();
		}
		else
			bResult = true;
	}
	return bResult;
}
//添加充电记录：参数 1 充电器ID, 参数2 状态0 开始充电 1停止充电，参数3 错误码
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
		//先查询是否存在记录
		strSql = "select * from chargedRecord where state_=0 and chargerId=" + QString::number(nChargerId);
		strSql += " order by id desc limit 1;";
		m_sql_query.prepare(strSql);
		if (!m_sql_query.exec()) 
		{
			//查询是否有记录
			if (m_sql_query.isNull("chargerId")){
				//有则更新				
				QString chargerId, strbeginTime;
				chargerId = m_sql_query.value("chargerId").toString();
				strbeginTime = m_sql_query.value("begin_charge").toString();
				// 
				strSql = "UPDATE chargedRecord ( endVol, state_, end_charge, charge_interval) values (";
				strTemp.sprintf(" %4.2f, ", fvol);
				strSql += strTemp;  //结束电压
				strSql += " 1, \' ";  //结束状态
				strSql += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss\')");  //结束时间
				strSql += ",";
				QDateTime beginTime = QDateTime::fromString(strbeginTime,"yyyy-MM-dd hh:mm:ss");
				//计算时间间隔
			}
			else{
				//无则插入

			}
			
			
		}
	}
	if (strSql.isEmpty() != false){
		m_sql_query.prepare(strSql);
		if (!m_sql_query.exec()) //插入或者更新是否成功  
		{
			qDebug() << QObject::tr("select failed");
			qDebug() << m_sql_query.lastError();
		}
	}
	return ;
}
//查询记录：参数1 充电器ID , 参数2开始时间， 参数3结束时间
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
		if (!m_sql_query.exec()) //查看是否成功  
		{
			qDebug() << QObject::tr("select failed");
			qDebug() << m_sql_query.lastError();
		}
		else
			bResult = true;
	}
	return bResult;
}