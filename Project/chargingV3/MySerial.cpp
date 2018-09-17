#include "MySerial.h"
#include <QDebug>
int MySerial::serialOpen(QString port,QString bitRate,QString dataLength,QString stopLength,QString parityNum)
{
    QString value[6];
	if (port.toInt() < 10)
		value[1] = "com" + port;
	else
		value[1]="\\\\.\\com"+port;
    value[2]=bitRate;
    value[3]=dataLength;
    value[4]=stopLength;
    value[5]=parityNum;




    struct PortSettings myComSetting = {BAUD9600,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,500};
    myCom = new Win_QextSerialPort(value[1],myComSetting,QextSerialBase::EventDriven);
    if(myCom->open(QIODevice::ReadWrite)==0)
    {
        qDebug()<<(tr("打开串口失败"));
		myCom=NULL;
        return -1;
    }

    if(value[2]=="300")
        {
            myCom->setBaudRate(BAUD300);
        }
        else if(value[2]=="1200")
        {
            myCom->setBaudRate(BAUD1200);
        }
        else if(value[2]=="2400")
        {
            myCom->setBaudRate(BAUD2400);
        }
        else if(value[2]=="4800")
        {
            myCom->setBaudRate(BAUD4800);
        }
        else if(value[2]=="9600")
        {
            myCom->setBaudRate(BAUD9600);
        }
        else if(value[2]=="19200")
        {
            myCom->setBaudRate(BAUD19200);
        }
        else if(value[2]=="38400")
        {
            myCom->setBaudRate(BAUD38400);
        }
        else if(value[2]=="57600")
        {
            myCom->setBaudRate(BAUD57600);
        }
        else if(value[2]=="115200")
        {
            myCom->setBaudRate(BAUD115200);
        }
        else
        {
//            qDebug()<<(tr("不支持的波特率"));
            return -2;
        }

        if(value[3]=="5")
        {
            myCom->setDataBits(DATA_5);
        }
        else if(value[3]=="6")
        {
            myCom->setDataBits(DATA_6);
        }
        else if(value[3]=="7")
        {
            myCom->setDataBits(DATA_7);
        }
        else if(value[3]=="8")
        {
            myCom->setDataBits(DATA_8);
        }
        else
        {
 //           qDebug()<<(tr("不支持的数据位"));
            return -3;
        }

        if(value[4]=="1")
        {
            myCom->setStopBits(STOP_1);
        }
        else if(value[4]=="1.5")
        {
            myCom->setStopBits(STOP_1_5);
        }
        else if(value[4]=="2")
        {
            myCom->setStopBits(STOP_2);
        }
        else
        {
            //qDebug()<<(tr("不支持的停止位"));
            return -4;
        }

        if(value[5]=="NONE")//value[5]==tr("无")||value[5]==tr("无校验")||
        {
            myCom->setParity(PAR_NONE);
        }
        else if(value[5]=="ODD")//value[5]==tr("奇")||value[5]==tr("奇校验")||
        {
            myCom->setParity(PAR_ODD);
        }
        else if(value[5]=="EVEN")//value[5]==tr("偶")||value[5]==tr("偶校验")||
        {
            myCom->setParity(PAR_EVEN);
        }
        else if(value[5]=="SPACE")//value[5]==tr("空")||value[5]==tr("空校验")||
        {
            myCom->setParity(PAR_SPACE);
        }
        else
        {
           qDebug()<<(tr("不支持的校验方式"));
            return -5;
        }

        myCom->setFlowControl(FLOW_OFF);
        myCom->setTimeout(500);

        connect(myCom,SIGNAL(readyRead()),this,SLOT(readNow()));
        read_buff="";
		is_open = true;
        return 0;
}
int MySerial::serialClose()
{
	if (myCom == NULL)return 0;
	myCom->close();
	myCom=NULL;
	is_open = false;
    return 0;
}
int MySerial::serialWrite(QByteArray message)
{
	if(myCom==NULL)return -1;
    myCom->write(message);
    return 0;
}
QByteArray MySerial::serialRead()
{
	if(myCom==NULL)
		return "";
	if (read_buff.isEmpty() == false)
	{
		m_byteArrayOut = read_buff;
		read_buff.resize(0);
		return m_byteArrayOut;
	}else
		return "";
}

void MySerial::readNow()
{
    read_buff = read_buff + myCom->readAll();
    emit readReady();
}


MySerial::MySerial()
{
	myCom=NULL;
	is_open = 0;
}






