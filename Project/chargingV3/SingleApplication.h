#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QObject>
#include <QApplication>
#include <QtNetwork/QLocalServer>
#include <QWidget>

class SingleApplication : public QApplication {
	Q_OBJECT
public:
	SingleApplication(int &argc, char **argv);

	bool isRunning();                // �Ƿ��ѽ���ʵ��������
	QWidget *w;                        // MainWindowָ��

	private slots:
	// ��������ʱ����
	void _newLocalConnection();

private:
	// ��ʼ����������
	void _initLocalConnection();
	// ���������
	void _newLocalServer();
	// �����
	void _activateWindow();

	bool _isRunning;                // �Ƿ��ѽ���ʵ��������
	QLocalServer *_localServer;     // ����socket Server
	QString _serverName;            // ��������
};

#endif // SINGLEAPPLICATION_H
