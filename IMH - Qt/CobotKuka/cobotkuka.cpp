#include "cobotkuka.h"
#include "ui_cobotkuka.h"



void CobotKuka::init(){
	//disable the actions while not connected
	ui->picture_groupBox->setEnabled(false);
	ui->text_groupBox->setEnabled(false);
	ui->svg_groupBox->setEnabled(false);
	ui->sketch_groupBox->setEnabled(false);


	//TODO : load TTF fonts exclusively

	ui->svg_actions_groupBox->setEnabled(true);

	ui->text_actions_groupBox->setEnabled(false);
	ui->picture_actions_groupBox->setEnabled(false);
	ui->sketch_actions_groupBox->setEnabled(false);

	//initialise the TCP Socket
	tcpSocket = new QTcpSocket(this);
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),				this, SLOT(displayError(QAbstractSocket::SocketError)));
	connect(tcpSocket, SIGNAL(connected()), this, SLOT(serverConnected()));
	connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(serverDisconnected()));
	connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(datawritten(qint64)));

	connected = false;
}

CobotKuka::CobotKuka(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CobotKuka)
{
	ui->setupUi(this);
	init();
}

CobotKuka::~CobotKuka()
{
	delete ui;
}

void CobotKuka::on_connect_pushButton_clicked()
{
	if(!connected){
		if(ip.setAddress(ui->connect_ip_lineEdit->text())){
			qDebug() << "trying to connect to server " << ip << ":" << ui->connect_port_lineEdit->text().toInt();
				tcpSocket->connectToHost(ip, ui->connect_port_lineEdit->text().toInt());
		}
		else {
			QMessageBox::warning(this,"Socket Error : IP Adress Invalid", "The IP Adress is invalid. \nIP Adress must be in the form of xxx.xxx.xxx.xxx with only digits and dots. example : 192.168.0.18 \nPlease enter a valid Adress and retry.", QMessageBox::Ok, QMessageBox::NoButton);
		}
	}
	else{
		tcpSocket->disconnectFromHost();
	}
}
/* Setting Font */
void CobotKuka::on_text_font_fontComboBox_currentFontChanged(const QFont &f)
{
	ui->text_text_lineEdit->setFont(f);
}

/* Mod Selection*/
void CobotKuka::on_svg_radioButton_clicked()
{
	ui->svg_actions_groupBox->setEnabled(true);

	ui->text_actions_groupBox->setEnabled(false);
	ui->picture_actions_groupBox->setEnabled(false);
	ui->sketch_actions_groupBox->setEnabled(false);
}

void CobotKuka::on_text_radioButton_clicked()
{
	ui->text_actions_groupBox->setEnabled(true);

	ui->svg_actions_groupBox->setEnabled(false);
	ui->picture_actions_groupBox->setEnabled(false);
	ui->sketch_actions_groupBox->setEnabled(false);
}

void CobotKuka::on_picture_radioButton_clicked()
{
	ui->picture_actions_groupBox->setEnabled(true);

	ui->text_actions_groupBox->setEnabled(false);
	ui->svg_actions_groupBox->setEnabled(false);
	ui->sketch_actions_groupBox->setEnabled(false);
}

void CobotKuka::on_sketch_radioButton_clicked()
{
	ui->sketch_actions_groupBox->setEnabled(true);

	ui->text_actions_groupBox->setEnabled(false);
	ui->picture_actions_groupBox->setEnabled(false);
	ui->svg_actions_groupBox->setEnabled(false);
}


/* Picture mode selection */
void CobotKuka::on_picture_webcam_radioButton_clicked()
{
	ui->picture_webcam_pushButton->setEnabled(true);

	ui->picture_file_lineEdit->setEnabled(false);
	ui->picture_file_pushButton->setEnabled(false);

}

void CobotKuka::on_picture_file_radioButton_clicked()
{
	ui->picture_webcam_pushButton->setEnabled(false);

	ui->picture_file_lineEdit->setEnabled(true);
	ui->picture_file_pushButton->setEnabled(true);
}

void CobotKuka::on_svg_file_pushButton_clicked()
{
	//create and config the file dialog
	filedialog = new QFileDialog(this,"Choose a SVG file", QDir::toNativeSeparators(QDir::homePath()), tr("SVG Image(*.svg)"));
	filedialog->setFileMode(QFileDialog::ExistingFile);
	filedialog->setViewMode(QFileDialog::Detail);
	filedialog->setModal(true);

	//show it. return 1 if OK and 0 otherwise.
	int res = filedialog->exec();
	if(res){
		QString path = filedialog->selectedFiles().takeFirst();
		//TODO : get this working
		svgrenderer = new QSvgRenderer(this);
		svgrenderer->load(path);
		ui->svg_file_lineEdit->setText(path);

		/*TODO : traitement pour extraire les parametres du svg */

		ui->send_pushButton->setEnabled(true);
	}



}

