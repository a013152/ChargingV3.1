#include "MessageBoxCus.h"
#pragma execution_character_set("utf-8")
QFont ft;
QPalette pa;
CMessageBoxCus::CMessageBoxCus( QWidget* parent, int nTimerOut ) :QDialog(parent), m_nTimerOut(nTimerOut)
{ 
	ui.setupUi((QDialog*)this);
	this->setWindowTitle("��ʾ");
	QPalette pa;
	pa.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(pa);
	ui.lab_logo->setStyleSheet("QLabel{border-image: url(" +  QString::fromLocal8Bit(g_AppPath) + "/img/msg_tips.png);}");
	ui.lab_logo->setText("");
	ui.lab_message->setText("");
	ui.lab_message->setAlignment(Qt::AlignCenter);    // ���ö��뷽ʽ :��ˮƽ������С�
	
	ui.pushButton_2->setVisible(false);
	float num = ((float)m_nTimerOut ) / 1000;
	static char  szTemp[100] = { 0 }; sprintf_s(szTemp, "ȷ��(%0.1fs)", num);
 
	ui.pushButton->setText(szTemp);
	//QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close1()));


	//if (nType != 2 )  //���󲻿����Զ��رչ���
	{
		m_Timer = new QTimer();
		m_Timer->setInterval(TIMER_INTERVAL);
		QObject::connect(m_Timer, SIGNAL(timeout()), this, SLOT(timer_out()));
		//m_Timer->start();
	} 
}


CMessageBoxCus::~CMessageBoxCus()
{
}

void CMessageBoxCus::setTypeShow(int nType)
{ 
	//����ui
	if (nType == 0){
		ui.lab_logo->setStyleSheet("QLabel{border-image: url(" +  QString::fromLocal8Bit(g_AppPath) + "/img/msg_tips.png);}");
		this->setWindowTitle("��ʾ");
		ui.lab_message->setStyleSheet("QLabel{font: 14px;color:black;}");		 
	}
	else if (nType == 1){
		ui.lab_logo->setStyleSheet("QLabel{border-image: url(" +  QString::fromLocal8Bit(g_AppPath) + "/img/msg_warning.png);}");
		this->setWindowTitle("����");
		ui.lab_message->setStyleSheet("QLabel{font: 14px;color:black;}");
	}
	else if (nType == 2){
		ui.lab_logo->setStyleSheet("QLabel{border-image: url(" +  QString::fromLocal8Bit(g_AppPath) + "/img/msg_error.png);}");
		this->setWindowTitle("����");
		ui.lab_message->setStyleSheet("QLabel{font: bold 16px;color:red;}");
	} 
	m_iCount = 0;
	m_Timer->start();
	this->show();
}

void CMessageBoxCus::timer_out()
{ 
	float num = ((float)m_nTimerOut -(m_iCount*TIMER_INTERVAL)) / 1000;
	static char  szTemp[100] = { 0 }; sprintf_s(szTemp, "ȷ��(%0.1fs)", num);
	QString data = QString("ȷ��(%0s)").arg(num);
	ui.pushButton->setText(szTemp);
	if (m_nTimerOut < ((m_iCount++)*TIMER_INTERVAL)){
		m_iCount = 0;
		this->close();
		m_Timer->stop();
		emit closeWindowSignal();  
		ui.lab_logo->setText("");
	}
}

void CMessageBoxCus::close1()
{
	m_iCount = 0;
	m_Timer->stop();
	emit closeWindowSignal();
	this->close();
}
 
