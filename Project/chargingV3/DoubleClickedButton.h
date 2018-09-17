#pragma once
#include <QPushButton>
class QTimer;
class DoubleClickedButton :
	public QPushButton
{
	Q_OBJECT

public:
	DoubleClickedButton(QWidget *parent = Q_NULLPTR);
	DoubleClickedButton(const QString &text, QWidget *parent = Q_NULLPTR);
	DoubleClickedButton(const QIcon &icon, const QString &text, QWidget *parent = Q_NULLPTR);

	~DoubleClickedButton();

protected:
	virtual bool event(QEvent* event);
signals:
	void singleClicked();
	void doubleClicked();
private slots :
	//¶¨Ê±Æ÷³¬Ê±
	void timer_out_single_clicked();
	void timer_out_double_clicked();
private:
	bool m_bDoubleClicked;
	QTimer* m_single_clicked_timer;
	QTimer* m_double_clicked_timer;
};

