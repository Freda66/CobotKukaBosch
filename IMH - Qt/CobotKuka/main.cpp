#include "cobotkuka.h"
//#include <opencv2/opencv.hpp>
//#include <QDebug>
#include <QApplication>

//using namespace cv;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CobotKuka w;
	w.show();


//	VideoCapture cap(0); // open the default camera
//	  if(!cap.isOpened())  // check if we succeeded
//	  {
//		  qDebug() <<"cam not opened";
//		  return 0;
//	  }
//	  namedWindow("edges",1);
//	  for(;;)
//	  {
//		  Mat frame;
//		  cap >> frame;
//		  imshow("edges", frame);
//		  if(waitKey(30) >= 0) break;
//	  }




	return a.exec();
}
