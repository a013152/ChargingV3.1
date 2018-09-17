#pragma once

/*
电池型号页，显示电池型号的配置情况 

*/

#include <QtWidgets/QWidget>
#include "typedef.h"
class CBatteryModelPage : public QWidget
{
public:
	CBatteryModelPage(MAP_BATTERY_MODEL& mapBatteryModel, QWidget *parent = 0);
	~CBatteryModelPage();
};

