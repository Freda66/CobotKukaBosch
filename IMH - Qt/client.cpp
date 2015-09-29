#include "client.h"

client::client(QString sHost, int iPort, QObject *parent): QTcpSocket(parent)
{
	QObject::connect(this,SIGNAL(error( QAbstractSocket::SocketError)),this,SLOT(ServerError(QAbstractSocket::SocketError)));
	QObject::connect(this, SIGNAL(readyRead()), this, SLOT(messageReceived()));
	
	this->connectToHost(sHost,iPort);
}

client::~client()
{

}

void client::ServerError(QAbstractSocket::SocketError error)
{
	QMessageBox::critical(0, "Client","Probleme à la connexion");
	emit clientError();
}

void client::sendMessage(QString sMessage)
{
	QString sMess = sMessage+"\n";
    write(sMess.toLatin1());
}

void client::messageReceived()
{
	QString sMess = readAll();
    emit messageReady(sMess);
}
