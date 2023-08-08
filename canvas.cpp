#include <QMouseEvent>
#include <QTextStream>
#include <sstream>
#include "canvas.h"
#include "QPainter"




void Canvas::setDebugLabel(QLabel *newDebugLabel)
{
    debugLabel = newDebugLabel;
}

Canvas::Canvas():image(NULL), pressedLocation(NULL), otherCanvas(NULL)
{

}

Canvas::~Canvas()
{
    if(pressedLocation != NULL)
        delete pressedLocation;
}

void Canvas::setImage(const QImage &newImage)
{
    image = newImage;
}

void Canvas::setOtherCanvas(Canvas *other)
{
    otherCanvas = other;
}

void Canvas::setPressedLocation(QPoint *point)
{
    if(pressedLocation != NULL)
        delete pressedLocation;
    pressedLocation = point;
}

QPoint *Canvas::getPressedLocation()
{
    return pressedLocation;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(!image.isNull()){
        painter.setPen(QPen(Qt::black,3));
        painter.setBrush(QBrush(Qt::SolidPattern));
        painter.drawRect(0,0,width(),height());
        painter.drawImage(QPoint(0,0),image);
        if(pressedLocation){
            painter.setPen(QPen(Qt::red,1));
            painter.setBrush(QBrush(Qt::NoBrush));
            painter.drawEllipse(*pressedLocation,5,5);
        }
    }else{
        painter.setPen(QPen(Qt::black,3));
        painter.setBrush(QBrush(Qt::NoBrush));
        painter.drawRect(0,0,width(),height());
    }

}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    setPressedLocation(new QPoint(event->pos()));
    otherCanvas->setPressedLocation(new QPoint(event->pos()));
    std::stringstream debugText;
    debugText << "Ausgewähltes Pixel: R: " << image.pixelColor(*pressedLocation).red();
    debugText << " G: " << image.pixelColor(*pressedLocation).green();
    debugText << " B: " << image.pixelColor(*pressedLocation).blue();
    debugText << "\tZugehörigs Pixel: R: " << otherCanvas->image.pixelColor(*pressedLocation).red();
    debugText << " G: " << otherCanvas->image.pixelColor(*pressedLocation).green();
    debugText << " B: " << otherCanvas->image.pixelColor(*pressedLocation).blue();
    debugLabel->setText(QString::fromStdString(debugText.str()));
    update();
    otherCanvas->update();
}
