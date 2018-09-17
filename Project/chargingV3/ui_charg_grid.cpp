#include "ui_charg_grid.h"


ui_charg_grid::ui_charg_grid(QWidget *parent, QString Id) : QGroupBox(parent)
{  
	//this = new QGroupBox(parent); 
	this->setObjectName(Id);
	
	isBalence = false;
	isCharging = false;
	isDisCharging = false; 
	//groupBox->setGeometry(QRect(20, 60, 281, 121));
	label = new QLabel("电压", this);
	label->setObjectName(QStringLiteral("label"));
	label->setGeometry(QRect(20, 70, 41, 20));
	label_2 = new QLabel("电流", this);
	label_2->setObjectName(QStringLiteral("label_2"));
	label_2->setGeometry(QRect(80, 70, 41, 20));
	label_2->setStyleSheet("QLabel{color:white;}");
	label_3 = new QLabel("充电器闲置", this);
	label_3->setObjectName(QStringLiteral("label_3"));
	label_3->setGeometry(QRect(160, 40, 80, 20));
	label_3->setStyleSheet("QLabel{color:white;}");
	progressBar_1 = new QProgressBar(this);
	progressBar_1->setObjectName(QStringLiteral("progressBar_1"));
	progressBar_1->setGeometry(QRect(20, 15, 21, 55));
	progressBar_1->setValue(1);
	progressBar_1->setTextVisible(false);
	progressBar_1->setOrientation(Qt::Vertical);
	progressBar_2 = new QProgressBar(this);
	progressBar_2->setObjectName(QStringLiteral("progressBar_2"));
	progressBar_2->setGeometry(QRect(80, 15, 21, 55));
	progressBar_2->setValue(1);
	progressBar_2->setTextVisible(false);
	progressBar_2->setOrientation(Qt::Vertical);
	progressBar_3 = new QProgressBar(this);
	progressBar_3->setObjectName(QStringLiteral("progressBar_3"));
	progressBar_3->setGeometry(QRect(140, 15, 21, 55));
	progressBar_3->setValue(24);
	progressBar_3->setTextVisible(false);
	progressBar_3->setOrientation(Qt::Vertical);


	label_4 = new QLabel("电池状态", this);
	label_4->setObjectName(QStringLiteral("label_4"));
	label_4->setGeometry(QRect(90, 65, 80, 20));
	label_4->setStyleSheet("QLabel{color:rgb(17,191,255,255);}");

	label_5 = new QLabel("M100", this);
	label_5->setObjectName(QStringLiteral("label_5"));
	label_5->setGeometry(QRect(10, 30, 80, 20));
	label_5->setStyleSheet("QLabel{color:white;}");

	label_6 = new QLabel("电池型号", this);
	label_6->setObjectName(QStringLiteral("label_5"));
	label_6->setGeometry(QRect(15, 65, 75, 20));
	label_6->setStyleSheet("QLabel{color:rgb(17,191,255,255);}");

	labBatteryState = new QLabel("", this);
	labBatteryState->setObjectName(QStringLiteral("labBatteryState"));
	labBatteryState->setGeometry(QRect(15, 18, 53, 48));
	labBatteryState->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/battery_1_not_online.png);}");

	labChargerState = new QLabel("", this);
	labChargerState->setObjectName(QStringLiteral("labBatteryState"));
	labChargerState->setGeometry(QRect(25, 31, 35, 28));
	labChargerState->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/0.png);}");
	

	pushButton = new DoubleClickedButton("", this);  //充电
	pushButton->setObjectName(QStringLiteral("pushButton")); 
	pushButton->setGeometry(QRect(0, 3, 85, 73));
	//pushButton->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_Gray.png);}");
	/*static int i = 0;
	i++;
	if (i % 4 == 0){
		pushButton->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Normal.png);}"
			"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_Hover.png);}");
	}
	else if (i % 4 == 1){
		pushButton->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_Gray.png);}");
	}
	else if (i % 4 == 2){
		pushButton->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_Red.png);}");
		 
	}
	else{
		pushButton->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Pressed.png);}"
			"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnCharge_Pressed_Hover.png);}");
		labBatteryState->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/battery_3_online_half.png);}");
		labChargerState->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/disCharging.png);}");
	}*/
	pushButton_2 = new QPushButton("", this); //放电
	pushButton_2->setObjectName(QStringLiteral("pushButton_2")); 
	pushButton_2->setGeometry(QRect(165, 15, 50, 65));
	pushButton_2->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btnDischarge.png);}");

	/*labTitle = new QLabel("001", this);
	labTitle->setObjectName(QStringLiteral("labBatteryState"));
	labTitle->setGeometry(QRect(0, 0, 40, 18));
	labTitle->setStyleSheet("QLabel{font-size:16px;font-weight:bold;color:white}");*/
				

	//隐藏不用的控件 20180123
	label->setVisible(false);
	label_2->setVisible(false);
	label_3->setVisible(false);
	progressBar_1->setVisible(false);
	progressBar_2->setVisible(false);
	progressBar_3->setVisible(false);

	//隐藏不用的控件 20180426
	//labBatteryState->setVisible(false);
	//pushButton->setVisible(false); 
	pushButton_2->setVisible(false);
	label_6->setVisible(false);
	label_5->setVisible(false);
	
	setBorderColor(0);

	this->setFixedSize(QSize(120, 110)); //设置自身的大小
	m_strBtnStyleOffline = "QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_Gray.png);}";
	m_strBtnStyleFree = "QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Normal.png);}"
		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_Hover.png);}";
	m_strBtnStyleCharging = "QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Pressed.png);}"
		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnCharge_Pressed_Hover.png);}";
	m_strBtnStyleDischarg = "QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Pressed.png);}"
		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnCharge_Pressed_Hover.png);}";
	m_strBtnStyleOverHeat = "QPushButton{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_Red.png);}"
		"QPushButton:hover{border-image: url(" + g_AppPath + "/img/btnCharge_Normal_Red.png);}";
	 	 
}


