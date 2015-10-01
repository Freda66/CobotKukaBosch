#include "cobotkuka.h"
#include "ui_cobotkuka.h"



void CobotKuka::init(){
	//disable the actions while not connected
	ui->picture_groupBox->setEnabled(false);
	ui->text_groupBox->setEnabled(false);
	ui->svg_groupBox->setEnabled(false);
	ui->sketch_groupBox->setEnabled(false);

	ui->send_pushButton->setEnabled(false);

	//TODO : load TTF fonts exclusively

	ui->svg_actions_groupBox->setEnabled(true);

	ui->text_actions_groupBox->setEnabled(false);
	ui->picture_actions_groupBox->setEnabled(false);
	ui->sketch_actions_groupBox->setEnabled(false);

	//initialise the TCP Socket
	tcpSocket = new QTcpSocket(this);
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),				this, SLOT(displayError(QAbstractSocket::SocketError)));

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

void CobotKuka::on_connect_pushButton_clicked(bool checked)
{
	if(checked){
		/*
		 * TODO : Include code de connection au serveur
		 */

		if(ip.setAddress(ui->connect_ip_lineEdit->text())){
				tcpSocket->connectToHost(ip, ui->connect_port_lineEdit->text().toInt());
				/* TODO : voir web services avec qt (wsdl) */
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
		else {
			QMessageBox::warning(this,"Socket Error : IP Adress Invalid", "The IP Adress is invalid. \nIP Adress must be in the form of xxx.xxx.xxx.xxx with only digits and dots. example : 192.168.0.18 \nPlease enter a valid Adress and retry.", QMessageBox::Ok, QMessageBox::NoButton);

		}



	}
	else{
		/* TODO : Include code de deconnection au serveur*/
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

void CobotKuka::on_send_pushButton_clicked()
{
	/* TODO : procedure to send the data to the server */

	ui->send_pushButton->setEnabled(false);
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
	/*TODO : Include code to disconnect from the server */

	//close the program
	this->close();
}
