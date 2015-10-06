#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include <QApplication>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;


int main( int argc, char** argv )
{
    QApplication a(argc, argv);
    Mat frame; //image caméra sans filtre
    Mat grey; //image avec filtre gris
    Mat cannye; //image avec filtre cannye
    Mat drawing; //image avec contours
    RNG rng(12345); //Random Number Generator

    vector<vector<Point> > contours; //Définition d'un vecteur de points
    vector<Vec4i> hierarchy;




    VideoCapture cap(0); // Ouvrir la caméra par défaut
       if(!cap.isOpened())
           {
           // check if we succeeded
            return -1;
       }



       for(;;)
       {


           cap >> frame; // get a new frame from camera
           cvtColor(frame, grey, CV_BGR2GRAY);


           //Appliquer un blurring pour enlever le bruit
            blur(grey, grey, Size(3,3));


            //Appliquer la fonction Canny.
            Canny(grey, cannye, 100, 100, 3);
            imshow("Canny",cannye);
            resizeWindow("Canny", 635, 475);



           if(waitKey(30) >= 0) break;
        }

       //Appliquer la fonction « fondContours ».
       findContours(cannye, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0)); //Récupérer les points des contours

       // Draw contours
       drawing = Mat::zeros(cannye.size(), CV_8UC3);

       for(int i = 0; i< contours.size(); i++)
       {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255)); //ajouter couleur pour chaque courbe
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point()); //Fonction pour dessiner les contours ou courbes
       }

        namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
        imshow("Contours", drawing);
        resizeWindow("Contours", 635, 475); //redimensionner la fenêtre



//Format de renvoie des données des courbes: { "pt" : [taille x,taille y], "webcam" : [ [x1,y1,x2,y2,...], [x1,y1,x2,y2,...], [x1,y1,x2,y2,...], ...]}
                                            //                                              courbe1             courbe2             courbe3

        QString courbe="";

        for(int i = 0; i < contours.size(); ++i) { //Boucler pour chaque Courbe

            vector<Point>  res=contours.at(i);
            courbe="";

            QString coordonnees="";
            courbe+="[";
            for(int j = 0; j < res.size(); ++j){ //Récupérer les coordonnées des points
               Point pts = res.at(j);
                   coordonnees+=QString::number(pts.x); //Récupérer coordonnée X d'un point dans le vecteur Point
                   coordonnees+=",";
                   coordonnees+= QString::number(pts.y); //Récupérer coordonnée Y d'un point dans le vecteur Point
                   //***
                   if(j==res.size()-1)
                    {}
                   else
                       coordonnees+=",";

               //qDebug() << "C" << i << "/" << "X" << j << ": " << pts.x;
               //qDebug() << "C" << i << "/" << "Y" << j << ": " << pts.y;



           }
           courbe+=coordonnees.toUtf8();
             courbe+="]";
           if(i==contours.size()-1)
            {}
           else
               coordonnees+=",";
           courbe=courbe.replace("\n","");
           courbe=courbe.replace("\r","");
           courbe=courbe.replace("\t","");

        }
        QString courbetmp=courbe;

        courbe="{\"pt\":[";
        courbe+="635";
        courbe+=",";
        courbe+="475";
        courbe+="],\"webcam\":[";
        courbe+=courbetmp;
        courbe+="]}";


        qDebug() << courbe; //Renvoyer les données en format type Json


 return a.exec();
}
