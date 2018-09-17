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
	//退出事件
	void closeWindowSignal();
	
private slots :
	//定时器超时
	void timer_out();
	void close1();
	void close2();
public:
	//第一参数 阻塞父窗 第二参数 自动关闭的时间ms 第三参数 图标 0提示 1警告 2错误 错误不开启自动关闭功能
	CMessageBoxCus( QWidget* parent = 0, int nTimerOut = 1500 );
	~CMessageBoxCus();
	void setMessageText(QString strMsg){
		ui.lab_message->setText(strMsg);
	}
	 
	void setTypeShow(int nType);
};

