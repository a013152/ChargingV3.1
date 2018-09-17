#pragma once
#include <QDialog>
#include "ui_loginDialog.h"

class CLoginDialog :
	public QDialog
{
	Q_OBJECT
private:
	Ui::loginDialog ui;
public:
	CLoginDialog(QWidget* parent = 0);
	~CLoginDialog();
};

