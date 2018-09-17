#include "SingleApplication.h"
#include <QtNetwork/QLocalSocket>
#include <QFileInfo>

#define TIME_OUT                (500)    // 500ms

SingleApplication::SingleApplication(int &argc, char **argv)
: QApplication(argc, argv)
, w(NULL)
, _isRunning(false)
, _localServer(NULL) {

	// ȡӦ�ó�������ΪLocalServer������
	_serverName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();

	_initLocalConnection();
}


////////////////////////////////////////////////////////////////////////////////
// ˵����
// ����Ƿ��ѽ���һ��ʵ��������, true - ��ʵ�����У� false - û��ʵ������
////////////////////////////////////////////////////////////////////////////////
bool SingleApplication::isRunning() {
	return _isRunning;
}

////////////////////////////////////////////////////////////////////////////////
// ˵����
// ͨ��socketͨѶʵ�ֳ���ʵ�����У��������µ�����ʱ�����ú���
////////////////////////////////////////////////////////////////////////////////
void SingleApplication::_newLocalConnection() {
	QLocalSocket *socket = _localServer->nextPendingConnection();
	if (socket) {
		socket->waitForReadyRead(2 * TIME_OUT);
		delete socket;

		// ���������磺��ȡ��������

		_activateWindow();
	}
}

////////////////////////////////////////////////////////////////////////////////
// ˵����
// ͨ��socketͨѶʵ�ֳ���ʵ�����У�
// ��ʼ���������ӣ�������Ӳ���server���򴴽��������˳�
////////////////////////////////////////////////////////////////////////////////
void SingleApplication::_initLocalConnection() {
	_isRunning = false;

	QLocalSocket socket;
	socket.connectToServer(_serverName);
	if (socket.waitForConnected(TIME_OUT)) {
		fprintf(stderr, "%s already running.\n",
			_serverName.toLocal8Bit().constData());
		_isRunning = true;
		// ���������磺�������������͵������
		return;
	}

	//���Ӳ��Ϸ��������ʹ���һ��
	_newLocalServer();
}

////////////////////////////////////////////////////////////////////////////////
// ˵����
// ����LocalServer
////////////////////////////////////////////////////////////////////////////////
void SingleApplication::_newLocalServer() {
	_localServer = new QLocalServer(this);
	connect(_localServer, SIGNAL(newConnection()), this, SLOT(_newLocalConnection()));
	if (!_localServer->listen(_serverName)) {
		// ��ʱ����ʧ�ܣ������ǳ������ʱ,�������̷����µ�,�Ƴ�֮
		if (_localServer->serverError() == QAbstractSocket::AddressInUseError) {
			QLocalServer::removeServer(_serverName); // <-- �ص�
			_localServer->listen(_serverName); // �ٴμ���
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// ˵����
// ����������
////////////////////////////////////////////////////////////////////////////////
void SingleApplication::_activateWindow() {
	if (w) {
		w->show();
		w->raise();
		w->activateWindow(); // �����
	}
}