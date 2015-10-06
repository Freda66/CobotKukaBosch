#include "cobotkuka.h"
#include "ui_cobotkuka.h"




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


/* Setting Font */
void CobotKuka::on_text_font_fontComboBox_currentFontChanged(const QFont &f)
{
	ui->text_text_lineEdit->setFont(f);
}

/* Mod Selection*/
void CobotKuka::on_svg_radioButton_clicked()
{
	ui->svg_actions_groupBox->setEnabled(true);

	change_Action_Group_Color();

	ui->text_actions_groupBox->setEnabled(false);
	ui->picture_actions_groupBox->setEnabled(false);
	ui->sketch_actions_groupBox->setEnabled(false);
}

void CobotKuka::on_text_radioButton_clicked()
{
	ui->text_actions_groupBox->setEnabled(true);

	change_Action_Group_Color();

	ui->svg_actions_groupBox->setEnabled(false);
	ui->picture_actions_groupBox->setEnabled(false);
	ui->sketch_actions_groupBox->setEnabled(false);
}

void CobotKuka::on_picture_radioButton_clicked()
{
	ui->picture_actions_groupBox->setEnabled(true);

	change_Action_Group_Color();

	ui->text_actions_groupBox->setEnabled(false);
	ui->svg_actions_groupBox->setEnabled(false);
	ui->sketch_actions_groupBox->setEnabled(false);
}

void CobotKuka::on_sketch_radioButton_clicked()
{
	ui->sketch_actions_groupBox->setEnabled(true);

	change_Action_Group_Color();

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
		//recupere le path du fichier
		svgFile = filedialog->selectedFiles().takeFirst();
		ui->svg_file_lineEdit->setText(svgFile);

		//une fois le json generé, on peut l'envoyer.
		activate_OK_pushButton();
	}
}

