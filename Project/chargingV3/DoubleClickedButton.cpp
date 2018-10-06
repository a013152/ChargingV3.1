#include "DoubleClickedButton.h"
#include <QEvent>
#include <QTimer>
#pragma execution_character_set("utf-8")
DoubleClickedButton::DoubleClickedButton(QWidget *parent)
: QPushButton(parent),
m_single_clicked_timer(new QTimer(this)),
m_double_clicked_timer(new QTimer(this)),
m_bDoubleClicked(false)
{
	m_single_clicked_timer->setSingleShot(true);
	m_double_clicked_timer->setSingleShot(true);
	connect(m_single_clicked_timer, SIGNAL(timeout()), this, SLOT(timer_out_single_clicked())/* SIGNAL(singleClicked())*/);
	connect(m_double_clicked_timer, SIGNAL(timeout()), this, SLOT(timer_out_double_clicked())/* SIGNAL(singleClicked())*/);
}

DoubleClickedButton::DoubleClickedButton(const QString &text, QWidget *parent)
: DoubleClickedButton(parent)
{
	setText(text);
}

DoubleClickedButton::DoubleClickedButton(const QIcon &icon, const QString &text, QWidget *parent)
: DoubleClickedButton(text, parent)
{
	setIcon(icon);
}

DoubleClickedButton::~DoubleClickedButton()
{
}


void DoubleClickedButton::timer_out_single_clicked()
{
	emit singleClicked(); 
}

void DoubleClickedButton::timer_out_double_clicked()
{
	m_bDoubleClicked = false;
}

bool DoubleClickedButton::event(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::MouseButtonRelease:
		m_bDoubleClicked ? m_bDoubleClicked = false//如果是第二次进入，重置状态
			/*这里需要一个超时判断，超时还没进入MouseButtonDblClick事件才算是单击效果*/
			: m_single_clicked_timer->start(300);//测试发现间隔时间最好不要低于10ms，否则不够两个事件的反馈间隔，如果需要延迟，可以自行修改间隔时间
		m_double_clicked_timer->stop();
		break;
	case QEvent::MouseButtonDblClick:
		m_single_clicked_timer->stop();//进入双击事件停止掉定时器
		m_bDoubleClicked = true;
		emit doubleClicked();
		m_double_clicked_timer->start(500); //500毫秒后，m_bDoubleClicked = false；恢复单击标志
		break;
	default:
		break;
	}

	return QPushButton::event(event);
}
