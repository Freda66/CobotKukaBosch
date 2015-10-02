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

    else {
    }


    dom->setContent(&svg_doc);

    QDomElement dom_element = dom->documentElement();
    QDomNode node = dom_element.firstChild(); //balise d
    node = node.firstChild();
    QString ligne= "";



    while(!node.isNull()){ //vérifier si un noeud Path est dans le fichier

        QStringList charList = node.attributes().item(0).toAttr().value().split(' ');


        int nb = charList.count(); //compter le nombre d'élément dans la liste

        for(int j=0; j < nb; j++){ //faire une boucle pour parcourir la liste
            ligne+=" "+charList.at(j).toLatin1().replace("\n","");

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
                      jsonelement+="]";
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

        qDebug() <<json +"\n\r";
        node = node.nextSibling(); //Aller au Path suivant

    }

    return 0;
}
