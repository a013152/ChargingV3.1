#pragma once

/*
����ͺ�ҳ����ʾ����ͺŵ�������� 

*/

#include <QtWidgets/QWidget>
#include "typedef.h"
class CBatteryModelPage : public QWidget
{
public:
	CBatteryModelPage(MAP_BATTERY_MODEL& mapBatteryModel, QWidget *parent = 0);
	~CBatteryModelPage();
};

