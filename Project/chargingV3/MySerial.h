#ifndef MYSERIAL_H
#define MYSERIAL_H

#include "qextserialbase.h"
#include "win_qextserialport.h"


//这个被文件被修改过，读取数据的方式由QString改为QByteArray
class MySerial :public QObject
{
    Q_OBJECT
public:
	MySerial();
	bool isOpen(){ return is_open; }

	QByteArray m_byteArrayOut; //对外的存储区，用于读取
private:
   Win_QextSerialPort *myCom;
   QByteArray read_buff;
   bool is_open;//是否打开
public slots:
   int serialOpen(QString port="4",QString bitRate="19200",QString dataLength="8",QString stopLength="1",QString parityNum="NONE");
   /*
    * value[1]串口号 “4”
    * value[2]波特率 “9600”
    * value[3]数据位 “8”
    * value[4]停止位 “1”
    * value[5]校验位 “tr(”无“)”
    */
   int serialClose();

   int serialWrite(QByteArray message);

   QByteArray serialRead();  //读取数据

private slots:
   void readNow();
signals:
   void readReady();



};




#endif // MYSERIAL_H