ui_charg_grid::~ui_charg_grid()
{
}

//设置电压
void ui_charg_grid::setVol(float vol)
{
	auto transfun = [&](float vol)->int {		 
		int ret = 0;
		if (vol <= 3.5)
			ret = 1;
		else if (vol >= 4.35)
			ret = 99;
		else if (vol > 3.5 && vol < 4.35)
		{
			ret = (vol - 3.5) * 100 / (4.35 - 3.5);
		}
		return ret;
	};
	QString strVol; strVol.sprintf("%3.2fV", vol);

	label->setText(strVol);

	progressBar_1->setValue(transfun(vol)); 
}

//设置电流
void ui_charg_grid::setCurrent(float cur)
{
	auto fun = [](float cur)->int{
		// 暂时规定最大电流8A		 
		int ret = 0;
		if (cur <= 0.0)
			ret = 0;
		else if (cur >= 8)
			ret = 99;
		else if (cur < 8 && cur > 0.0)
			ret = cur * 100 / 8;
		return ret;
	};

	QString strCur; strCur.sprintf("%3.2fA", cur);

	label_2->setText(strCur);

	progressBar_2->setValue(fun(cur));

}

//设置温度
void ui_charg_grid::setTemperature(float tem)
{
	/*auto fun = [](float tem)->int {
		int ret = 0; 
		if (tem <= -25)
			ret = 0;
		else if (tem >= 125)
			ret = 100;
		else if (tem <125 && tem > -25)
			ret = tem * 100 / 150;
		return ret;
	};
	QString strTemp; strTemp.sprintf("%3.1f°", tem);

	label_3->setText(strTemp);

	progressBar_3->setValue(fun(tem));*/

}

