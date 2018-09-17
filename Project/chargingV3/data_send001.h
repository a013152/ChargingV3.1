#ifndef DATA_SEND_H
#define DATA_SEND_H

#include "data_send_global.h"

#include <QSqlError>
#include <QSqlDatabase>
#include <qdebug.h>
#include <QSqlQuery>




/*ʹ�÷���˵��

1���ϴ�����
һ����ѯ�����ڣ�ÿ���ص���һ��msg_put�����������ص�my_id��״̬���¶ȡ���ѹ���������Ƿ�ʼ������Ϣ
����������ڵ����һ�ε�����msg_PUt�󣬵���һ��msg_put_now������ִ���ϴ�����

ʹ��ʾ����

data_send a;//���鱣����ȫ�ֱ����У�����ÿ�ζ���ʼ��
a.msg_open();

a.msg_put("1", "ȡ��", "33.6", "4.22", "1", 0);
a.msg_put("2", "���", "32.6", "4.22", "0.5", 0);
a.msg_put("3", "���", "31.6", "4.22", "3", 1);

if (a.msg_put_now()==0)
	qDenug()<<("�ϴ��ɹ�");
else
	qDenug()<<("�ϴ�ʧ��");


2����������

ʹ��msg_get_now()���Ի�ȡ��Щ��ر������ˣ���Ҫ��磩������Ĳ���Ϊ��Ҫ��ѯ��my_id���飬���Ϊһ�������ͱ�������
����1Ϊ�������ˣ�0Ϊδ�����롣ע������ʱ��my_id��Ҫ����������С�����˳������

ʹ��ʾ����
data_send a;//���鱣����ȫ�ֱ����У�����ÿ�ζ���ʼ��
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
		qDebug(QString::number(i)+"��ر�����")
	else
		qDebug(QString::number(i)+"���δ������")
	
}




*/


class DATA_SEND_EXPORT data_send
{

public:
	//�������ݿ�
	int msg_open();


	//������id��״̬���¶ȣ���ѹ���������Ƿ�ʼ���
	//�¶ȣ���ѹ����������3λ��Ч����
	int msg_put(QString my_id, QString state, QString temperature, QString voltage, QString current, bool count = 0);
	//�����ϴ����ݣ����µ����ݿ�
	int msg_put_now();

	//��ȡ��Ϣ
	//���ص������ǰ���id��С��������ģ���ע������ʱmy_id���밴����С�����˳��
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
