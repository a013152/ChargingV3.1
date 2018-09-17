#pragma once

#include <QFile>
#include <QSettings>
#include <QTextCodec>

#include "..\common\common.h"
#include "..\common\errorCode.h"
class CReadIniFile
{
	CReadIniFile();
public:
	static CReadIniFile * getInstance();
	
	~CReadIniFile();

	QString readProfileInfo(QString SectionName, QString KeyName, QString IniPath, int* iError);  //∂¡»°ini≈‰÷√

	bool writeProfileInfo(QString SectionName, QString KeyName, QString strInfo, QString InitFilePath, int *iError	);//–¥ini≈‰÷√
};

