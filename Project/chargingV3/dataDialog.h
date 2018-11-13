#pragma once
#include <QDialog>
#include <QTextEdit>
#include "ui_dataDialog01.h"
#include "typedef.h"

class CDataDialog :
	public QDialog
{
	Q_OBJECT
private:
	Ui::dataDialog01 ui;

	QTextEdit *m_TextEdit_data = nullptr;   //��Ϣ�ı���

	QTimer * m_timer = NULL;  //��ʱ��

	MAP_BtData m_mapBtDataA, m_mapBtDataB; //A�汾����ˢ��UI, B�汾�������can�豸��ȡ�ĵ�ؾ�̬��Ϣ
	
	void init();

	bool compareMemory();  //�Ƚ��ڴ���AB���汾������ͬ����false

	void refreshDataUi();  //ˢ��ui
public:
	CDataDialog(QWidget* parent = 0);
	~CDataDialog();
	

	void addData(int nBatteryId, QString& strData);

	void clearMemoryB(){ m_mapBtDataB.clear(); }

	void doRefreshUI();//ˢ�º��������Ƚ��ڴ�

};

