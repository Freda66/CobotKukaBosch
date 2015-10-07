#include "cobotkuka.h"
#include "ui_cobotkuka.h"
#include "mainwindow.h"

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


/* ----------- UI ---------- */

/* INIT */
void CobotKuka::init(){

	//disable the actions while not connected
	ui->picture_groupBox->setEnabled(false);
	ui->text_groupBox->setEnabled(false);
	ui->svg_groupBox->setEnabled(false);
	ui->sketch_groupBox->setEnabled(false);

	//TODO : load TTF fonts exclusively

	ui->svg_actions_groupBox->setEnabled(false);

	ui->text_actions_groupBox->setEnabled(false);
	ui->picture_actions_groupBox->setEnabled(false);
	ui->sketch_actions_groupBox->setEnabled(false);

	//load the action groupboxes background colors
	ui->svg_groupBox->setStyleSheet("background: rgb(240, 255, 240)");
	ui->text_groupBox->setStyleSheet("background: rgb(255, 240, 255)");
	ui->picture_groupBox->setStyleSheet("background: rgb(240, 255, 255)");
	ui->sketch_groupBox->setStyleSheet("background: rgb(255, 255, 240)");

	//disable the buttons and set the correct stylesheet
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

/* Font */
void CobotKuka::on_text_font_fontComboBox_currentFontChanged(const QFont &f)
{
	ui->text_text_lineEdit->setFont(f);
}


/* PushButton */
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

void CobotKuka::change_Action_Group_Color(){
	if(ui->svg_radioButton->isChecked()){
		ui->svg_groupBox->setStyleSheet("background: rgb(232, 255, 232)");

		ui->text_groupBox->setStyleSheet("background: rgb(255, 240, 255)");
		ui->picture_groupBox->setStyleSheet("background: rgb(240, 255, 255)");
		ui->sketch_groupBox->setStyleSheet("background: rgb(255, 255, 240)");
	}
	else if(ui->text_radioButton->isChecked()){
		ui->text_groupBox->setStyleSheet("background: rgb(255, 220, 255)");

		ui->svg_groupBox->setStyleSheet("background: rgb(240, 255, 240)");
		ui->picture_groupBox->setStyleSheet("background: rgb(240, 255, 255)");
		ui->sketch_groupBox->setStyleSheet("background: rgb(255, 255, 240)");
	}
	else if(ui->picture_radioButton->isChecked()){
		ui->picture_groupBox->setStyleSheet("background: rgb(220, 255, 255)");

		ui->text_groupBox->setStyleSheet("background: rgb(255, 240, 255)");
		ui->svg_groupBox->setStyleSheet("background: rgb(240, 255, 240)");
		ui->sketch_groupBox->setStyleSheet("background: rgb(255, 255, 240)");
	}
	else if(ui->sketch_radioButton->isChecked()){
		ui->sketch_groupBox->setStyleSheet("background: rgb(255, 255, 220)");

		ui->text_groupBox->setStyleSheet("background: rgb(255, 240, 255)");
		ui->svg_groupBox->setStyleSheet("background: rgb(240, 255, 240)");
		ui->picture_groupBox->setStyleSheet("background: rgb(240, 255, 255)");
	}
	else{
		ui->svg_groupBox->setStyleSheet("background: rgb(240, 255, 240)");
		ui->text_groupBox->setStyleSheet("background: rgb(255, 240, 255)");
		ui->picture_groupBox->setStyleSheet("background: rgb(240, 255, 255)");
		ui->sketch_groupBox->setStyleSheet("background: rgb(255, 255, 240)");
	}
}

void CobotKuka::change_Action_Group_Activation(){
	if(ui->svg_radioButton->isChecked()){
		ui->svg_actions_groupBox->setEnabled(true);

		ui->text_actions_groupBox->setEnabled(false);
		ui->picture_actions_groupBox->setEnabled(false);
		ui->sketch_actions_groupBox->setEnabled(false);
	}
	else if(ui->text_radioButton->isChecked()){
		ui->text_actions_groupBox->setEnabled(true);

		ui->svg_actions_groupBox->setEnabled(false);
		ui->picture_actions_groupBox->setEnabled(false);
		ui->sketch_actions_groupBox->setEnabled(false);
	}
	else if(ui->picture_radioButton->isChecked()){
		ui->picture_actions_groupBox->setEnabled(true);

		ui->text_actions_groupBox->setEnabled(false);
		ui->svg_actions_groupBox->setEnabled(false);
		ui->sketch_actions_groupBox->setEnabled(false);

		if(ui->picture_webcam_radioButton->isChecked()){
			ui->picture_file_lineEdit->setEnabled(false);
			ui->picture_file_pushButton->setEnabled(false);

			ui->picture_webcam_pushButton->setEnabled(true);
		}
		else if(ui->picture_file_radioButton->isChecked()){

				ui->picture_file_lineEdit->setEnabled(true);
				ui->picture_file_pushButton->setEnabled(true);

				ui->picture_webcam_pushButton->setEnabled(false);
		}
		else{
			ui->picture_file_lineEdit->setEnabled(false);
			ui->picture_file_pushButton->setEnabled(false);

			ui->picture_webcam_pushButton->setEnabled(false);
		}
	}
	else if(ui->sketch_radioButton->isChecked()){
		ui->sketch_actions_groupBox->setEnabled(true);

		ui->text_actions_groupBox->setEnabled(false);
		ui->svg_actions_groupBox->setEnabled(false);
		ui->picture_actions_groupBox->setEnabled(false);
	}
	else{
		ui->svg_actions_groupBox->setEnabled(false);
		ui->text_actions_groupBox->setEnabled(false);
		ui->picture_actions_groupBox->setEnabled(false);
		ui->sketch_actions_groupBox->setEnabled(false);
	}
}



/* ---------- SLOTS ---------- */

/* RadioButton */
void CobotKuka::on_svg_radioButton_clicked()
{
	change_Action_Group_Activation();
	change_Action_Group_Color();
}

void CobotKuka::on_text_radioButton_clicked()
{
	change_Action_Group_Activation();
	change_Action_Group_Color();}

void CobotKuka::on_picture_radioButton_clicked()
{
	change_Action_Group_Activation();
	change_Action_Group_Color();
}

void CobotKuka::on_sketch_radioButton_clicked()
{
	change_Action_Group_Activation();
	change_Action_Group_Color();
}


// Picture mode selection
void CobotKuka::on_picture_webcam_radioButton_clicked()
{
	change_Action_Group_Activation();
}

void CobotKuka::on_picture_file_radioButton_clicked()
{
	change_Action_Group_Activation();
}


/* PushButton */
void CobotKuka::on_connect_pushButton_clicked(bool checked)
{
	/* Code de connection au serveur déporté dans la foncton on_send_pushButton_clicked()*/
	if(checked){
		ui->connect_status_label->setText("Connected !");
		ui->connect_status_label->setStyleSheet("QLabel { color : green; }");
		ui->connect_pushButton->setText("Disconnect");
		ui->connect_pushButton->setDefault(false);

		//once connected, enable the action buttons
		ui->picture_groupBox->setEnabled(true);
		ui->text_groupBox->setEnabled(true);
		ui->svg_groupBox->setEnabled(true);
		ui->sketch_groupBox->setEnabled(true);
		change_Action_Group_Activation();

		//can't change the socket while connected
		ui->connect_groupBox->setEnabled(false);

		//once connected, possibility to send the stop signal at any moment
		activate_Stop_pushButton();

		//the svg action groupbox is selected by default, setting the correct stylesheet
		change_Action_Group_Color();
	}
	else{
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

		//once deconnected, impossible to send anything
		desactivate_OK_pushButton();
		desactivate_Send_pushButton();
		desactivate_Stop_pushButton();
	}
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
		//recupere le path du fichier
		svgFile = filedialog->selectedFiles().takeFirst();
		ui->svg_file_lineEdit->setText(svgFile);

		//une fois le json generé, on peut l'envoyer.
		activate_OK_pushButton();
	}
}

