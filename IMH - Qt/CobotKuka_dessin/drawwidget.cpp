#include "drawwidget.h"
#include "cobotkuka.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRgb>
#include "qdebug.h"

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    m_drawColor = QColor(Qt::black);
    jsonToSend="{\"pt\":["+QString::number(this->width())+","+QString::number(this->height())+"] ,\"dessin\":[";

    //connect(timer, )
}

DrawWidget::~DrawWidget()
{

}

void DrawWidget::drawPixel(QPoint pt){
    QRgb value = m_drawColor.rgb();
    //m_canvas.setPixel(pt.x(),pt.y(),value);
    QPainter p;
    p.begin(&m_canvas);
    QBrush brush = p.brush();
    brush.setStyle(Qt::SolidPattern);
    p.setBrush(brush);
    p.setPen(QPen(Qt::color1)); // use 1 to draw, the pen with is 1 pixel solid line
    int x=pt.x();
    int y=pt.y();// 500 : taille fenetre

   // qDebug() <<QString::number( );
    if((x>=0 && y>=0) && (x<=this->width() && y<=this->height())) //  : taille fenetre
    {
        p.drawEllipse(QRect(x,y,5,5));
        p.end();

        Coordonnees.insert(++index,QPoint(x,y));// 500 : taille de la fenêtre
    }


    qint32 index=0;

    foreach( int key, Coordonnees.keys() )
    {
        QPoint point = Coordonnees.value( key );

        jsonToSend+=QString::number(point.x())+","+QString::number(point.y());

        jsonToSend+=",";

        /*if (index<Coordonnees.size()-1)
        {

        }*/
        index++;



    }


    qDebug()  << jsonToSend.toUtf8();




}

void DrawWidget::mousePressEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        jsonToSend += "[";
        drawPixel(event->pos());
        repaint();
    }
}

void DrawWidget::mouseReleaseEvent(QMouseEvent *event){
    jsonToSend += "],";
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        drawPixel(event->pos());
        repaint();
    }
}

void DrawWidget::resizeEvent(QResizeEvent *event)
{
    m_canvas = QImage(width(),height(),QImage::Format_RGBA8888);
}

QColor DrawWidget::drawColor()
{
    return m_drawColor;
}

void DrawWidget::setDrawColor(QColor color)
{
    m_drawColor = color;
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    painter.drawPixmap(0,0,QPixmap::fromImage(m_canvas));
}

