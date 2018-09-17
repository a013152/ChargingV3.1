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
QString CReadIniFile::readProfileInfo(QString SectionName, QString KeyName, QString IniPath, int* iError)  //读取ini配置
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

		//Qt中使用QSettings类读写ini文件  
		//QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省  
		configIni = new QSettings(IniPath, QSettings::IniFormat);

		if (configIni->status() != QSettings::Status::NoError)
		{
			*iError = configIni->status();

		}
	}

	if (configIni->status() == QSettings::Status::NoError)
	{
		configIni->setIniCodec(QTextCodec::codecForName("GB2312")); //在此添加设置，即可读写ini文件中的中文 

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

		//Qt中使用QSettings类读写ini文件  
		//QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省  
		configIni = new QSettings(InitFilePath, QSettings::IniFormat);

		if (configIni->status() != QSettings::Status::NoError)
		{
			*iError = configIni->status(); 
		}
	}

	if (configIni->status() == QSettings::Status::NoError)
	{
		configIni->setIniCodec(QTextCodec::codecForName("GB2312")); //在此添加设置，即可读写ini文件中的中文 

		QString key; key.sprintf("/%s/%s", SectionName.toLatin1().data(), KeyName.toLatin1().data());
		configIni->setValue(key, strInfo);
		//configIni->setValue(key).toString();
		return true;
	}
	return false;
	
}