void CobotKuka::on_picture_file_pushButton_clicked()
{
	//create and config the file dialog
	filedialog = new QFileDialog(this,"Choose an Image", QDir::toNativeSeparators(QDir::homePath()), tr("Image(*.jpg, *.jpeg, *.png)"));
	filedialog->setFileMode(QFileDialog::ExistingFile);
	filedialog->setViewMode(QFileDialog::Detail);
	//when open, only this window can be clicked in the application
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

int  CobotKuka::on_picture_webcam_pushButton_clicked()
{



//	VideoCapture cap(1); // Ouvrir la caméra par défaut
//	   if(!cap.isOpened())
//		   {
//		   // check if we succeeded
//			return -1;
//	   }


//	   for(;;)
//	   {
//		   cap >> frame; // get a new frame from camera
//	   cvtColor(frame, grey, CV_BGR2GRAY);


//   //Appliquer un blurring pour enlever le bruit
//			blur(grey, grey, Size(3,3));


//			//Appliquer la fonction Canny.
//			Canny(grey, cannye, 100, 100, 3);
//			imshow("Canny",cannye);
//			resizeWindow("Canny", 635, 475);


//		   if(waitKey(30) >= 0) break;
//		}

//	activate_OK_pushButton();
//	getJsonFromWebcam();
	return 0;
}

void CobotKuka::on_sketch_pushButton_clicked()
{
    MainWindow *testWindow = new MainWindow();
    testWindow->show();

	activate_OK_pushButton();

}

void CobotKuka::on_ok_pushButton_clicked()
{
	if(ui->svg_radioButton->isChecked()){
		qDebug() << "svg button is checked";
		json = getJsonFromSvg(svgFile);
	}
	else if(ui->text_radioButton->isChecked()){
		qDebug() << "text button is checked";
		json = getJsonFromText();
	}
	else if(ui->picture_radioButton->isChecked()){
		qDebug() << "picture button is checked";
		if(ui->picture_webcam_radioButton->isChecked()){
			qDebug() << "picture webcam button is checked";
			json = getJsonFromWebcam();
		}
		else if(ui->picture_file_radioButton->isChecked()){
			qDebug() << "picture file button is checked";
			json = getJsonFromPicture();
		}

	}
	else if(ui->sketch_radioButton->isChecked()){
		qDebug() << "sketch button is checked";
		json = getJsonFromSketch();
	}
	else{
		qDebug() << "no radio button checked";
	}

	activate_Send_pushButton();
}

void CobotKuka::on_send_pushButton_clicked()
{

	//creation de la socket et connection au serveur
	if(!connected) {

		/* CODE A COMMENTER POUR TEST HORS LIGNE*/

        if(connectToServer()){
            //Once the connection is established, send the JSON chain to the server.
            writeJSONToServer(json);
        }

		/* /CODE A COMMENTER POUR TEST HORS LIGNE*/

		// ---

		/* CODE A DECOMMENTER POUR TEST HORS LIGNE */

        //writeJSONToServer(json);

		/* /CODE A DECOMMENTER POUR TEST HORS LIGNE*/
	}
	else{
		//if the connection is already established, send the JSON chain to the server.
		writeJSONToServer(json);
	}


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


/* LineEdit */
void CobotKuka::on_text_text_lineEdit_editingFinished()
{
	/*QString aaa = ui->text_font_fontComboBox->currentText();
	QFont currentFont = ui->text_font_fontComboBox->currentFont();
	DWORD outline = GetGlyphOutlineW(this, (uint)aaa.at(0), GGO_BEZIER, );
	*/
	activate_OK_pushButton();
}


/* Exit */
void CobotKuka::on_actionQuit_triggered()
{
	//close the program
	this->close();
}

void CobotKuka::on_CobotKuka_destroyed()
{
	disconnectFromServer();
}


/* Reseau */
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


/* ---------- RESEAU ---------- */

bool CobotKuka::connectToServer(){
	if(ip.setAddress(ui->connect_ip_lineEdit->text())){
		qDebug() << "trying to connect to server " << ip << ":" << ui->connect_port_lineEdit->text().toInt();
			tcpSocket->connectToHost(ip, ui->connect_port_lineEdit->text().toInt());
			//wait 5s max for the connection to be established
			return tcpSocket->waitForConnected(3000);
	}
	else {
		QMessageBox::warning(this,"Socket Error : IP Adress Invalid", "The IP Adress is invalid. \nIP Adress must be in the form of xxx.xxx.xxx.xxx with only digits and dots. example : 192.168.0.18 \nPlease enter a valid Adress and retry.", QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
}

bool CobotKuka::disconnectFromServer(){
	tcpSocket->disconnectFromHost();
	//wait 3s max for the connection to be close
	return tcpSocket->waitForDisconnected(3000);
}

void CobotKuka::datawritten(qint64 w){
	qDebug() << "Data written to the server. number of bytes written : " << w;
}

/* ---------- JSON ---------- */

//From SVG
QString CobotKuka::getJsonFromSvg(QString svgpath){


	QString finalchain = "{"; //String qui contiendra la chaine finale envoyée au serveur

	QString json=""; //contiendra les valeurs du json extrait du xml

	QDomDocument *dom = new QDomDocument("MonDom");

	QFile svg_doc(svgpath);
	dom->setContent(&svg_doc);

	QDomElement dom_element = dom->documentElement();
	QDomNode node = dom_element.firstChild(); //balise d
	node = node.firstChild();
	QString ligne= "";

	qDebug() << dom->toString();
	/***/
	QRegExp rxParam("(<g.+\n.+)\>"); //extrait les parametres dans la balise <g>
	//QRegExp rxProp("(width=\"\\d.+(pt)\").+(height=\"\\d.+(pt)\")");
	QRegExp rxe("(width=\"\\d.+(pt)\").+(height=\"\\d.+(pt)\")"); //extrait les proprietes height et width dans la balise <svg>

	QRegExp rxproperties("(\\(|\\,|\\))"); //RegEx for ' ' or ',' or '.' or ':' or '\t'

	 int pos = 0;
	 QString translate_property=""; //contiendra la valeur de la translation
	 QString scale_property=""; //contiendra la valeur du scale
	 QString size_property=""; //contiendra la taille du dessin



	 //Recupere les parametres de la balise <g>
	 if((pos = rxParam.indexIn(dom->toString(), pos)) != -1)
	 {
		QString nn=(QString)rxParam.cap(1);
		QStringList charList =nn.split(' ');


		int nb = charList.count(); //compter le nombre d'élément dans la liste
		for(int j=0; j < nb; j++){ //faire une boucle pour parcourir la liste
		QString chaine =charList.at(j).toLatin1().replace("\n"," ");
		QStringList queryList = chaine.split(rxproperties);
			if(chaine.contains("translate") >= 1)
			{
				translate_property+="\"translate\":["+queryList.at(1)+","+queryList.at(2)+"]";
			}
			else if(chaine.contains("scale") >= 1)
			{
				scale_property+="\"scale\":["+queryList.at(1)+","+queryList.at(2)+"]";
			}
		}
	 }
	/***/

	 /*WARNING : OUVRIR LE FICHIER AVEC LE BOUTON OUVRIR ET PAS AVEC UN DOUBLE CLIC, SINON LA CHAINE N'EST PAS DETECTEE ! */

	 //recupere les proprietes dans la balise <svg>
	 int pos1 = 0;
	 if((pos1 = rxe.indexIn(dom->toString(), pos1)) != -1)
	 {
		QString nn=(QString)rxe.cap(1) + " " + (QString)rxe.cap(3); //extrait la width et la height des captures du regex
		QStringList charList =nn.split(' '); //creer une liste contenant chaque capture

		int nb = charList.count(); //compter le nombre d'élément dans la liste

		for(int j=0; j < nb; j++){ //faire une boucle pour parcourir la liste
			QString chaine =charList.at(j).toLatin1().replace("\n"," ");//recupere l'element et nettoie les sauts de ligne

			if(chaine.contains("height") >= 1){ //si on recupere la hauteur

				chaine = chaine.replace("\"","");
				chaine = chaine.replace("height=","").replace(chaine.right(2),""); //on ne garde que le nombre utile


				chaine = chaine.replace(" ", ""); //on nettoie les espaces eventuels

				/**********/
				size_property.append(chaine + "]"); //on ajoute la valeur à la fin de la chaine. le format attendu est "pt":[width,heigh]
			}
			else if(chaine.contains("width") >= 1){

				chaine = chaine.replace("\"","");//on nettoie d'abord les guillemets pour avoir acces a l'unite.
				QString unite = chaine.right(2); //extrait l'unite de mesure (pt ou px)

				chaine = chaine.replace("width=","").replace(chaine.right(2),""); //on ne garde que la valeur utile
				chaine = chaine.replace(" ", ""); //on nettoie les espaces eventuels
				/**********/
				size_property.prepend("\"" + unite + "\":[" + chaine + ","); //on ajoute la valeur et l'unite au début de la chaine. le format attendu est   "pt":[width,heigh]
			}
		}
	 }

	 finalchain += translate_property + "," + scale_property +"," + size_property + ",\"svg\":{"; //ajoute les proprietes dans la chaine finale, avant de detecter les coordonnees.

QStringList charList ;
	while(!node.isNull()){ //vérifier si un noeud Path est dans le fichier

		charList += node.attributes().item(0).toAttr().value().split(' ');


		ligne = "";
		QString toto="";
		int nb = charList.count(); //compter le nombre d'élément dans la liste
		for(int j=0; j < nb; j++){ //faire une boucle pour parcourir la liste
			toto="";
			toto+=" "+charList.at(j).toLatin1();
			//qDebug() << toto;
			ligne+=toto;
		}

		ligne=ligne.replace("\n"," ");
		ligne=ligne.replace("\t","");
		ligne=ligne.replace("\r","");
		ligne=ligne.replace("l"," l");
		ligne=ligne.replace("c"," c");
		ligne=ligne.replace("L"," L");
		ligne=ligne.replace("C"," C");
		ligne=ligne.replace("-"," -");
		ligne=ligne.replace("[,","[");
		ligne=ligne.replace(",-","-");
		ligne=ligne.replace(","," ");

		qDebug() << ligne;
		// Création d'une liste avec chaque paramètre
		QStringList query = ligne.split(" ");

		json=+"";

		for (QStringList::iterator it = query.begin();it != query.end(); ++it) {
			int index = std::distance(query.begin(), it);

				QString str = *it;
				QString jsonelement="";
			   if(str!="")
			   {
				   if(str.startsWith("M"))
				   {
						jsonelement+="M:["+QString("%1").arg(str.right(str.size()-1));
						jsonelement+=",";
				   }
				   else if(str.startsWith("m"))
				   {
					  jsonelement+="],m:["+QString("%1").arg(str.right(str.size()-1));
					   jsonelement+=",";
				   }
				   else if(str.startsWith("c"))
				   {
					   jsonelement+="],c:["+QString("%1").arg(str.right(str.size()-1));
						 if(QString("%1").arg(str.right(str.size()-1))!="")
					   {
						   jsonelement+=",";
					   }
				   }
else if(str.startsWith("C"))
				   {
					   jsonelement+="],C:["+QString("%1").arg(str.right(str.size()-1));
						jsonelement+=",";
				   }
				   else if(str.startsWith("l"))
				   {
					   jsonelement+="],l:["+QString("%1").arg(str.right(str.size()-1));
if(QString("%1").arg(str.right(str.size()-1))!="")
					   {
						   jsonelement+=",";
					   }
				   }
				   else if(str.startsWith("L"))
				   {
					   jsonelement+="],L:["+QString("%1").arg(str.right(str.size()-1));
						jsonelement+=",";
				   }
				   else if (str.endsWith("z"))
				   {
					  jsonelement+= QString("%1").arg(str.left(str.size()-1));
					  jsonelement+= "]";
					  jsonelement+="}\n";
				   }
				   else
				   {
					   jsonelement+=QString("%1").arg(str);
					   jsonelement+=",";
				   }

				   json+=jsonelement;
			   }

		}

		json=json.replace("\n","");
		json=json.replace("\r","");
		json=json.replace("\t","");
		json=json.replace(",]","]");
		json=json.replace("M","\"M\"");
		json=json.replace(",{\"M\"","],\"M\"");
		json=json.replace("L","\"L\"");
		json=json.replace("C","\"C\"");
		json=json.replace("c","\"c\"");
		json=json.replace("l","\"l\"");
		json=json.replace("m","\"m\":");
		json=json.replace("\n\"","");
		json=json.replace("}{",",");
		json=json.replace("],{","],");
		json=json.replace("]}]","]");
		json=json.replace("}\"M\":","},\"M\":");
		json=json.replace("]},","],");
		json=json.replace(",\"M\"","],\"M\"");
		json=json.replace("]]","]");
		json=json.replace(":\":",":\"");
		json=json.replace("::",":");
		json=json.replace(":[,","[");
		json=json.replace("\"m\"[","\"m\":[");

		node = node.nextSiblingElement("path"); //Aller au Path suivant
	}

	finalchain.append(json); //ajoute le json à la liste des vecteurs
	finalchain.append("}");


	qDebug() << "finalchain: " << finalchain;

	return finalchain;
}

//From WebCam
QString CobotKuka::getJsonFromWebcam(){

//	Mat frame; //image caméra sans filtre
//	Mat grey; //image avec filtre gris
//	Mat cannye; //image avec filtre cannye
//	Mat drawing; //image avec contours
//	RNG rng(12345); //Random Number Generator

//	vector<vector<Point> > contours; //Définition d'un vecteur de points
//	vector<Vec4i> hierarchy;









//	VideoCapture cap(1); // open the default camera
//	  if(!cap.isOpened())  // check if we succeeded
//		  return "error";
///*
//	//  Mat edges;
//	  namedWindow("edges",1);
//	  for(;;)
//	  {
//		  Mat frame;
//		  cap >> frame;
//		  imshow("edges", frame);
//		  if(waitKey(30) >= 0) break;
//	  }


//*/









//	VideoCapture cap(1); // Ouvrir la caméra par défaut
//	   if(!cap.isOpened())
//		   {
//		   // check if we succeeded
//			return -1;
//	   }


//	   //for(;;)
//	   //{
//	   char key = 0;
//	   while(key !='q' || key !='Q'){

//		  // cap >> frame; // get a new frame from camera
//		   cap.read(frame);

//		   frame = cvQueryFrame(cap);

//		   cvtColor(frame, grey, CV_BGR2GRAY);


//		   //Appliquer un blurring pour enlever le bruit
//			blur(grey, grey, Size(3,3));


//			//Appliquer la fonction Canny.
//			Canny(grey, cannye, 100, 100, 3);
//			imshow("Canny",cannye);
//			resizeWindow("Canny", 635, 475);

//			key = cvWaitKey(30);
//}


		  // if(waitKey(30) >= 0) break;
		//}

	   //Appliquer la fonction « fondContours ».
//	   findContours(cannye, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0)); //Récupérer les points des contours

//	   // Draw contours
//	   drawing = Mat::zeros(cannye.size(), CV_8UC3);

//	   for(int i = 0; i< contours.size(); i++)
//	   {
//		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255)); //ajouter couleur pour chaque courbe
//		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point()); //Fonction pour dessiner les contours ou courbes
//	   }

//		namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
//		imshow("Contours", drawing);
//		resizeWindow("Contours", 635, 475); //redimensionner la fenêtre



////Format de renvoie des données des courbes: { "pt" : [taille x,taille y], "webcam" : [ [x1,y1,x2,y2,...], [x1,y1,x2,y2,...], [x1,y1,x2,y2,...], ...]}
//											//                                              courbe1             courbe2             courbe3

//		QString total = "[";

//		for(int i = 0; i < contours.size(); ++i) { //Boucler pour chaque Courbe
//			vector<Point>  res=contours.at(i);
//			QString coordonnees = "";
//			for(int j = 0; j < res.size(); ++j){ //Récupérer les coordonnées des points
//			   Point pts = res.at(j);
//				   coordonnees+=QString::number(pts.x); //Récupérer coordonnée X d'un point dans le vecteur Point
//				   coordonnees+=",";
//				   coordonnees+= QString::number(pts.y); //Récupérer coordonnée Y d'un point dans le vecteur Point
//				   //***
//				   if(j!=res.size()-1)
//					   coordonnees+=",";

//			   //qDebug() << "C" << i << "/" << "X" << j << ": " << pts.x;
//			   //qDebug() << "C" << i << "/" << "Y" << j << ": " << pts.y;



//		   }
//		   total += "[" + coordonnees.toUtf8() + "]";


//		   if(i!=contours.size()-1)
//			   total += ",";
//		}

//		total += "]";
//		QString tmp = total;

//		total = "{\"pt\":[";
//		total += "635";
//		total += ",";
//		total += "475";
//		total += "],\"webcam\":";
//		total += tmp;
//		total += "}";


//		qDebug() << total; //Renvoyer les données en format type Json
		return "";
}

/* ///////////////////////////////////////////////////////////////////////////////////// */







//From Picture
QString CobotKuka::getJsonFromPicture(){

	/* TODO : traitement pour extraire points d'une image fixe non SVG */
	return "picture";
}

//From Sketch
QString CobotKuka::getJsonFromSketch(){
	/* TODO : traitement pour extraire points d'un dessin en temps reel */
	return "sketch";
}

//From Text
QString CobotKuka::getJsonFromText(){
	/* TODO : traitement pour extraire points de contour d'un texte avec une police TureType */
	return "text";
}

//Write the JSON
void CobotKuka::writeJSONToServer(const QString& json){

	/* CODE A COMMENTER POUR TEST HORS LIGNE */
	if(connected) {
		tcpSocket->write((QByteArray)json.toUtf8());
		tcpSocket->flush();
	}
	else qDebug() << "Tentative d'ecriture du JSON sans connexion au serveur";
	/* /CODE A COMMENTER POUR TEST HORS LIGNE */

}