void CobotKuka::on_ok_pushButton_clicked()
{
	if(ui->svg_radioButton->isChecked()){
		qDebug() << "svg button is checked";
		//decode le fichier pour obtenir le json
		getJsonFromSvg(svgFile);
	}
	else if(ui->text_radioButton->isChecked()){
		qDebug() << "text button is checked";
		/* TODO : traitement pour extraire points d'un texte */
	}
	else if(ui->picture_radioButton->isChecked()){
		qDebug() << "picture button is checked";
		getJsonFromWebcam();
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
	// lettre A
	//QString tst = "[{\"x\":0,\"y\":0},{\"x\":20,\"y\":40},{\"x\":40,\"y\":0}]";
	//QString tst2 = "[{\"x\":10,\"y\":20},{\"x\":30,\"y\":20}]";

	//
	//QString tst = "[{\"x\":150,\"y\":150},{\"x\":110,\"y\":150},{\"x\":110,\"y\":110},{\"x\":150,\"y\":110}]";
	//QString tst2 = "[{\"x\":110,\"y\":110},{\"x\":110,\"y\":70},{\"x\":150,\"y\":70}]";
	//jsonChainList.append(tst);
	//jsonChainList.append(tst2);

	//creation de la socket et connection au serveur
	if(!connected) {

		/* CODE A DECOMMENTER POUR TEST HORS LIGNE */
		writeJSONToServer(jsonChainList);
		/* /CODE A DECOMMENTER POUR TEST HORS LIGNE*/
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
	/*QString aaa = ui->text_font_fontComboBox->currentText();
	QFont currentFont = ui->text_font_fontComboBox->currentFont();
	DWORD outline = GetGlyphOutlineW(this, (uint)aaa.at(0), GGO_BEZIER, );
	*/
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

int CobotKuka::on_picture_webcam_pushButton_clicked()
{

	activate_OK_pushButton();
}

void CobotKuka::on_sketch_pushButton_clicked()
{
	activate_OK_pushButton();
}

void CobotKuka::writeJSONToServer(const QStringList& jsonList){



	//allow to test if the element read is the first of the list, to know if a ',' is needed. Can't use the first() method in the foreach because of the pop_front() method : every iteration, the first element changes
	QString last = jsonList.last();


	qDebug() << "str -> " << jsonList;
	//finalchain += "}";
	//qDebug() << "finalchain => " << finalchain;

}


void CobotKuka::getJsonFromSvg(QString svgpath){

	/* DEBUG */
	//QString finalchain = "{";
	//to verify if the parameters have been read
	bool translate = false;
	bool scale = false;
	bool height = false;
	bool width = false;


	QString json="";

	QDomDocument *dom = new QDomDocument("MonDom");

	QFile svg_doc(svgpath);
	dom->setContent(&svg_doc);

	QDomElement dom_element = dom->documentElement();
	QDomNode node = dom_element.firstChild(); //balise d
	node = node.firstChild();
	QString ligne= "";


	/***/
		QRegExp rxParam("(<g.+\n.+)\>"); //extrait les parametres dans la balise <g>
		//QRegExp rxProp("(width=\"\\d.+(pt)\").+(height=\"\\d.+(pt)\")"); //extrait les proprietes dans la balise <svg>
		QRegExp rxe("(width=\"\\d.+(pt)\").+(height=\"\\d.+(pt)\")");


		 QStringList list;
		 int pos = 0;
		 QString translate_property="";
		 QString scale_property="";
		 QString height_property="";
		 QString width_property="";



		 //Recupere les parametres de la balise <g>
		 if((pos = rxParam.indexIn(dom->toString(), pos)) != -1)
		 {
			QString nn=(QString)rxParam.cap(1);
			QStringList charList =nn.split(' ');
			QString chaineq="";
			QRegExp rxproperties("(\\(|\\,|\\))"); //RegEx for ' ' or ',' or '.' or ':' or '\t'

			int nb = charList.count(); //compter le nombre d'élément dans la liste
			for(int j=0; j < nb; j++){ //faire une boucle pour parcourir la liste
			QString chaine =charList.at(j).toLatin1().replace("\n"," ");
				if(chaine.contains("translate") >= 1)
				{
					QStringList query = chaine.split(rxproperties);
					translate_property+="\"translate\":["+query.at(1)+","+query.at(2)+"]";
				}else if(chaine.contains("scale") >= 1)
				{
					QStringList query = chaine.split(rxproperties);
					scale_property+="\"scale\":["+query.at(1)+","+query.at(2)+"]";
				}
			}
		 }
		/***/

		 /*WARNING : OUVRIR LE FICHIER AVEC LE BOUTON OUVRIR ET PAS AVEC UN DOUBLE CLIC, SINON LA CHAINE N'EST PAS DETECTEE ! */
		 /* TODO : FORMATTER LA CHAINE DETECTEE POUR CORRESPONDRE AU FORMAT ATTENDU */
		 //recupere les proprietes dans la balise <svg>
		 int pos1 = 0;
		 if((pos1 = rxe.indexIn(dom->toString(), pos1)) != -1)
		 {
			 qDebug() << "height and width detected !";
			QString nn=(QString)rxe.cap(1) + " " + (QString)rxe.cap(3);
			QStringList charList =nn.split(' ');
			QString chaineq="";
			QRegExp rxproperties("(\\(|\\,|\\))"); //RegEx for ' ' or ',' or '.' or ':' or '\t'

			int nb = charList.count(); //compter le nombre d'élément dans la liste
			for(int j=0; j < nb; j++){ //faire une boucle pour parcourir la liste
			QString chaine =charList.at(j).toLatin1().replace("\n"," ");
				if(chaine.contains("height") >= 1){
					QStringList query = chaine.split(rxproperties);
					height_property+="\"height\":"+query.at(1);
				}
				else if(chaine.contains("width") >= 1){
					QStringList query = chaine.split(rxproperties);
					width_property+="\"width\":"+query.at(1);
				}
			}
		 }



	while(!node.isNull()){ //vérifier si un noeud Path est dans le fichier

		QStringList charList = node.attributes().item(0).toAttr().value().split(' ');


		ligne = "";

		int nb = charList.count(); //compter le nombre d'élément dans la liste

		for(int j=0; j < nb; j++){ //faire une boucle pour parcourir la liste
			ligne+=" "+charList.at(j).toLatin1().replace("\n"," ");

		}

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
					   jsonelement+=",m:["+QString("%1").arg(str.right(str.size()-1));
				   }
				   else if(str.startsWith("c"))
				   {
					   jsonelement+="],c:["+QString("%1").arg(str.right(str.size()-1));
						jsonelement+=",";
				   }
				   else if(str.startsWith("l"))
				   {
					   jsonelement+="],l:["+QString("%1").arg(str.right(str.size()-1));
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
		json=json.replace(",]","]");
		json=json.replace("M","{\"M\"");
		json=json.replace("c","\"c\"");
		json=json.replace("l","\"l\"");
		json=json.replace("m","\"m\"");
		json=json.replace("\n\"","");

		/*if(translate_property!="")
		{
			json= translate_property;
		} */

		node = node.nextSibling(); //Aller au Path suivant

	}
	if(translate_property!="")
	{
		//json= translate_property+","+json;
		jsonChainList.prepend(translate_property);
	}

	if(scale_property!="")
	{
		//json= scale_property+","+json;
		jsonChainList.prepend(scale_property);
	}
	if(width_property!=""){
		jsonChainList.prepend(width_property);
	}
	if(height_property!=""){
		jsonChainList.prepend(height_property);
	}


	if(json.contains("translate")) translate = true;
	if(json.contains("scale")) scale = true;
	if(json.contains("height")) {height = true; qDebug() << "height";}
	if(json.contains("width")) {width = true; qDebug() << "width";}




	if(scale && translate && height && width) {
		json += "\"svg\":";
		scale = translate = height = width = false;
	}

	//json= translate_property+","+scale_property+","+json;
	json="{"+json;
	qDebug() <<json +"\n\r";

		//TODO : expression reguliere et methode pour corriger chaine.
		//QRegExp("(/d)-(/d)")

		//ajoute le json à la liste des vecteurs
		jsonChainList.append(json);
		node = node.nextSibling(); //Aller au Path suivant

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


QString CobotKuka::getJsonFromWebcam(){





//	rng(12345);

//   //Appliquer la fonction « fondContours ».
//   findContours(cannye, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0)); //Récupérer les points des contours

//   // Draw contours
//   drawing = Mat::zeros(cannye.size(), CV_8UC3);

//   for(int i = 0; i< contours.size(); i++)
//   {
//	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255)); //ajouter couleur pour chaque courbe
//	drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point()); //Fonction pour dessiner les contours ou courbes
//   }

//	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
//	imshow("Contours", drawing);
//	resizeWindow("Contours", 635, 475); //redimensionner la fenêtre



////Format de renvoie des données des courbes: { "pt" : [taille x,taille y], "webcam" : [ [x1,y1,x2,y2,...], [x1,y1,x2,y2,...], [x1,y1,x2,y2,...], ...]}
//											//                                              courbe1             courbe2             courbe3

//		QString courbe="";

//		for(int i = 0; i < contours.size(); ++i) { //Boucler pour chaque Courbe

//			vector<Point>  res=contours.at(i);
//			courbe="";

//			QString coordonnees="";
//			courbe+="[";
//			for(int j = 0; j < res.size(); ++j){ //Récupérer les coordonnées des points
//			   Point pts = res.at(j);
//				   coordonnees+=QString::number(pts.x); //Récupérer coordonnée X d'un point dans le vecteur Point
//				   coordonnees+=",";
//				   coordonnees+= QString::number(pts.y); //Récupérer coordonnée Y d'un point dans le vecteur Point
//				   //***
//				   if(j==res.size()-1)
//					{}
//				   else
//					   coordonnees+=",";

//			   //qDebug() << "C" << i << "/" << "X" << j << ": " << pts.x;
//			   //qDebug() << "C" << i << "/" << "Y" << j << ": " << pts.y;



//		   }
//		   courbe+=coordonnees.toUtf8();
//			 courbe+="]";
//		   if(i==contours.size()-1)
//			{}
//		   else
//			   coordonnees+=",";
//		   courbe=courbe.replace("\n","");
//		   courbe=courbe.replace("\r","");
//		   courbe=courbe.replace("\t","");

//		}
//		QString courbetmp=courbe;

//		courbe="{\"pt\":[";
//		courbe+="635";
//		courbe+=",";
//		courbe+="475";
//		courbe+="],\"webcam\":[";
//		courbe+=courbetmp;
//		courbe+="]}";


//		qDebug() << courbe; //Renvoyer les données en format type Json
}