//设置状态 setBatteryState：等待中、 未放置电池
void ui_charg_grid::setBatteryState(QString strState ,QString strVol)
{
	//需要加载不同的图片
	//label_4->setText(strState);
	if (strState == "电池在线")
	{ 
		//判断电压
		float fvol = strVol.toFloat();
		if (fvol < 3.5)
			labBatteryState->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/battery_2_online.png);}");
		else if (fvol >= 3.5 && fvol < 4.10)
			labBatteryState->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/battery_3_online_half.png);}");
		else if (fvol >= 4.10 && fvol < 4.3)
			labBatteryState->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/battery_4_online_full.png);}");
	}
	else if (strState == "未放置电池")
	{ 
		labBatteryState->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/battery_1_not_online.png);}");

	}
}
//设置状态 ： 充电器闲置、充电中、
void ui_charg_grid::setChargerState(QString strState)
{ 
	
	label_3->setText(strState);
	if (strState == STATE_OFFLINE){ //不在线
		pushButton->setStyleSheet(m_strBtnStyleOffline);
		isCharging = false;
		isDisCharging = false;
	}
	 else if (strState == STATE_FREE){  //闲置
		pushButton->setStyleSheet(m_strBtnStyleFree);
		pushButton_2->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btnDischarge.png);}");
		labChargerState->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/0.png);}");

		//pushButton->setText("充电"); //充电
		//pushButton_2->setText("放电");//放电
		isCharging = false;
		isDisCharging = false;
	}
	else if (strState == STATE_CHARGING)  //充电中
	{
		pushButton->setStyleSheet(m_strBtnStyleCharging);
		pushButton_2->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btnDischarge.png);}");
		labChargerState->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/charging.png);}");

		//pushButton->setText("停止"); //停止
		//pushButton_2->setText("放电"); //放电
		isCharging = true;
		isDisCharging = false;
	}
	else if (strState == STATE_DISCHARGING)  //放电中
	{
		pushButton->setStyleSheet(m_strBtnStyleDischarg);
		pushButton_2->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btnDischarge.png);}");
		labChargerState->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/disCharging.png);}");

		//pushButton->setText("充电");//充电
		//pushButton_2->setText("停止");//停止
		isCharging = false;
		isDisCharging = true;
	}
	else if (strState == STATE_OVER_HEAT)//高温
	{ 
		pushButton->setStyleSheet(m_strBtnStyleOverHeat);
		pushButton_2->setStyleSheet("QPushButton{border-image: url(" + g_AppPath + "/img/btnDischarge.png);}");
		labChargerState->setStyleSheet("QLabel{border-image: url(" + g_AppPath + "/img/0.png);}");
		 
		isCharging = false;
		isDisCharging = false;
	}
	//
	
}

//设置平衡 平衡（智能）电池不用放电
void ui_charg_grid::setBalence(bool flag)
{
	isBalence = flag;
	//pushButton_2->setVisible(!flag); 
}

void ui_charg_grid::setBatteryModel(QString strModel)
{
	label_5->setText(strModel);
}

void ui_charg_grid::setBackgroudColor(int iState)
{
	if (iState == 0){ 
		this->setStyleSheet("QGroupBox{background:red}");
	}
	else if (iState == 1){
		this->setStyleSheet("QGroupBox{background:blue}");
	}
}
void ui_charg_grid::setBorderColor(int iState)
{ 
	if (iState == 0){ 
		//this->setStyleSheet("QGroupBox{border: 2px solid red;}QGroupBox:title{font: bold 14px;color:white;}");
		QString str = this->styleSheet();
		this->setStyleSheet("QGroupBox{font-size:16px;font-weight:bold;border: 2px solid gray;border-radius:8px;margin-top:6px;}\
						QGroupBox:title{color:white;subcontrol-origin: margin;left: 0px;}");
		str = this->styleSheet();
	}
	else if (iState == 1){
		//this->setStyleSheet("QGroupBox{border: 2px solid lime;}QGroupBox:title{font: bold 14px;color:white;}");
		this->setStyleSheet("QGroupBox{font-size:16px;font-weight:bold;border: 2px solid rgb(28, 116, 187, 255);border-radius:4px;margin-top:6px;}\
									QGroupBox:title{color:white;subcontrol-origin: margin;left: 0px;}");
	}
	
}
/**/
//设置标题
//void ui_charg_grid::setTitle(QString strId)
//{
//	labTitle->setText(strId);
//	QGroupBox::setTitle("");
//}