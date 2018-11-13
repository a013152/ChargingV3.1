#pragma once
#include <QDialog>
#include <QTextEdit>
#include <QTimer>
#include <QTime>
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
public:
	CDataDialog(QWidget* parent = 0);
	~CDataDialog();
	void init();

	
	void timer_out();//��ʱ����Ӧ����

	bool compareMemory();  //�Ƚ��ڴ���AB���汾������ͬ����false

	void refreshDataUi();  //ˢ��ui

	void addData(int nBatteryId, QString& strData);
};

