#include "mainwindow.h"
#include <QApplication>
#include <QtXml>
#include <iostream>
#include <QDebug>

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QString json="";
	//MainWindow w;
	//w.show();

	QDomDocument *dom = new QDomDocument("MonDom");


    QFile svg_doc("/Users/Mustapha/Qt/Projets/lireSvg/pirate.svg");


	if (!svg_doc.open(QIODevice::ReadOnly)) {
		qDebug("false");
	}


	dom->setContent(&svg_doc);


	QDomElement dom_element = dom->documentElement();
	QDomNode node = dom_element.firstChild(); //balise d
	node = node.firstChild();
	QString ligne= "";

    qDebug() << dom->toString();

      QRegExp rxe("(width=\"\\d.+(pt)\").+(height=\"\\d.+(pt)\")");
      int pos1 = 0;
      if((pos1 = rxe.indexIn(dom->toString(), pos1)) != -1)
      {
        qDebug() <<"ee"+ (QString)rxe.cap(1);
        qDebug() <<"ee"+ (QString)rxe.cap(3);
      }


    /***/
        QRegExp rx("(<g.+\n.+)\>");

         QStringList list;
         int pos = 0;
         QString translate_property="";
         QString scale_property="";
         if((pos = rx.indexIn(dom->toString(), pos)) != -1)
         {
            QString nn=(QString)rx.cap(1);
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
                    scale_property+="{\"scale\":["+query.at(1)+","+query.at(2)+"]";
                }
            }
         }
        /***/

    qint32 index=1 ;
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

                //qDebug() << str;

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
    if(translate_property!="")
    {
        json= translate_property+","+json;
    }

    if(scale_property!="")
    {
        json= scale_property+","+json;
    }
    //json= translate_property+","+scale_property+","+json;
    if(!json.startsWith("{"))
    {
        json="{"+json;
    }
    qDebug() <<json +"\n\r";

	return 0;
}
