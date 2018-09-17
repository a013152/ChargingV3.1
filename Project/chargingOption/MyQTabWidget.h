#pragma once
#include <QtWidgets/QTabWidget>
#include <QtGui/QEnterEvent>
#include <QtWidgets/QComboBox>
class MyQTabWidget:public QTabWidget
{
	Q_OBJECT
public:
public:
	MyQTabWidget(QWidget *parent = 0);
	~MyQTabWidget();
	void wheelEvent(QWheelEvent * event)
	{
		if (event->delta() > 0){
			//放上
		}
		else{
			 //向下
		}
		//判断鼠标是否在combobox上
		QPoint point1 = mapFromGlobal(cursor().pos());
		if (m_ComboBox){
			QRect rc = m_ComboBox->geometry();
			if (rc.contains(point1) == false)
			{
				m_ComboBox->setVisible(false);
			}
		}
		//PtInRect();

	}
private:
	QComboBox* m_ComboBox =nullptr;
};

