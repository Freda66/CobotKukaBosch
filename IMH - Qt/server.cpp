#include "server.h"

server::server(int port,QObject *parent): QTcpServer(parent)
{
	mClientConnection = NULL;

	if (!this->listen(QHostAddress::Any,port))
	{
		QMessageBox::critical(0, "Serveur","Probleme à l'ouverture du port d'écoute");
		emit serverError();
	}

	QObject::connect(this, SIGNAL(newConnection()), this, SLOT(clientConnected()));
}

void server::clientConnected()
{
	mClientConnection = this->nextPendingConnection();

	QObject::connect(mClientConnection, SIGNAL(readyRead()), this, SLOT(messageReceived()));
    QObject::connect(mClientConnection, SIGNAL(disconnected()), mClientConnection, SLOT(deleteLater()));
}

void server::sendMessage(QString sMessage)
{
	QString sMess = sMessage+"\n";
	if (mClientConnection)
		mClientConnection->write(sMess.toLatin1());
}

void server::messageReceived()
{
	QString sMess = mClientConnection->readAll();
    emit messageReady(sMess);
}


server::~server()
{

}


