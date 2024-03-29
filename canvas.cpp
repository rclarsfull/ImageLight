#include "canvas.h"
#include "QPainter"
#include "converter.h"
#include "mainwindow.h"
#include "messurebox.h"
#include "converter.h"
#include <QMouseEvent>
#include <QTextStream>
#include <sstream>
#include <QFile>
#include <QLocale>

QImage *Canvas::getResizedImage() const
{
    return resizedImage;
}

bool Canvas::saveCanvas(QString filename)
{
    return canvas.save(filename);
}


Canvas::Canvas(bool isOriginalImage, Converter *converter, MainWindow* mainWindow):isOriginalImage(isOriginalImage), minCandela(0), maxCandela(255), converter(converter), image(NULL), originalResizedImage(NULL),
    resizedImage(NULL), pressedLocation(NULL), otherCanvas(NULL), canvas(), mainWindow(mainWindow)
{
}

void Canvas::delDrawabels()
{
    if(isOriginalImage){
        for (Drawable*& d:drawabels){
            if (d!= NULL){
                d = NULL;
                isDelAction = true;
            }
        }
        for (Drawable*& d:otherCanvas->getDrawabels()){
            if (d!= NULL){
                delete d;
                d = NULL;
                isDelAction = true;
            }
        }
    }

}

Canvas::~Canvas()
{
    if(pressedLocation != NULL){
        delete pressedLocation;
        pressedLocation = NULL;
    }
    if(resizedImage != NULL)
        delete resizedImage;
    if(otherCanvas->resizedImage != NULL)
        delete otherCanvas->resizedImage;
    if(originalResizedImage != NULL)
        delete originalResizedImage;
    if(otherCanvas->originalResizedImage != NULL)
        delete otherCanvas->originalResizedImage;
    delDrawabels();

}

