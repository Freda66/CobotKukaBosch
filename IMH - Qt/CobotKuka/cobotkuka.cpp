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

	desactivate_OK_pushButton();
	desactivate_Send_pushButton();
	desactivate_Stop_pushButton();

	//initialise the TCP Socket
	tcpSocket = new QTcpSocket(this);
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
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
	/* Code de connection au serveur déporté dans la foncton on_send_pushButton_clicked()*/

	ui->connect_status_label->setText("Connected !");
	ui->connect_status_label->setStyleSheet("QLabel { color : green; }");
	ui->connect_pushButton->setText("Disconnect");
	ui->connect_pushButton->setDefault(false);

	//once connected, enable the action buttons
	ui->picture_groupBox->setEnabled(true);
	ui->text_groupBox->setEnabled(true);
	ui->svg_groupBox->setEnabled(true);
	ui->sketch_groupBox->setEnabled(true);

	//can't change the socket while connected
	ui->connect_groupBox->setEnabled(false);

	activate_Stop_pushButton();
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
		svgFile = filedialog->selectedFiles().takeFirst();
		//TODO : get this working
		svgrenderer = new QSvgRenderer(this);
		svgrenderer->load(svgFile);
		ui->svg_file_lineEdit->setText(svgFile);

		activate_OK_pushButton();
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

	activate_Send_pushButton();
}

void CobotKuka::on_send_pushButton_clicked()
{
	/* DEBUG */
	QString tst = "[{\"x\":0,\"y\":0},{\"x\":20,\"y\":40},{\"x\":40,\"y\":0}]";
	QString tst2 = "[{\"x\":50,\"y\":50},{\"x\":80,\"y\":80},{\"x\":44,\"y\":12},{\"x\":48,\"y\":18}]";
	QString tst3 = "[{\"x\":99,\"y\":99},{\"x\":88,\"y\":55}]";
	jsonChainList.append(tst);
	jsonChainList.append(tst2);
	jsonChainList.append(tst3);

	//creation de la socket et connection au serveur
	if(!connected) {
		if(connectToServer()){
			//Once the connection is established, send the JSON chain to the server.
			writeJSONToServer(jsonChainList);
		}
	}
	else{
		//if the connection is already established, send the JSON chain to the server.
		writeJSONToServer(jsonChainList);
	}


}

