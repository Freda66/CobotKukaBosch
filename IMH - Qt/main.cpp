#include "client_serveur.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Client_serveur w;
	w.show();
	return a.exec();
}
