#include "LoginDialog.h"


CLoginDialog::CLoginDialog(QWidget* parent) :QDialog(parent)
{
	ui.setupUi((QDialog*)this);
	this->setWindowTitle("��¼");
}


CLoginDialog::~CLoginDialog()
{
}