bool CobotKuka::connectToServer(){
	if(ip.setAddress(ui->connect_ip_lineEdit->text())){
		qDebug() << "trying to connect to server " << ip << ":" << ui->connect_port_lineEdit->text().toInt();
			tcpSocket->connectToHost(ip, ui->connect_port_lineEdit->text().toInt());
			//wait for the connection to be established
			return tcpSocket->waitForConnected();
	}
	else {
		QMessageBox::warning(this,"Socket Error : IP Adress Invalid", "The IP Adress is invalid. \nIP Adress must be in the form of xxx.xxx.xxx.xxx with only digits and dots. example : 192.168.0.18 \nPlease enter a valid Adress and retry.", QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
}

bool CobotKuka::disconnectFromServer(){
	tcpSocket->disconnectFromHost();
	return tcpSocket->waitForDisconnected();
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
		//get the selected file path and put it in the LineEdit
		pictureFile = filedialog->selectedFiles().takeFirst();
		ui->picture_file_lineEdit->setText(pictureFile);

		activate_OK_pushButton();
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

}

void CobotKuka::serverDisconnected(){
	qDebug() << "Client disconnected from server" << tcpSocket->peerAddress() << ":" << tcpSocket->peerPort();
	tcpSocket->close();
	qDebug() << "socket closed";

	connected = false;

	//indicates the status and activate the lineEdits
	ui->connect_pushButton->setText("Connect");
	ui->connect_status_label->setText("Disconnected !");
	ui->connect_status_label->setStyleSheet("QLabel { color : red; }");
	ui->connect_pushButton->setDefault(true);
	ui->connect_groupBox->setEnabled(true);

	//if not connected, block the access to the actions
	ui->picture_groupBox->setEnabled(false);
	ui->text_groupBox->setEnabled(false);
	ui->svg_groupBox->setEnabled(false);
	ui->sketch_groupBox->setEnabled(false);

	desactivate_OK_pushButton();
	desactivate_Send_pushButton();
	desactivate_Stop_pushButton();
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
	disconnectFromServer();
}

void CobotKuka::on_text_text_lineEdit_editingFinished()
{
	activate_OK_pushButton();
}

void CobotKuka::activate_OK_pushButton(){
	ui->ok_pushButton->setEnabled(true);
	ui->ok_pushButton->setStyleSheet("color:blue");
}

void CobotKuka::desactivate_OK_pushButton(){
	ui->ok_pushButton->setEnabled(false);
	ui->ok_pushButton->setStyleSheet("color:grey");
}

void CobotKuka::activate_Send_pushButton(){
	ui->send_pushButton->setEnabled(true);
	ui->send_pushButton->setStyleSheet("color:brown; background-color: green");
}

void CobotKuka::desactivate_Send_pushButton(){
	ui->send_pushButton->setEnabled(false);
	ui->send_pushButton->setStyleSheet("color:grey; background-color: palegreen ");
}

void CobotKuka::activate_Stop_pushButton(){
	ui->stop_pushButton->setEnabled(true);
	ui->stop_pushButton->setStyleSheet("color:orange; background-color: red");
}

void CobotKuka::desactivate_Stop_pushButton(){
	ui->stop_pushButton->setEnabled(false);
	ui->stop_pushButton->setStyleSheet("color:sandybrown; background-color: lightpink");
}


void CobotKuka::on_stop_pushButton_clicked()
{
	QString stop = "stop";

	if(!connected) {
		if(connectToServer()){
			tcpSocket->write(stop.toUtf8());
		}
		else{
			qDebug() << "erreur connexion au serveur pour message STOP";
		}
	}
	else tcpSocket->write(stop.toUtf8());;

	qDebug() << "!!! STOP !!!";
}

void CobotKuka::on_picture_webcam_pushButton_clicked()
{
	/* TODO : code pour ouvrir une fenetre avec la webcam */
	activate_OK_pushButton();
}

void CobotKuka::on_sketch_pushButton_clicked()
{
	activate_OK_pushButton();
}

void CobotKuka::writeJSONToServer(const QStringList& jsonList){


	ui->stop_pushButton->setEnabled(true);
	/* coordonnes pour dessiner un A
	 * 0,0
	 * 20,40
	 * 40,0
	 *
	 * 10,20
	 * 30,20
	 */


//	QString svg;
//	static int i = 0;
//	if(i == 0){

//	/* TODO : procedure to send the data to the server */
//	svg = "{[{\"x\":0,\"y\":0},{\"x\":20,\"y\":40},{\"x\":40,\"y\":0}]}";
//	tcpSocket->write(QtJson::serialize(QtJson::parse(svg.toUtf8())));
//	qDebug() << "writted : " << QtJson::serialize(QtJson::parse(svg.toUtf8()));
//	i++;
//	}
//	else{
//		svg = "{[{\"x\":10,\"y\":20},{\"x\":30,\"y\":20}]}";
//		tcpSocket->write(QtJson::serialize(QtJson::parse(svg.toUtf8())));
//		i=0;

//		qDebug() << "writted : " << QtJson::serialize(QtJson::parse(svg.toUtf8()));
//		desactivate_Send_pushButton();
//	}

	/* DEBUG */
//	QString test = "svg svg svg svg";
//	tcpSocket->write(QtJson::serialize(QtJson::parse(test.toUtf8)));
//		qDebug() << "writted : " << QtJson::serialize(QtJson::parse(test"));

	/* DEBUG */
	QString finalchain = "{\"svg\":[";
	foreach (QString str, jsonList) {
		if(str != jsonList.first()) finalchain += ",";
		finalchain += str;
		qDebug() << "str -> " << str;
	}
	finalchain += "]}";
	qDebug() << "finalchain => " << finalchain;

	if(connected) {tcpSocket->write( QtJson::serialize(QtJson::parse(finalchain.toUtf8())));
	tcpSocket->flush();
	}
	else qDebug() << "Tentative d'ecriture du JSON sans connexion au serveur";

}
