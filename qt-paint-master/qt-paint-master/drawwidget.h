#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QObject>
#include <QWidget>

class QPaintEvent;
class QMouseEvent;
class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = 0);
    ~DrawWidget();

    void drawPixel(QPoint pt);

public slots:
    QColor drawColor();
    void setDrawColor(QColor color);

signals:

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    QColor m_drawColor;
    QPixmap m_pixmap;
    QImage m_canvas;
};

#endif // DRAWWIDGET_H
