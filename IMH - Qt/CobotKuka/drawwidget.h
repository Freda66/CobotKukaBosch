#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H
#include <QObject>
#include <QWidget>
#include <QMap>
#include <QTimer>
#include "mainwindow.h"

class QPaintEvent;
class QMouseEvent;
class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = 0);
    QMap<int, QPoint> Coordonnees;
    int index=0;
    ~DrawWidget();

    QString jsonToSend;

    void drawPixel(QPoint pt);

public slots:
    QColor drawColor();
    void setDrawColor(QColor color);

signals:

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    QColor m_drawColor;
    QPixmap m_pixmap;
    QImage m_canvas;
    QTimer timer;
};

#endif // DRAWWIDGET_H
