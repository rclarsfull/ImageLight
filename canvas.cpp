#include "canvas.h"
#include "QPainter"


Canvas::Canvas():image(NULL)
{

}

void Canvas::setImage(const QImage &newImage)
{
    image = newImage;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0,0),image);
    painter.setPen(QPen(Qt::black,3));
    painter.drawRect(0,0,width(),height());

}
