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

	//��
	bool onOpenDbFile();
	//�ر�
	void onCloseDbFile();
	//������
	bool onCreateChargedRecordTable();
	//��ӳ���¼������ 1 �����ID, ����2 ״̬0 ��ʼ��� 1ֹͣ��磬����3 ��ѹ������4 ������
	void onAddChargedRecord(int nChargerId, enChargeRecordStateType nState, float fvol, int &iError);
	//��ѯ��¼������1 �����ID , ����2��ʼʱ�䣬 ����3����ʱ��
	bool onQueryChargedRecord(int nChargerId = 0, QDateTime* beginTime = nullptr, QDateTime *endTime = nullptr);
	// ��ȡ���ز�ѯ���
	QSqlQuery getSqlQuery()const{ return m_sql_query; }
private:
	QSqlDatabase m_database;
	QSqlQuery m_sql_query;

};

