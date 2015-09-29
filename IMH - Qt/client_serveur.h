#ifndef CLIENT_SERVEUR_H
#define CLIENT_SERVEUR_H

#include <QMainWindow>
#include "ui_client_serveur.h"
#include "client.h"
#include "server.h"

class Client_serveur : public QMainWindow
{
	Q_OBJECT

public:
    Client_serveur(QWidget *parent = 0);
	~Client_serveur();

private:
	Ui::Client_serveurClass ui;

	client* mClient;
	server* mServer;

private slots:
	void clientConnected();
	void clientNotConnected();
	void serverConnected();
	void serverNotConnected();
	void connectionServer();
	void connectionClient();
	void clientSendMess();
	void serverSendMess();
};

#endif // CLIENT_SERVEUR_H
