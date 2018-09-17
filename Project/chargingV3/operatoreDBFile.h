#pragma once

#include"typedef.h"
#include "common.h"
#include "errorCode.h"
#include <QString>
#include <QtSql\QSqlDatabase>
#include <QtSql\QSqlError>  
#include <QtSql\QSqlQuery> 
#include <QDateTime>
#include <qDebug>

class COperatoreDBFile
{
public:
	COperatoreDBFile();
	~COperatoreDBFile();

	//打开
	bool onOpenDbFile();
	//关闭
	void onCloseDbFile();
	//创建表
	bool onCreateChargedRecordTable();
	//添加充电记录：参数 1 充电器ID, 参数2 状态0 开始充电 1停止充电，参数3 电压，参数4 错误码
	void onAddChargedRecord(int nChargerId, enChargeRecordStateType nState, float fvol, int &iError);
	//查询记录：参数1 充电器ID , 参数2开始时间， 参数3结束时间
	bool onQueryChargedRecord(int nChargerId = 0, QDateTime* beginTime = nullptr, QDateTime *endTime = nullptr);
	// 获取返回查询结果
	QSqlQuery getSqlQuery()const{ return m_sql_query; }
private:
	QSqlDatabase m_database;
	QSqlQuery m_sql_query;

};