void CobotKuka::on_ok_pushButton_clicked()
{
	if(ui->svg_radioButton->isChecked()){
		qDebug() << "svg button is checked";
		/* TODO : traitement pour extraire points d'un svg */
	}
	else if(ui->text_radioButton->isChecked()){
		qDebug() << "text button is checked";
		/* TODO : traitement pour extraire points d'un texte */
	}
	else if(ui->picture_radioButton->isChecked()){
		qDebug() << "picture button is checked";
		/* TODO : traitement pour extraire contour et points d'une image */
	}
	else if(ui->sketch_radioButton->isChecked()){
		qDebug() << "sketch button is checked";
		/* TODO : traitement pour extraire points d'un dessin en temps reel */
	}
	else{
		qDebug() << "no radio button checked";
	}

	ui->send_pushButton->setEnabled(true);
	ui->send_pushButton->setStyleSheet("color:green");
}

void CobotKuka::on_send_pushButton_clicked()
{
	static int i = 0;
	if(i == 0){

	/* TODO : procedure to send the data to the server */
	tcpSocket->write(QtJson::serialize(QtJson::parse("{\"svg\":[{\"x\":1,\"y\":2},{\"x\":3,\"y\":4},{\"x\":5,\"y\":6}]}")));
	qDebug() << "writted : " << QtJson::serialize(QtJson::parse("{\"svg\":[{\"x\":1,\"y\":2},{\"x\":3,\"y\":4},{\"x\":5,\"y\":6}]}"));
	i++;
	}
	else{
		tcpSocket->write(QtJson::serialize(QtJson::parse("{\"svg\":[{\"x\":10,\"y\":11}]}")));
		i=0;

		qDebug() << "writted : " << QtJson::serialize(QtJson::parse("{\"svg\":[{\"x\":10,\"y\":11}]}"));
		ui->send_pushButton->setEnabled(false);
		ui->send_pushButton->setStyleSheet("color:grey");
	}
	//QString test = "svg svg svg svg";
	//QByteArray aaa = QtJson::serialize(test);
//	tcpSocket->write(test.toUtf8());
	tcpSocket->flush();
	//qDebug(aaa);

}

void CobotKuka::on_picture_file_pushButton_clicked()
{
	//create and config the file dialog
	filedialog = new QFileDialog(this,"Choose an Image", QDir::toNativeSeparators(QDir::homePath()), tr("Image(*.jpg, *.jpeg, *.png)"));
	filedialog->setFileMode(QFileDialog::ExistingFile);
	filedialog->setViewMode(QFileDialog::Detail);
	filedialog->setModal(true);

	//show it. return 1 if OK and 0 otherwise.
	int res = filedialog->exec();
	if(res){
		/*TODO : Include code to open the file and extract the drawing*/
	}
}

void CobotKuka::on_actionQuit_triggered()
{
	//close the program
	this->close();
}

void CobotKuka::serverConnected(){
	qDebug() << "client connected !" << tcpSocket->peerAddress() << ":" << tcpSocket->peerPort() ;
	connected = true;
	ui->connect_status_label->setText("Connected !");
	ui->connect_status_label->setStyleSheet("QLabel { color : green; }");
	ui->connect_pushButton->setText("Disconnect");
	ui->connect_pushButton->setDefault(false);

	ui->picture_groupBox->setEnabled(true);
	ui->text_groupBox->setEnabled(true);
	ui->svg_groupBox->setEnabled(true);
	ui->sketch_groupBox->setEnabled(true);

	//can't change the socket while connected
	ui->connect_groupBox->setEnabled(false);
}

void CobotKuka::serverDisconnected(){
	qDebug() << "Client disconnected from server" << tcpSocket->peerAddress() << ":" << tcpSocket->peerPort();
	tcpSocket->close();
	qDebug() << "socket closed";

	connected = false;

	ui->connect_pushButton->setText("Connect");
	ui->connect_status_label->setText("Disconnected !");
	ui->connect_status_label->setStyleSheet("QLabel { color : red; }");
	ui->connect_pushButton->setDefault(true);
	ui->connect_groupBox->setEnabled(true);

	ui->picture_groupBox->setEnabled(false);
	ui->text_groupBox->setEnabled(false);
	ui->svg_groupBox->setEnabled(false);
	ui->sketch_groupBox->setEnabled(false);

	ui->send_pushButton->setEnabled(false);
}


void CobotKuka::displayError(QAbstractSocket::SocketError socketError)
{
	switch (socketError) {
	case QAbstractSocket::RemoteHostClosedError:
		break;
	case QAbstractSocket::HostNotFoundError:
		QMessageBox::information(this, tr("Socket error"), tr("The host was not found. Please check the host name and port settings."));
		break;
	case QAbstractSocket::ConnectionRefusedError:
		QMessageBox::information(this, tr("Socket error"), tr("The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct."));
		break;
	default:
		QMessageBox::information(this, tr("Socket error"),
								 tr("The following error occurred: %1.").arg(tcpSocket->errorString()));
	}
}

void CobotKuka::datawritten(qint64 w){
	qDebug() << "Data written to the server. number of bytes written : " << w;
}

void CobotKuka::on_CobotKuka_destroyed()
{
	tcpSocket->disconnectFromHost();
}
