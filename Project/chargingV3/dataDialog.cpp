#include "dataDialog.h"
#pragma execution_character_set("utf-8")

CDataDialog::CDataDialog(QWidget* parent) :QDialog(parent)
{
	ui.setupUi((QDialog*)this);
	this->setWindowTitle("�������");
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
	m_timer->setInterval(3*1000); //3��
	QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(timer_out()));
}

void CDataDialog::timer_out()
{
	//����һ��
	if (false == compareMemory()){
		refreshDataUi();
	}
}

bool CDataDialog::compareMemory()
{
	bool result_ = false;  //��ͬfalse�� ��ͬtrue
	do 
	{
		//��B���ݱȽ�A����
		if (m_mapBtDataA.size() != m_mapBtDataB.size()) //���������ͬ
		{
			result_ = true;
			break;
		}		
		MAP_BtData_IT itA;
		for (auto itB: m_mapBtDataB)
		{
			itA = m_mapBtDataA.find(itB.second.nBatteryId);  //���û�ҵ����id
			if (itA == m_mapBtDataA.end()){
				result_ = true;
				break;
			}
			if (itB.second.strData != itA->second.strData){ //��������Ϣ�иı�
				result_ = true;
				break;
			}
		}
		if (result_)
			break;

		 
	} while (0);
	//����ͬ�����
	if (result_)
	{
		m_mapBtDataA.swap(m_mapBtDataB);
		m_mapBtDataB.clear();
	}
	return result_;
}

void CDataDialog::refreshDataUi()
{
	m_TextEdit_data->clear();
	//����ui
	for (auto itA : m_mapBtDataA){
		m_TextEdit_data->append(itA.second.strData);
	}

}

void CDataDialog::addData(int nBatteryId, QString& strData)
{
	MAP_BtData_IT itB = m_mapBtDataB.find(nBatteryId);
	if (itB != m_mapBtDataB.end()){
		itB->second.strData = strData;
	}
	else{
		//���뵽B����
		stBatteryData obj;
		obj.nBatteryId = nBatteryId; 
		obj.strData = strData;
		m_mapBtDataB.insert(MAP_BtData::value_type(nBatteryId, obj));
	}
}
