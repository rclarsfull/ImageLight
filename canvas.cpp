#include <QMouseEvent>
#include <QTextStream>
#include <sstream>
#include "canvas.h"
#include "QPainter"
#include "converter.h"
#include "messurebox.h"




void Canvas::setDebugLabel(QLabel *newDebugLabel)
{
    debugLabel = newDebugLabel;
}

void Canvas::resize()
{
    if(!image.isNull())
        resizedImage = image.scaledToWidth(width(),Qt::SmoothTransformation);

}

QVector<Drawable *>& Canvas::getDrawabels()
{
    return drawabels;
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
    resize();
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
    resize();
    if(!image.isNull()){
        painter.setPen(QPen(Qt::black,3));
        painter.setBrush(QBrush(Qt::SolidPattern));
        painter.drawRect(0,0,width(),height());
        painter.drawImage(QPoint(0,0),resizedImage);
        if(pressedLocation){
            painter.setPen(QPen(Qt::red,1));
            painter.setBrush(QBrush(Qt::NoBrush));
            painter.drawEllipse(*pressedLocation,5,5);
        }
        for (Drawable* d:drawabels)
            if(d!=NULL)
                d->draw(&painter);
    }else{
        painter.setPen(QPen(Qt::black,3));
        painter.setBrush(QBrush(Qt::NoBrush));
        painter.drawRect(0,0,width(),height());
    }

}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    bool delAction = false;
    for (Drawable*& d:drawabels){
        if (d!= NULL && ((d->getOrigen().x()-event->pos().x())*(d->getOrigen().x()-event->pos().x()))+((d->getOrigen().y()-event->pos().y())*(d->getOrigen().y()-event->pos().y())) < 25){
            delete d;
            d = NULL;
            delAction = true;
        }
    }
    for (Drawable*& d:otherCanvas->getDrawabels()){
        if (d!= NULL && ((d->getOrigen().x()-event->pos().x())*(d->getOrigen().x()-event->pos().x()))+((d->getOrigen().y()-event->pos().y())*(d->getOrigen().y()-event->pos().y())) < 25){
            delete d;
            d = NULL;
            delAction = true;
        }
    }
    if(!delAction){
    setPressedLocation(new QPoint(event->pos()));
    otherCanvas->setPressedLocation(new QPoint(event->pos()));
    std::stringstream debugText;
    debugText << "Ausgewähltes Pixel: R: " << resizedImage.pixelColor(*pressedLocation).red();
    debugText << " G: " << resizedImage.pixelColor(*pressedLocation).green();
    debugText << " B: " << resizedImage.pixelColor(*pressedLocation).blue();

    unsigned int grey;
    if(resizedImage.pixelColor(*pressedLocation).green() == resizedImage.pixelColor(*pressedLocation).red() && resizedImage.pixelColor(*pressedLocation).red() == resizedImage.pixelColor(*pressedLocation).green())
        grey = Converter::colorToGrey(resizedImage.pixelColor(*pressedLocation));
    else
        grey = Converter::colorToGrey(otherCanvas->resizedImage.pixelColor(*pressedLocation));
    debugText << "\tGrauwert Pixel:  " << grey;
    debugText << "\tCandela: " << Converter::greyToCandela(grey);
    debugText << "[+/- " << Converter::getConversionPresition(grey) << " ]";
    debugLabel->setText(QString::fromStdString(debugText.str()));
    }
    update();
    otherCanvas->update();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if(pressedLocation != NULL && 100 < (pressedLocation->x() - event->pos().x())*(pressedLocation->x() - event->pos().x())+(pressedLocation->y() - event->pos().y())*(pressedLocation->y() - event->pos().y())){
        drawabels.push_back(new MessureBox(*pressedLocation, event->pos(), &resizedImage));
        otherCanvas->getDrawabels().push_back(new MessureBox(*pressedLocation, event->pos(), &resizedImage));
        pressedLocation = NULL;
        update();
        otherCanvas->update();
    }

}
