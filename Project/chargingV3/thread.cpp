#include "charging.h"
#pragma execution_character_set("utf-8")



CConnectDBThread::CConnectDBThread()
{
	stopped = false;
}

void CConnectDBThread::run()
{
	/*while (!stopped)
	{
		Sleep(1);
	}*/
    stopped = false;

	QString strCom;
	QSqlDatabase d = QSqlDatabase::addDatabase("QMYSQL", "db2");
	d.setHostName(DBHOSTNAME);
	d.setDatabaseName("nfpower");
	d.setPort(3306);
	d.setUserName("nfnf");
	d.setPassword("Server2008");

	if (d.open())
	{
		emit connectDBResult(true); 
	}
	else
	{
		emit connectDBResult(false); 
	}
	d.close();
}

void CConnectDBThread::stop()
{
	stopped = true;
}


