#pragma once 
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QProgressBar>
#include <qdebug.h>
#include "common.h"
#include "DoubleClickedButton.h"
#pragma execution_character_set("utf-8")
extern char g_AppPath[256];  //exe路径 
//QGroupBox 样式表
//QGroupBox{
//	border - width:1px;   //线的粗细
//	border - style:solid;
//	border - color:lightGray;   //颜色，
//	margin - top: 0.5ex;  //文字在方框中位置的偏离度
//}
//QGroupBox::title{
//	subcontrol - origin: margin;
//	subcontrol - position: top left;
//left:25px;       //线的偏离度
//	margin - left: 0px;
//padding:0 1px;   //文字在方框中位置的偏离度
//}

//充电格UI类
class ui_charg_grid :public QGroupBox
{
	Q_OBJECT
public:
	ui_charg_grid(QWidget * parent, QString Id);
	~ui_charg_grid();

	void setVol(float vol);  //设置电压

	void setCurrent(float cur);  //设置电流

	void setTemperature(float tem);  //设置温度

	void setBatteryState(QString strState, QString strVol); //设置电池状态

	void setChargerState(QString strState); //设置充电状态
	
	void setBalence(bool flag); //设置平衡 平衡 电池用放电

	void setBatteryModel(QString strModel); //设置电池型号

	//void setTitle(QString strId); //设置标题

	void setBackgroudColor(int iState);  //0 灰色 1绿色

	void setBorderColor(int iState);     //0 灰色 1蓝色 

	DoubleClickedButton * getChargingBtn(){ return pushButton; }
	QPushButton * getDisChargingBtn(){ return pushButton_2; }

	bool getCharging(){ return isCharging; }
	bool getDisCharging(){ return isDisCharging; }  

	//void setTitle(QString strId);
	//状态图片加载路径  //为了非智能电池加载正确的图片路径
	QString m_strBtnStyleOffline;
	QString m_strBtnStyleFree;
	QString m_strBtnStyleCharging;
	QString m_strBtnStyleDischarg;
	QString m_strBtnStyleOverHeat;
private:
	QLabel *label = NULL;    //电压
	QLabel *label_2 = NULL;	 //电流
	QLabel *label_3 = NULL;	 //充电器状态
	 
	QLabel *label_5 = NULL;	 //电池型号
	QLabel *label_6 = NULL;	 //文字 电池型号
	QLabel *label_7 = NULL;	 //文字 电池状态

	QLabel *labBatteryState = NULL;	 //电池状态图
	QLabel *labChargerState = NULL;	 //充电器状态图
	//QLabel *labTitle = NULL;	 //标题

	DoubleClickedButton *pushButton = NULL;   //充/放结合
	QPushButton *pushButton_2 = NULL;	//放
	QProgressBar* progressBar_1 = NULL; //电压
	QProgressBar* progressBar_2 = NULL; //电流
	QProgressBar* progressBar_3 = NULL; //温度

	

	bool isBalence;
	bool isCharging;
	bool isDisCharging;
	 
};

