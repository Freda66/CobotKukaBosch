#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QMessageBox>

class client : public QTcpSocket
{
	Q_OBJECT

public:
	client(QString sHost, int iPort, QObject *parent=0);
	virtual ~client();
	void sendMessage(QString sMessage);
	
private slots:
	void ServerError(QAbstractSocket::SocketError error);
	void messageReceived();

signals:
	void messageReady(QString);
	void clientError();
	
};

#endif // CLIENT_H
