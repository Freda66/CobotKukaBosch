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
    Mat frame;
    Mat grey;
    Mat cannye;
    Mat drawing; //image avec contours
    RNG rng(12345); //Random Number Generator
    int height=300;
    int width=400;

    vector<vector<Point> > contours; //Définition d'un vecteur de points
    vector<Vec4i> hierarchy;


    VideoCapture cap(0); // open the default camera
       if(!cap.isOpened())
           {
           // check if we succeeded
            return -1;
       }

       namedWindow("edges",1);


       for(;;)
       {


           cap >> frame; // get a new frame from camera
           cvtColor(frame, grey, CV_BGR2GRAY);
           //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);

           //Appliquer un « blurring » pour enlever le bruit
            blur(grey, grey, Size(3,3));
                       //imshow("Blure", grey);

            //Appliquer la fonction Canny.
            Canny(grey, cannye, 100, 100, 3);
            imshow("Canny",cannye);


           //waitKey();

           //imshow("edges", edges);
           if(waitKey(30) >= 0) break;
        }

       //Appliquer la fonction « fondContours ».
       findContours(cannye, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0)); //peut être rajouter le paramètre  Point(0, 0)

       // Draw contours
       drawing = Mat::zeros(cannye.size(), CV_8UC3);

       for(int i = 0; i< contours.size(); i++)
       {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point()); //peut être rajouter le paramètre  Point()
       }

        namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
        imshow("Contours", drawing);

        //Afficher les valeurs du vecteur contours
               // for (int i = 0; i < contours.size(); ++i) {
                 //  Point *data = contours.data();
                  // qDebug() << data[i] << " ; " ;
               // }

        QString courbe="";

        for(int i = 0; i < contours.size(); ++i) {

            vector<Point>  res=contours.at(i);
            courbe="";
            courbe+="[";
            QString coordonnees="";
           for(int j = 0; j < res.size(); ++j){

               Point pts = res.at(j);
                   coordonnees+=QString::number(pts.x);
                   coordonnees+=",";
                   coordonnees+= QString::number(pts.y);
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
        courbe+="400";
        courbe+=",";
        courbe+="300";
        courbe+="],\"webcam\":[";
        courbe+=courbetmp;
        courbe+="]}";


        qDebug() << courbe;


        //Format de renvoie de la courbe: {"webcam" : [ [x1,y1,x2,y2,...], [x1,y1,x2,y2,...], [x1,y1,x2,y2,...], ...]}
        //                                              courbe1             courbe2             courbe3

 return a.exec();
}


