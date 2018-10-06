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
		m_bDoubleClicked ? m_bDoubleClicked = false//����ǵڶ��ν��룬����״̬
			/*������Ҫһ����ʱ�жϣ���ʱ��û����MouseButtonDblClick�¼������ǵ���Ч��*/
			: m_single_clicked_timer->start(300);//���Է��ּ��ʱ����ò�Ҫ����10ms�����򲻹������¼��ķ�������������Ҫ�ӳ٣����������޸ļ��ʱ��
		m_double_clicked_timer->stop();
		break;
	case QEvent::MouseButtonDblClick:
		m_single_clicked_timer->stop();//����˫���¼�ֹͣ����ʱ��
		m_bDoubleClicked = true;
		emit doubleClicked();
		m_double_clicked_timer->start(500); //500�����m_bDoubleClicked = false���ָ�������־
		break;
	default:
		break;
	}

	return QPushButton::event(event);
}
