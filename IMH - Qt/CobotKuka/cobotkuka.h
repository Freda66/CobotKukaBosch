#ifndef COBOTKUKA_H
#define COBOTKUKA_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>
#include <QtSvg/QtSvg>
#include <QtSvg/QSvgRenderer>
#include <QtSvg/QtSvgDepends>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QHostAddress>

#include "json.h"
#include <wingdi.h>

#include <QtXml/QtXml>
#include <iostream>

namespace Ui {
class CobotKuka;
}

class CobotKuka : public QMainWindow
{
	Q_OBJECT

public:
	explicit CobotKuka(QWidget *parent = 0);
	void init();
	~CobotKuka();


	void activate_OK_pushButton();
	void desactivate_OK_pushButton();
	void activate_Send_pushButton();
	void desactivate_Send_pushButton();
	void activate_Stop_pushButton();
	void desactivate_Stop_pushButton();
	void change_Action_Group_Color();
	void writeJSONToServer(const QStringList& jsonList);
	bool connectToServer();
	bool disconnectFromServer();
	void getJsonFromSvg(QString svgpath);


private slots:

//	Client slots
	void displayError(QAbstractSocket::SocketError socketError);
	void serverConnected();
	void serverDisconnected();
	void datawritten(qint64 w);

	//ui slots
	void on_connect_pushButton_clicked(bool checked);

	void on_text_font_fontComboBox_currentFontChanged(const QFont &f);

	void on_svg_radioButton_clicked();

	void on_text_radioButton_clicked();

	void on_picture_radioButton_clicked();

	void on_sketch_radioButton_clicked();

	void on_picture_webcam_radioButton_clicked();

	void on_picture_file_radioButton_clicked();

	void on_svg_file_pushButton_clicked();

	void on_send_pushButton_clicked();

	void on_picture_file_pushButton_clicked();

	void on_actionQuit_triggered();

	void on_ok_pushButton_clicked();

	void on_CobotKuka_destroyed();

	void on_text_text_lineEdit_editingFinished();

	void on_stop_pushButton_clicked();

	void on_picture_webcam_pushButton_clicked();

	void on_sketch_pushButton_clicked();

	private:
	//ui variables
	Ui::CobotKuka *ui;
	QFileDialog *filedialog ;
	QSvgRenderer *svgrenderer ;
	boolean connected;
	QString svgFile;
	QString pictureFile;

	//Client variables
	QTcpSocket *tcpSocket;
	QHostAddress ip;
	QStringList jsonChainList;
};

#endif // COBOTKUKA_H