void Canvas::paintEvent(QPaintEvent *event)
{
    if(isOriginalImage || candela != NULL){
        QPainter finalPainter(this);
        canvas = QPixmap(width(),height());
        QPainter painter(&canvas);
        painter.setPen(QPen(Qt::black,1));
        painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
        painter.drawRect(QRect(QPoint(0,0),QPoint(width(),height())));
        resize();
        if(image != NULL){
            painter.drawImage(QPoint(0,0),*resizedImage);
            if(pressedLocation){
                painter.setPen(QPen(Qt::red,1));
                painter.setBrush(QBrush(Qt::NoBrush));
                painter.drawEllipse(*pressedLocation,5,5);
            }
            for (Drawable*& d:drawabels)
                if(d!=NULL)
                    d->draw(&painter);
            painter.setPen(QPen(Qt::black,1));
            painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
            painter.drawRect(QRect(QPoint(resizedImage->width(),0),QPoint(width(),height())));
            int difference = (maxCandela-minCandela) ?  maxCandela-minCandela : 1;
            int labelCount = mainWindow->getSettingsWindow()->getLabelCount();
            int skipSize = 1;
            int minSpace = 10;
            int height = this->height();
            while((difference+2*minSpace)/skipSize > height)
                skipSize++;
            int scaleFactor = (height /(difference+2*minSpace)) ? height /(difference+2*minSpace) : 1;

            int space = (height - ((difference/skipSize) * scaleFactor)+2*minSpace)/2;
            painter.setPen(QPen(Qt::white,1));
            painter.drawText(QPoint(width()-20,space)+QPoint(-4,4),QString::number(minCandela));
            painter.drawLine(QPoint(width()-25,space),QPoint(width()-30,space));
            painter.drawText(QPoint(width()-20,((maxCandela-minCandela) * scaleFactor)/skipSize+space)+QPoint(-4,4),QString::number(maxCandela));
            painter.drawLine(QPoint(width()-25,((maxCandela-minCandela) * scaleFactor)/skipSize+space),QPoint(width()-30,((maxCandela-minCandela) * scaleFactor)/skipSize+space));
            for (int i = minCandela; i < maxCandela; i+= skipSize){
                if(i%labelCount == 0){
                    painter.setPen(QPen(Qt::white,1));
                    painter.drawText(QPoint(width()-20,((i-minCandela) * scaleFactor)/skipSize+space+scaleFactor/2)+QPoint(-4,4),QString::number(i));
                    painter.drawLine(QPoint(width()-25,((i-minCandela) * scaleFactor)/skipSize+space+scaleFactor/2),QPoint(width()-30,((i-minCandela) * scaleFactor)/skipSize+space+scaleFactor/2));
                }
                painter.setPen(QPen(converter->candelaToColor(i,minCandela,maxCandela),1));
                painter.setBrush(QBrush(converter->candelaToColor(i,minCandela,maxCandela),Qt::SolidPattern));
                painter.drawRect(QRect(QPoint(resizedImage->width()+5,((i-minCandela) * scaleFactor)/skipSize +space ),QPoint(width()-30,(((i-minCandela)*scaleFactor)/skipSize)+scaleFactor+space)));
            }
            painter.setPen(QPen(Qt::white,1));
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(QRect(QPoint(resizedImage->width()+5,space),QPoint(width()-30,((maxCandela-minCandela) * scaleFactor)/skipSize+space)));
            finalPainter.drawPixmap(QPoint(0,0),canvas);
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if(image != NULL){
        if(event->pos().x() <= resizedImage->width() && event->pos().y() <= resizedImage->height()){
            isDelAction = false;
            for (Drawable*& d:drawabels){
                if (d!= NULL && ((d->getOrigen().x()-event->pos().x())*(d->getOrigen().x()-event->pos().x()))+((d->getOrigen().y()-event->pos().y())*(d->getOrigen().y()-event->pos().y())) < 25){
                    d = NULL;
                    isDelAction = true;
                }
            }
            for (Drawable*& d:otherCanvas->getDrawabels()){
                if (d!= NULL && ((d->getOrigen().x()-event->pos().x())*(d->getOrigen().x()-event->pos().x()))+((d->getOrigen().y()-event->pos().y())*(d->getOrigen().y()-event->pos().y())) < 25){
                    delete d;
                    d = NULL;
                    isDelAction = true;
                }
            }
            if(!isDelAction){
                //TODO Handel delete
                setPressedLocation(new QPoint(event->pos()));
                otherCanvas->setPressedLocation(new QPoint(event->pos()));
            }
            update();
            otherCanvas->update();
        }
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if(image != NULL){
        if(!isDelAction){
            if(event->pos().x() <= resizedImage->width() && event->pos().y() <= resizedImage->height()){
                if(pressedLocation != NULL && 100 < (pressedLocation->x() - event->pos().x())*(pressedLocation->x() - event->pos().x())+(pressedLocation->y() - event->pos().y())*(pressedLocation->y() - event->pos().y())){
                    resize();
                    MessureBox* tmp = new MessureBox(*pressedLocation, event->pos(), candela, &originalResizedImage, &originalImage, converter, mainWindow);
                    drawabels.push_back(tmp);
                    otherCanvas->getDrawabels().push_back(tmp);
                    pressedLocation = NULL;
                    update();
                    otherCanvas->update();
                }
            }
        }
    }
}

void Canvas::resize()
{
    if(image != NULL){
        if(resizedImage != NULL)
            delete resizedImage;
        if(otherCanvas->resizedImage != NULL)
            delete otherCanvas->resizedImage;
        if(originalResizedImage != NULL)
            delete originalResizedImage;
        if(otherCanvas->originalResizedImage != NULL)
            delete otherCanvas->originalResizedImage;
        resizedImage = new QImage(image->scaledToWidth(width()-75,Qt::SmoothTransformation));
        otherCanvas->resizedImage = new QImage(otherCanvas->image->scaledToWidth(width()-75,Qt::SmoothTransformation));
        originalResizedImage = new QImage(originalImage->scaledToWidth(width()-75,Qt::SmoothTransformation));
        otherCanvas->originalResizedImage = new QImage(originalImage->scaledToWidth(width()-75,Qt::SmoothTransformation));
        for (Drawable*& d:otherCanvas->getDrawabels()){
            MessureBox* tmp = dynamic_cast<MessureBox*>(d);
            if (tmp != nullptr && (tmp->getOrigen().x() >= resizedImage->width() || tmp->getOrigen().y() >= resizedImage->height() || tmp->getEnd().x() >= resizedImage->width() || tmp->getEnd().y() >= resizedImage->height())){
                d = NULL;
            }
        }
        for (Drawable*& d:getDrawabels()){
            MessureBox* tmp = dynamic_cast<MessureBox*>(d);
            if (tmp != nullptr && (tmp->getOrigen().x() >= resizedImage->width() || tmp->getOrigen().y() >= resizedImage->height() || tmp->getEnd().x() >= resizedImage->width() || tmp->getEnd().y() >= resizedImage->height())){
                delete d;
                d = NULL;
            }
        }
    }
}

void Canvas::setDebugLabel(QLabel *newDebugLabel)
{
    debugLabel = newDebugLabel;
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

void Canvas::setImage(QImage *newImage)
{
    image = newImage;
}

void Canvas::setOriginalImage(QImage *newImage)
{
    originalImage = newImage;
}

int Canvas::getMinCandela() const
{
    return minCandela;
}

void Canvas::setMinCandela(int newMinCandela)
{
    minCandela = newMinCandela;
}

int Canvas::getMaxCandela() const
{
    return maxCandela;
}

void Canvas::setMaxCandela(int newMaxCandela)
{
    maxCandela = newMaxCandela;
}

QPixmap *Canvas::getCanvas()
{
    return &canvas;
}

void Canvas::saveDataAsCSV(QString fileName)
{
    QFile csvFile(fileName);
    csvFile.open(QFile::WriteOnly);
    QTextStream stream(&csvFile);
    QLocale location(mainWindow->getSettingsWindow()->getLocation());
    stream.setLocale(location);

    if(mainWindow->getMode() == normalMode){
        stream << "ID" << ";" << "Candela" << ";\n";
        for(int i = 0; i < drawabels.size(); i++){
            MessureBox* messureBox = dynamic_cast<MessureBox*>(drawabels.at(i));
            if(messureBox == NULL)
                continue;
            stream << messureBox->getId() << ";" << location.toString(messureBox->getAvgCandala()) << ";\n";
        }
    } else {
        stream << "ID" << ";" << "Red" << ";" << "Green" << ";" << "Blue" << ";\n";
        for(int i = 0; i < drawabels.size(); i++){
            MessureBox* messureBox = dynamic_cast<MessureBox*>(drawabels.at(i));
            if(messureBox == NULL)
                continue;
            stream << messureBox->getId() << ";" << location.toString(messureBox->getAvgRed()) << ";"
                   << location.toString(messureBox->getAvgGreen()) << ";" << location.toString(messureBox->getAvgBlue()) << ";\n";
        }
    }
    csvFile.close();
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

