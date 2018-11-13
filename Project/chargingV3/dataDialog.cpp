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
	m_TextEdit_data->setGeometry(QRect(10, 10, 740, 500));
	m_TextEdit_data->setReadOnly(true);

}

void CDataDialog::doRefreshUI()
{
	//����һ��
	if (compareMemory()){
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
	QString str;
	int icount = 0;
	//����ui
	for (auto itA : m_mapBtDataA){
		str = (++icount) % 2 ? "\n":"";str += "<p><font size=\"4\" color=\"black\">" + itA.second.strData + "</font></p>";
		m_TextEdit_data->append(str);
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
