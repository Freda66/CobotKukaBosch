#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

class server : public QTcpServer
{
	Q_OBJECT

public:
	server(int port,QObject *parent=0);
	virtual ~server();
	void sendMessage(QString sMessage);

private:
	QTcpSocket *mClientConnection;
	
private slots:
	void clientConnected();
	void messageReceived();

signals:
	void messageReady(QString);
	void serverError();
	
};

#endif // SERVER_H
