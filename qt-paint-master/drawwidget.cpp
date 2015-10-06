#include "drawwidget.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRgb>
#include "qdebug.h"

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    m_drawColor = QColor(Qt::black);

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
    qint32 x=pt.x();
    qint32 y=pt.y();

    if(x>=0 && y>=0)
    {
        p.drawEllipse(QRect(x,y,5,5));
        p.end();
        Coordonnees.insert(++index,QPoint(x,500-y));// 500 : taille de la fenêtre
    }


    QString json="{\"pt\"[500,500] ,\"dessin\":[";
    qint32 index=0;
    for(auto e : Coordonnees.keys())
    {
        json+=QString::number(Coordonnees.value(e).x())+","+QString::number(Coordonnees.value(e).y());

        if (index<Coordonnees.size()-1)
        {

            json+=",";
        }
        index++;
    }
    json+="]}";
    qDebug()  << json.toUtf8();
}

void DrawWidget::mousePressEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        drawPixel(event->pos());
        repaint();
    }
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

