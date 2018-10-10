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

	while (stopped == false){
		Sleep(100);
		if (m_bTestConnectDB) //测试连接数据库，是否通畅
		{
			m_bTestConnectDB = false;
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
		if (m_bDetectServerDB && m_pMainProcess) //连接数据库：提交电池数据，检测申请充电
		{ 
			m_bDetectServerDB = false;

			m_pMainProcess->detectServerBatteryState();//检测 服务器的电池申请情况	

			m_pMainProcess->detectSubmitBatteryState(); //提交电池信息			
		}
	} 
} 
