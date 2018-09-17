#ifndef DATA_SEND_H
#define DATA_SEND_H

#include "data_send_global.h"

#include <QSqlError>
#include <QSqlDatabase>
#include <qdebug.h>
#include <QSqlQuery>




/*使用方法说明

1、上传数据
一个轮询周期内，每块电池调用一次msg_put函数，存入电池的my_id、状态、温度、电压、电流、是否开始充电等信息
在这个周期内的最后一次调用完msg_PUt后，调用一次msg_put_now，最终执行上传动作

使用示范：

data_send a;//建议保存在全局变量中，无需每次都初始化
a.msg_open();

a.msg_put("1", "取用", "33.6", "4.22", "1", 0);
a.msg_put("2", "充电", "32.6", "4.22", "0.5", 0);
a.msg_put("3", "充电", "31.6", "4.22", "3", 1);

if (a.msg_put_now()==0)
	qDenug()<<("上传成功");
else
	qDenug()<<("上传失败");


2、接收数据

使用msg_get_now()可以获取哪些电池被申请了（需要充电），输入的参数为需要查询的my_id数组，输出为一个布尔型变量数组
其中1为被申请了，0为未被申请。注意输入时，my_id需要按照数字由小到大的顺序排序。

使用示范：
data_send a;//建议保存在全局变量中，无需每次都初始化
a.msg_open();

QVector<QString> c;
c.append("1");
c.append("2");
c.append("3");
c.append("4");
c.append("5");
QVector<bool> b = a.msg_get_now(c);

for (int i = 0; i < b.size(); i++)
{
	if(b[i])
		qDebug(QString::number(i)+"电池被申请")
	else
		qDebug(QString::number(i)+"电池未被申请")
	
}




*/


class DATA_SEND_EXPORT data_send
{

public:
	//连接数据库
	int msg_open();


	//参数：id，状态，温度，电压，电流，是否开始充电
	//温度，电压，电流保留3位有效数字
	int msg_put(QString my_id, QString state, QString temperature, QString voltage, QString current, bool count = 0);
	//立即上传数据，更新到数据库
	int msg_put_now();

	//获取信息
	//返回的数据是按照id由小到大排序的，请注意输入时my_id必须按照由小到大的顺序
	QVector<bool> msg_get_now(QVector<QString>my_id);



private:

	QStringList drivers;
	QSqlDatabase db;
	QSqlError err;
public:
	data_send();

private:
	QString getting_sql;


public:
	~data_send();

private:

};

#endif // DATA_SEND_H
