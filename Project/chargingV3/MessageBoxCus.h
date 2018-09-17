#pragma once

#include <QDialog>
#include <QTimer>
#include "ui_messageBoxCus.h"
#include "common.h"


class CMessageBoxCus :public QDialog
{
	Q_OBJECT
private:
	Ui::MessageBoxCus ui;

	QTimer * m_Timer;
	int m_nTimerOut = 0, m_iCount = 0;
	 
signals:
	//�˳��¼�
	void closeWindowSignal();
	
private slots :
	//��ʱ����ʱ
	void timer_out();
	void close1();
	void close2();
public:
	//��һ���� �������� �ڶ����� �Զ��رյ�ʱ��ms �������� ͼ�� 0��ʾ 1���� 2���� ���󲻿����Զ��رչ���
	CMessageBoxCus( QWidget* parent = 0, int nTimerOut = 1500 );
	~CMessageBoxCus();
	void setMessageText(QString strMsg){
		ui.lab_message->setText(strMsg);
	}
	 
	void setTypeShow(int nType);
};

