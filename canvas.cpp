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
    if(image != NULL){
        if(resizedImage != NULL)
            delete resizedImage;
        if(otherCanvas->resizedImage != NULL)
            delete otherCanvas->resizedImage;
        resizedImage = new QImage(image->scaledToWidth(width()-75,Qt::SmoothTransformation));
        otherCanvas->resizedImage = new QImage(otherCanvas->image->scaledToWidth(width()-75,Qt::SmoothTransformation));
    }else
        qDebug() << "Fehler Image Null resize ignored";
}

QVector<Drawable *>& Canvas::getDrawabels()
{
    return drawabels;
}

bool Canvas::getIsOriginalImage() const
{
    return isOriginalImage;
}



QImage* Canvas::getImage()
{
    return image;
}

int Canvas::getMinGrey() const
{
    return minGrey;
}

void Canvas::setMinGrey(int newMinGrey)
{
    minGrey = newMinGrey;
}

int Canvas::getMaxGrey() const
{
    return maxGrey;
}

void Canvas::setMaxGrey(int newMaxGrey)
{
    maxGrey = newMaxGrey;
}

QImage *Canvas::getCanvas()
{
    return &canvas;
}

Canvas::Canvas(bool isOriginalImage):isOriginalImage(isOriginalImage), minGrey(0), maxGrey(255), image(NULL), resizedImage(NULL), pressedLocation(NULL), otherCanvas(NULL), canvas()
{

}

Canvas::~Canvas()
{
    if(pressedLocation != NULL)
        delete pressedLocation;
}

void Canvas::setImage(QImage *newImage)
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
    QPainter finalPainter(this);
    canvas = QImage(width(),height(), QImage::Format_RGB888);
    QPainter painter(&canvas);
    resize();
    if(image != NULL){
        painter.drawImage(QPoint(0,0),*resizedImage);
        if(pressedLocation){
            painter.setPen(QPen(Qt::red,1));
            painter.setBrush(QBrush(Qt::NoBrush));
            painter.drawEllipse(*pressedLocation,5,5);
        }
        for (Drawable* d:drawabels)
            if(d!=NULL)
                d->draw(&painter);

        painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
        painter.drawRect(QRect(QPoint(resizedImage->width(),0),QPoint(width(),height())));
        int scaleFactor = height() /(maxGrey-minGrey);
        int space = (height()-(maxGrey-minGrey)*scaleFactor)/2;
        for (int i = minGrey; i < maxGrey; i++){

            painter.setPen(QPen(Converter::greyToColor(i,minGrey,maxGrey),1));
            painter.setBrush(QBrush(Converter::greyToColor(i,minGrey,maxGrey),Qt::SolidPattern));
            painter.drawRect(QRect(QPoint(resizedImage->width()+5,i * scaleFactor +space ),QPoint(width()-30,(i+scaleFactor)*scaleFactor+space)));
            if(i%10 == 0){
                painter.setPen(QPen(Qt::white,1));
                painter.drawText(QPoint(width()-20,i * scaleFactor+space)+QPoint(-4,4),QString::number(Converter::greyToCandela(i)));
            }

        }
//        painter.setPen(QPen(Qt::yellow,3));
//        painter.drawLine(QPoint(resizedImage->width(),minGrey*tmp),QPoint(width(),minGrey*tmp));
        finalPainter.drawImage(QPoint(0,0),canvas);
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
    debugText << "Ausgewähltes Pixel: R: " << resizedImage->pixelColor(*pressedLocation).red();
                                              debugText << " G: " << resizedImage->pixelColor(*pressedLocation).green();
    debugText << " B: " << resizedImage->pixelColor(*pressedLocation).blue();

    unsigned int grey;
    resize();
    otherCanvas->resize();
    if(isOriginalImage)
            grey = Converter::colorToGrey(resizedImage->pixelColor(*pressedLocation));
    else
            grey = Converter::colorToGrey(otherCanvas->resizedImage->pixelColor(*pressedLocation));
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
    if(isOriginalImage){
        resize();
        drawabels.push_back(new MessureBox(*pressedLocation, event->pos(), &resizedImage));
        otherCanvas->getDrawabels().push_back(new MessureBox(*pressedLocation, event->pos(), &resizedImage));
    }else{
        otherCanvas->resize();
        drawabels.push_back(new MessureBox(*pressedLocation, event->pos(), &otherCanvas->resizedImage));
        otherCanvas->getDrawabels().push_back(new MessureBox(*pressedLocation, event->pos(), &otherCanvas->resizedImage));
    }

        pressedLocation = NULL;
        update();
        otherCanvas->update();
    }

}
