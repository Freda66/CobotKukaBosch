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

#include <QtXml/QtXml>
#include <iostream>


#include <QObject>
#include <QWidget>
#include <QMap>

/*#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
*/
#include <stdio.h>
#include <stdlib.h>



namespace Ui {
class CobotKuka;
}

class QPaintEvent;
class QMouseEvent;

class CobotKuka : public QMainWindow
{
	Q_OBJECT

public:
	explicit CobotKuka(QWidget *parent = 0);
	void init();
	~CobotKuka();
    QString chaineJSON="";

	/* UI */
	void activate_OK_pushButton();
	void desactivate_OK_pushButton();
	void activate_Send_pushButton();
	void desactivate_Send_pushButton();
	void activate_Stop_pushButton();
	void desactivate_Stop_pushButton();
	void change_Action_Group_Color();
	void change_Action_Group_Activation();

	/* RESEAU */
	bool connectToServer();
	bool disconnectFromServer();

	/* JSON */
	QString getJsonFromSvg(QString svgpath);
	QString getJsonFromPicture();
	QString getJsonFromSketch();
	QString getJsonFromText();
	QString getJsonFromWebcam();
    void writeJSONToServer(const QString& json);
    void setJSONDrawing(const QString& json);

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
	int on_picture_webcam_pushButton_clicked();
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

    //Json Variables
    QString json;


};

#endif // COBOTKUKA_H
