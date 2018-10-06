#include "LoginDialog.h"
#pragma execution_character_set("utf-8")

CLoginDialog::CLoginDialog(QWidget* parent) :QDialog(parent)
{
	ui.setupUi((QDialog*)this);
	this->setWindowTitle("µÇÂ¼");
}


CLoginDialog::~CLoginDialog()
{
}
