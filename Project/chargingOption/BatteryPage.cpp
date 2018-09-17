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

	QLabel *fileNameLabel = new QLabel(trUtf8("文件名:"), this);
	QLineEdit *fileNameEdit = new QLineEdit("AAA", this);//去掉目录路径前缀后的 
	fileNameEdit->setReadOnly(true);


	QLabel *pathLabel = new QLabel(trUtf8("路径:"), this);
	//QLabel *pathValueLabel = new QLabel(fileInfo.absoluteFilePath(), this);//完整的路径名 
	//pathValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);//设置label的样式 
	QLineEdit *pathValueLabel = new QLineEdit("AAA", this);//完整的路径名 
	pathValueLabel->setReadOnly(true);

	QLabel *sizeLabel = new QLabel(trUtf8("大小:"), this);
	 
	QLabel *sizeValueLabel = new QLabel(trUtf8("AAA") , this);
	sizeValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	QLabel *lastReadLabel = new QLabel(trUtf8("上次读取时间:"), this);
	QLabel *lastReadValueLabel = new QLabel("AAA", this);
	lastReadValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	QLabel *lastModLabel = new QLabel(trUtf8("上次修改时间:"), this);
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
