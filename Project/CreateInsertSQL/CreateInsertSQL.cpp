// CreateInsertSQL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <stdlib.h>

#define BASEDBID 723  //数据库id起址
#define BASELOCALID 151  //本地的电池id起址  1号柜=100， 2号柜=200
#define ONELOOP 15		//单柜充电器数量
#define BASETIME 1		//
int _tmain(int argc, _TCHAR* argv[])
{
	std::string strSQL;
	char szSQL[256] = { 0 };
	for (int iCloset = 0; iCloset < BASETIME; iCloset++)
	{
		for (int i = 1; i <= ONELOOP; i++){
			//('my_id','local_id','unit_num','battery_type','state','owner','temperature','voltage','current','message','count')
			sprintf_s(szSQL, "INSERT INTO battery VALUES(%d, %d, 1,'%s', '未放置电池',0, 0, 0, 0, '',0);\n",
				i + BASEDBID + (iCloset)* ONELOOP, i + BASELOCALID + (iCloset)* 15,  "PHANTOM4" );

			strSQL += szSQL;

		}
	}
	
	printf("%s", strSQL.c_str());
	strSQL = "";
	for (int i = 0; i < ONELOOP; i++){
		//('my_id','local_id','unit_num','battery_type','state','owner','temperature','voltage','current','message','count')
		sprintf_s(szSQL, "INSERT INTO relation(staff_num,battery_num, permission) VALUES(%d, %d, 1);\n",
			32,					 //员工id
			i + BASEDBID );  //电池id

		strSQL += szSQL;

	}
	printf("%s", strSQL.c_str());
	system("pause");
	return 0;
}

