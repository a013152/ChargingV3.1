#pragma once
/*
���ҳ����ʾ��ص��������

*/
#include <QtWidgets/QWidget>
#include "typedef.h"
class CBatteryPage : public QWidget
{
public:
	CBatteryPage(MAP_CLOSET& mapCloset, MAP_BATTERY& mapBattery,  QWidget *parent = 0);
	~CBatteryPage();
};

