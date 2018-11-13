#include "dataDialog.h"
#pragma execution_character_set("utf-8")

CDataDialog::CDataDialog(QWidget* parent) :QDialog(parent)
{
	ui.setupUi((QDialog*)this);
	this->setWindowTitle("电池数据");
	this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
	this->setFixedSize(this->width(), this->height());
	
	init();

}


CDataDialog::~CDataDialog()
{
}

void CDataDialog::init()
{
	m_TextEdit_data = new QTextEdit(this);
	m_TextEdit_data->setGeometry(QRect(10, 10, 680, 480));
	m_TextEdit_data->setReadOnly(true);

	if (m_timer == NULL)
		m_timer = new QTimer();
	m_timer->setInterval(2*1000); //2秒
	QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(timer_out()));
}

void CDataDialog::timer_out()
{
	//2秒更新一次
	if (false == compareMemory()){
		refreshDataUi();
	}

	//10秒清理一次
	static QTime s_Qtime;
	static bool first = true;
	if (first){		first = false;		s_Qtime.start();	}	 
	if (s_Qtime.elapsed() >= 10 *1000 )	//大于 默认10秒
	{

	}

}

bool CDataDialog::compareMemory()
{
	return false;
}

void CDataDialog::refreshDataUi()
{

}

void CDataDialog::addData(stBatteryData& obj)
{

}
