#pragma once
/*
电池页，显示电池的配置情况

*/
#include <QtWidgets/QWidget>
#include "typedef.h"
class CBatteryPage : public QWidget
{
public:
	CBatteryPage(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery,  QWidget *parent = 0);
	~CBatteryPage();
};

