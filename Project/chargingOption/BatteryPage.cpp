#include "BatteryPage.h"
#include <QVBoxLayout> 
#include <QGroupBox> 
#include <QCheckBox> 
#include <QLabel> 
#include <QLineEdit> 
#include "common.h"

CBatteryPage::CBatteryPage(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery, \
	QWidget *parent) :QWidget(parent)
{

	QLabel *fileNameLabel = new QLabel(trUtf8("�ļ���:"), this);
	QLineEdit *fileNameEdit = new QLineEdit("AAA", this);//ȥ��Ŀ¼·��ǰ׺��� 
	fileNameEdit->setReadOnly(true);


	QLabel *pathLabel = new QLabel(trUtf8("·��:"), this);
	//QLabel *pathValueLabel = new QLabel(fileInfo.absoluteFilePath(), this);//������·���� 
	//pathValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);//����label����ʽ 
	QLineEdit *pathValueLabel = new QLineEdit("AAA", this);//������·���� 
	pathValueLabel->setReadOnly(true);

	QLabel *sizeLabel = new QLabel(trUtf8("��С:"), this);
	 
	QLabel *sizeValueLabel = new QLabel(trUtf8("AAA") , this);
	sizeValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	QLabel *lastReadLabel = new QLabel(trUtf8("�ϴζ�ȡʱ��:"), this);
	QLabel *lastReadValueLabel = new QLabel("AAA", this);
	lastReadValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	QLabel *lastModLabel = new QLabel(trUtf8("�ϴ��޸�ʱ��:"), this);
	QLabel *lastModValueLabel = new QLabel("AAA");
	lastModValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(fileNameLabel);
	mainLayout->addWidget(fileNameEdit);
	mainLayout->addWidget(pathLabel);
	mainLayout->addWidget(pathValueLabel);
	mainLayout->addWidget(sizeLabel);
	mainLayout->addWidget(sizeValueLabel);
	mainLayout->addWidget(lastReadLabel);
	mainLayout->addWidget(lastReadValueLabel);
	mainLayout->addWidget(lastModLabel);
	mainLayout->addWidget(lastModValueLabel);
	//mainLayout->addStretch(1); 
	setLayout(mainLayout);
}


CBatteryPage::~CBatteryPage()
{
}
