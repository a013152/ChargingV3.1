#pragma once
#include <QDialog>
#include <QTextEdit>
#include "ui_dataDialog01.h"

class CDataDialog :
	public QDialog
{
	Q_OBJECT
private:
	Ui::dataDialog01 ui;

	QTextEdit *m_TextEdit_data = nullptr;   //��Ϣ�ı���

public:
	CDataDialog(QWidget* parent = 0);
	~CDataDialog();
	void init();
};

