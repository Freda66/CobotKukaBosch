#include "cobotkuka.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CobotKuka w;
	w.show();

	return a.exec();
}
