#include "ReadIniFile.h"
#include "ReadIniFile.h"

static CReadIniFile* s_this = nullptr;
CReadIniFile::CReadIniFile()
{
}


CReadIniFile::~CReadIniFile()
{
}
CReadIniFile * CReadIniFile::getInstance()
{
	if (s_this)
		s_this = new CReadIniFile();
	return s_this;
}
QString CReadIniFile::readProfileInfo(QString SectionName, QString KeyName, QString IniPath, int* iError)  //��ȡini����
{
	QFile mFile(IniPath);
	if (!mFile.exists())
	{
		*iError = ChargingClosetError::filePathError;
		return "";
	} 
	static QSettings *configIni = nullptr;
	static QString sIniPath;
	if (sIniPath != IniPath)
	{
		if (configIni)
		{
			delete configIni;
			configIni = nullptr;
		}
		sIniPath = IniPath;

		//Qt��ʹ��QSettings���дini�ļ�  
		//QSettings���캯���ĵ�һ��������ini�ļ���·��,�ڶ���������ʾ���ini�ļ�,��������������ȱʡ  
		configIni = new QSettings(IniPath, QSettings::IniFormat);

		if (configIni->status() != QSettings::Status::NoError)
		{
			*iError = configIni->status();

		}
	}

	if (configIni->status() == QSettings::Status::NoError)
	{
		configIni->setIniCodec(QTextCodec::codecForName("GB2312")); //�ڴ�������ã����ɶ�дini�ļ��е����� 

		QString key; key.sprintf("/%s/%s", SectionName.toLatin1().data(), KeyName.toLatin1().data());

		return configIni->value(key).toString();
	}
	return "";


}

bool CReadIniFile::writeProfileInfo(QString SectionName, QString KeyName, QString strInfo, QString InitFilePath, int *iError)
{
	QFile mFile(InitFilePath);
	if (!mFile.exists())
	{
		*iError = ChargingClosetError::filePathError;
		return false;
	}
	static QSettings *configIni = nullptr;
	static QString sIniPath;
	if (sIniPath != InitFilePath)
	{
		if (configIni)
		{
			delete configIni;
			configIni = nullptr;
		}
		sIniPath = InitFilePath;

		//Qt��ʹ��QSettings���дini�ļ�  
		//QSettings���캯���ĵ�һ��������ini�ļ���·��,�ڶ���������ʾ���ini�ļ�,��������������ȱʡ  
		configIni = new QSettings(InitFilePath, QSettings::IniFormat);

		if (configIni->status() != QSettings::Status::NoError)
		{
			*iError = configIni->status(); 
		}
	}

	if (configIni->status() == QSettings::Status::NoError)
	{
		configIni->setIniCodec(QTextCodec::codecForName("GB2312")); //�ڴ�������ã����ɶ�дini�ļ��е����� 

		QString key; key.sprintf("/%s/%s", SectionName.toLatin1().data(), KeyName.toLatin1().data());
		configIni->setValue(key, strInfo);
		//configIni->setValue(key).toString();
		return true;
	}
	return false;
	
}
