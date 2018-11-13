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
#include "sqlite3.h"
#include <QVector>

class COperatoreDBFile
{
public:
	COperatoreDBFile();
	~COperatoreDBFile();

	//打开
	bool onOpenDbFile();
	//关闭
	void onCloseDbFile();
	//添加充电记录：参数 1 电池ID, 参数2 电压 参数3 备注，参数4 错误码
	void onAddChargedRecord(int nChargerId, float fvol, QString strRemark, int &iError);
	//更新充电停止记录： 参数1 电池ID, 参数2 电压 ,参数3 开始充电的时间，参数4 错误码
	void onAddStopChargedRecord(int nChargerId, float fvol, QString strTime,  int &iError);
	//查询记录：参数1 向量 ,参数2 电池ID , 参数3 开始时间， 参数4 结束时间
	bool onQueryChargedRecord(QVector<stChargeRecord>& vtRecord, int nBatteryId = 0, QDateTime* beginTime = nullptr, QDateTime *endTime = nullptr);
	// 获取返回查询结果
	QSqlQuery getSqlQuery()const{ return m_sql_query; }

	bool isOpenSqlDB(){ return m_openSqliteFlag; }
private:
	QSqlDatabase m_database; //不能操作sqlite ，弃用
	QSqlQuery m_sql_query;

	sqlite3 * m_sqliteDB;
	bool m_openSqliteFlag = false;

};

