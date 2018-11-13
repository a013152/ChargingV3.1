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

	//��
	bool onOpenDbFile();
	//�ر�
	void onCloseDbFile();
	//��ӳ���¼������ 1 ���ID, ����2 ��ѹ ����3 ��ע������4 ������
	void onAddChargedRecord(int nChargerId, float fvol, QString strRemark, int &iError);
	//���³��ֹͣ��¼�� ����1 ���ID, ����2 ��ѹ ,����3 ��ʼ����ʱ�䣬����4 ������
	void onAddStopChargedRecord(int nChargerId, float fvol, QString strTime,  int &iError);
	//��ѯ��¼������1 ���� ,����2 ���ID , ����3 ��ʼʱ�䣬 ����4 ����ʱ��
	bool onQueryChargedRecord(QVector<stChargeRecord>& vtRecord, int nBatteryId = 0, QDateTime* beginTime = nullptr, QDateTime *endTime = nullptr);
	// ��ȡ���ز�ѯ���
	QSqlQuery getSqlQuery()const{ return m_sql_query; }

	bool isOpenSqlDB(){ return m_openSqliteFlag; }
private:
	QSqlDatabase m_database; //���ܲ���sqlite ������
	QSqlQuery m_sql_query;

	sqlite3 * m_sqliteDB;
	bool m_openSqliteFlag = false;

};

