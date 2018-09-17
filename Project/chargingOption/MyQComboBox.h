#pragma once
#include <QComboBox>
#include <QWidget>
#include <QTableWidget>
#include<QDebug>
#include <QEvent>
#include <QtGui/QEnterEvent>
class MyQComboBox :public QComboBox
{
	Q_OBJECT
public:
	MyQComboBox(QWidget *parent = 0);
	~MyQComboBox();
protected:
	virtual void focusOutEvent(QFocusEvent *e)
	{
		//this->setVisible(false); //失去焦点，自动隐藏
		static int i;
		qDebug() << QString("Combobox Focus Out ")<< QString::number(i++);
	}
	virtual void focusInEvent(QFocusEvent *e)
	{
		//this->setVisible(true);
		static int i;
		qDebug() << QString("Combobox Focus In ") << QString::number(i++);
		QPoint point1 = mapFromGlobal(cursor().pos());
		QRect rc = this->geometry();
		if (rc.contains(point1) == true)
			this->setVisible(false);

	}
	bool eventFilter(QObject *watched, QEvent *event)
	{
		if (watched == this)         //首先判断控件(这里指 lineEdit)       
		{
			if (event->type() == QEvent::FocusIn)     //然后再判断控件的具体事件 (这里指获得焦点事件)            
			{
				QPalette p = QPalette();
				p.setColor(QPalette::Base, Qt::green);
				this->setPalette(p);
			}
			else if(event->type() == QEvent::FocusOut)    // 这里指 lineEdit 控件的失去焦点事件            
			{
				QPalette p = QPalette();
				p.setColor(QPalette::Base, Qt::white);
				this->setPalette(p);
			}
		}
		return QWidget::eventFilter(watched, event);     // 最后将事件交给上层对话框  
	}
	void wheelEvent(QWheelEvent* event)
	{
		if (m_parent)
		{
			///*((QTableWidget *)m_parent)->wheelEvent(event);*/

			//int numDegrees = event->delta() / 8;//滚动的角度，*8就是鼠标滚动的距离
			//int numSteps = numDegrees / 15;//滚动的步数，*15就是鼠标滚动的角度
			//if (event->orientation() == Qt::Horizontal) {
			//	scrollHorizontally(numSteps);       //水平滚动
			//}
			//else {
			//	scrollVertically(numSteps);       //垂直滚动
			//}
		}
	}


	QWidget *m_parent;
};

