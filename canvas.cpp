#include "canvas.h"
#include "QPainter"
#include "converter.h"
#include "messurebox.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include <QTextStream>
#include <sstream>
#include <QFile>

Canvas::Canvas(bool isOriginalImage, Converter *converter, MainWindow* mainWindow):isOriginalImage(isOriginalImage), minGrey(0), maxGrey(255), converter(converter), image(NULL), resizedImage(NULL),
    pressedLocation(NULL), otherCanvas(NULL), canvas(), mainWindow(mainWindow)
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
    delDrawabels();

}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter finalPainter(this);
    canvas = QPixmap(width(),height());
    QPainter painter(&canvas);
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

        painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
        painter.drawRect(QRect(QPoint(resizedImage->width(),0),QPoint(width(),height())));
        int scaleFactor = height() /(maxGrey-minGrey);
        int space = (height()-(maxGrey-minGrey)*scaleFactor)/2;
        for (int i = minGrey; i < maxGrey; i++){

            painter.setPen(QPen(converter->greyToColor(i,minGrey,maxGrey),1));
            painter.setBrush(QBrush(converter->greyToColor(i,minGrey,maxGrey),Qt::SolidPattern));
            painter.drawRect(QRect(QPoint(resizedImage->width()+5,i * scaleFactor +space ),QPoint(width()-30,(i+scaleFactor)*scaleFactor+space)));
            if(i%10 == 0){
                painter.setPen(QPen(Qt::white,1));
                painter.drawText(QPoint(width()-20,i * scaleFactor+space)+QPoint(-4,4),QString::number(converter->greyToCandela(i)));
            }

        }
        finalPainter.drawPixmap(QPoint(0,0),canvas);
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if(mainWindow->getMode() == referenceSelection){
        // drawabels.push_front(new ReferenceBox())
    }else{
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
                grey = converter->colorToGrey(resizedImage->pixelColor(*pressedLocation));
            else
                grey = converter->colorToGrey(otherCanvas->resizedImage->pixelColor(*pressedLocation));
            debugText << "\tGrauwert Pixel:  " << grey;
            debugText << "\tCandela: " << converter->greyToCandela(grey);
            debugText << "[+/- " << converter->getConversionPresition(grey) << " ]";
            debugLabel->setText(QString::fromStdString(debugText.str()));
        }
    }
    update();
    otherCanvas->update();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if(!isDelAction){
        if(pressedLocation != NULL && 100 < (pressedLocation->x() - event->pos().x())*(pressedLocation->x() - event->pos().x())+(pressedLocation->y() - event->pos().y())*(pressedLocation->y() - event->pos().y())){
            if(isOriginalImage){
                resize();
                MessureBox* tmp = new MessureBox(*pressedLocation, event->pos(), &resizedImage, converter);
                drawabels.push_back(tmp);
                otherCanvas->getDrawabels().push_back(tmp);
            }else{
                otherCanvas->resize();
                MessureBox* tmp = new MessureBox(*pressedLocation, event->pos(), &otherCanvas->resizedImage, converter);
                drawabels.push_back(tmp);
                otherCanvas->getDrawabels().push_back(tmp);
            }

            pressedLocation = NULL;
            update();
            otherCanvas->update();
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
        resizedImage = new QImage(image->scaledToWidth(width()-75,Qt::SmoothTransformation));
        otherCanvas->resizedImage = new QImage(otherCanvas->image->scaledToWidth(width()-75,Qt::SmoothTransformation));
    }else
        qDebug() << "Fehler Image Null resize ignored";
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

QPixmap *Canvas::getCanvas()
{
    return &canvas;
}

void Canvas::saveDataAsCSV(QString fileName)
{
    QFile csvFile(fileName);
    csvFile.open(QFile::WriteOnly);
    QTextStream stream(&csvFile);
    stream << ";" << "ID" << ";" << "Candela" << ";\n";
    for(int i = 0; i < drawabels.size(); i++){
        MessureBox* messureBox = dynamic_cast<MessureBox*>(drawabels.at(i));
        if(messureBox == NULL)
            continue;
        stream << ";" << messureBox->getId() << ";" << messureBox->getAvgCandala() << ";\n";
        //qDebug() << ";" << messureBox->getAvgCandala() << ";\n";
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

