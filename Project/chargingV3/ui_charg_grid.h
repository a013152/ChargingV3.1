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
extern char g_AppPath[256];  //exe·�� 
//QGroupBox ��ʽ��
//QGroupBox{
//	border - width:1px;   //�ߵĴ�ϸ
//	border - style:solid;
//	border - color:lightGray;   //��ɫ��
//	margin - top: 0.5ex;  //�����ڷ�����λ�õ�ƫ���
//}
//QGroupBox::title{
//	subcontrol - origin: margin;
//	subcontrol - position: top left;
//left:25px;       //�ߵ�ƫ���
//	margin - left: 0px;
//padding:0 1px;   //�����ڷ�����λ�õ�ƫ���
//}

//����UI��
class ui_charg_grid :public QGroupBox
{
	Q_OBJECT
public:
	ui_charg_grid(QWidget * parent, QString Id);
	~ui_charg_grid();

	void setVol(float vol);  //���õ�ѹ

	void setCurrent(float cur);  //���õ���

	void setTemperature(float tem);  //�����¶�

	void setBatteryState(QString strState, QString strVol); //���õ��״̬

	void setChargerState(QString strState); //���ó��״̬
	
	void setBalence(bool flag); //����ƽ�� ƽ�� ����÷ŵ�

	void setBatteryModel(QString strModel); //���õ���ͺ�

	//void setTitle(QString strId); //���ñ���

	void setBackgroudColor(int iState);  //0 ��ɫ 1��ɫ

	void setBorderColor(int iState);     //0 ��ɫ 1��ɫ 

	DoubleClickedButton * getChargingBtn(){ return pushButton; }
	QPushButton * getDisChargingBtn(){ return pushButton_2; }

	bool getCharging(){ return isCharging; }
	bool getDisCharging(){ return isDisCharging; }  

	//void setTitle(QString strId);
	//״̬ͼƬ����·��  //Ϊ�˷����ܵ�ؼ�����ȷ��ͼƬ·��
	QString m_strBtnStyleOffline;
	QString m_strBtnStyleFree;
	QString m_strBtnStyleCharging;
	QString m_strBtnStyleDischarg;
	QString m_strBtnStyleOverHeat;
private:
	QLabel *label = NULL;    //��ѹ
	QLabel *label_2 = NULL;	 //����
	QLabel *label_3 = NULL;	 //�����״̬
	 
	QLabel *label_5 = NULL;	 //����ͺ�
	QLabel *label_6 = NULL;	 //���� ����ͺ�
	QLabel *label_7 = NULL;	 //���� ���״̬

	QLabel *labBatteryState = NULL;	 //���״̬ͼ
	QLabel *labChargerState = NULL;	 //�����״̬ͼ
	//QLabel *labTitle = NULL;	 //����

	DoubleClickedButton *pushButton = NULL;   //��/�Ž��
	QPushButton *pushButton_2 = NULL;	//��
	QProgressBar* progressBar_1 = NULL; //��ѹ
	QProgressBar* progressBar_2 = NULL; //����
	QProgressBar* progressBar_3 = NULL; //�¶�

	

	bool isBalence;
	bool isCharging;
	bool isDisCharging;
	 
};

