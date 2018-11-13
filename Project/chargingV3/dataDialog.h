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

	QTextEdit *m_TextEdit_data = nullptr;   //信息文本框

	QTimer * m_timer = NULL;  //定时器

	MAP_BtData m_mapBtDataA, m_mapBtDataB; //A版本负责刷新UI, B版本负责接收can设备读取的电池静态信息
	
	void init();

	bool compareMemory();  //比较内存中AB两版本，不相同返回false

	void refreshDataUi();  //刷新ui
public:
	CDataDialog(QWidget* parent = 0);
	~CDataDialog();
	

	void addData(int nBatteryId, QString& strData);

	void clearMemoryB(){ m_mapBtDataB.clear(); }

	void doRefreshUI();//刷新函数包含比较内存

};

