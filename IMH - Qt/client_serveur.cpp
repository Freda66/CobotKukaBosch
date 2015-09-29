#include "client_serveur.h"

Client_serveur::Client_serveur(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	ui.pbClientSendMess->setDisabled(true);
	ui.pbServerSendMess->setDisabled(true);

	mClient = NULL;
	mServer = NULL;

	QObject::connect(ui.pbClientConnect,SIGNAL(clicked()),this,SLOT(connectionClient()));
	QObject::connect(ui.pbServerConnect,SIGNAL(clicked()),this,SLOT(connectionServer()));
	
	QObject::connect(ui.pbClientSendMess,SIGNAL(clicked()),this,SLOT(clientSendMess()));
	QObject::connect(ui.pbServerSendMess,SIGNAL(clicked()),this,SLOT(serverSendMess()));
}

Client_serveur::~Client_serveur()
{

}

void Client_serveur::connectionClient()
{
	if (mClient == NULL)
	{
		mClient = new client(ui.leClientAdresse->text(),ui.leClientPort->text().toInt());	
		QObject::connect(mClient,SIGNAL(connected()),this,SLOT(clientConnected()));
		QObject::connect(mClient,SIGNAL(clientError()),this,SLOT(clientNotConnected()));
		QObject::connect(mClient,SIGNAL(messageReady(QString)),ui.teClientReception,SLOT(append(QString)));
	}
}

void Client_serveur::connectionServer()
{
	if (mServer == NULL)
	{
		mServer = new server(ui.leServerPort->text().toInt());	
		ui.pbServerConnect->setDisabled(true);
		QObject::connect(mServer, SIGNAL(newConnection()), this, SLOT(serverConnected()));
		QObject::connect(mServer, SIGNAL(serverError()), this, SLOT(serverNotConnected()));
		QObject::connect(mServer,SIGNAL(messageReady(QString)),ui.teServerReception,SLOT(append(QString)));
	}
}

void Client_serveur::clientSendMess()
{
	if(mClient == NULL)
		return;

	mClient->sendMessage(ui.leClientMessToSend->text());
	ui.leClientMessToSend->clear();
}

void Client_serveur::serverSendMess()
{
	if(mServer == NULL)
		return;

	mServer->sendMessage(ui.leServerMessToSend->text());
	ui.leServerMessToSend->clear();
}

void Client_serveur::clientConnected()
{
	ui.pbClientConnect->setDisabled(true);
	ui.pbClientSendMess->setDisabled(false);
}

void Client_serveur::clientNotConnected()
{
	delete mClient;
	mClient = NULL;
}

void Client_serveur::serverConnected()
{
	ui.pbServerSendMess->setDisabled(false);
}

void Client_serveur::serverNotConnected()
{
	ui.pbServerConnect->setDisabled(false);
}
