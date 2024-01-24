#include "canvas.h"
#include "QPainter"
#include "converter.h"
#include "mainwindow.h"
#include "messurebox.h"
#include <QMouseEvent>
#include <QTextStream>
#include <sstream>
#include <QFile>

QImage *Canvas::getResizedImage() const
{
    return resizedImage;
}


Canvas::Canvas(bool isOriginalImage, Converter *converter, MainWindow* mainWindow):isOriginalImage(isOriginalImage), minCandela(0), maxCandela(255), converter(converter), image(NULL), resizedImage(NULL),
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

            painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
            painter.drawRect(QRect(QPoint(resizedImage->width(),0),QPoint(width(),height())));
            int difference = (maxCandela-minCandela) ?  maxCandela-minCandela : 1;
            int skipSize = 1;
            int minSpace = 10;
            int height = this->height();
            while((difference+2*minSpace)/skipSize > height)
                skipSize++;
            int scaleFactor = (height /(difference+2*minSpace)) ? height /(difference+2*minSpace) : 1;

            int space = ((height- (((difference+2*minSpace)/skipSize) * scaleFactor))+minSpace)/2;
            for (int i = minCandela; i < maxCandela; i+= skipSize){
                painter.setPen(QPen(converter->candelaToColor(i,minCandela,maxCandela),1));
                painter.setBrush(QBrush(converter->candelaToColor(i,minCandela,maxCandela),Qt::SolidPattern));
                painter.drawRect(QRect(QPoint(resizedImage->width()+5,(i * scaleFactor)/skipSize +space ),QPoint(width()-30,((i*scaleFactor)/skipSize)+scaleFactor+space)));
                if(i%50 == 0){
                    painter.setPen(QPen(Qt::white,1));
                    painter.drawText(QPoint(width()-20,(i * scaleFactor)/skipSize+space)+QPoint(-4,4),QString::number(i));
                }
            }
            finalPainter.drawPixmap(QPoint(0,0),canvas);
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{

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

        unsigned int candelaValue;
        resize();
        otherCanvas->resize();
        if(mainWindow->getMode() == normalMode){
            candelaValue = (*candela)[Converter::scaleCordtoCanvas(pressedLocation->y(), resizedImage->height(),image->height())][Converter::scaleCordtoCanvas(pressedLocation->x() ,resizedImage->width() , image->width())];
            debugText << "\tCandela: " << candelaValue;
        }
        debugLabel->setText(QString::fromStdString(debugText.str()));
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
                MessureBox* tmp = new MessureBox(*pressedLocation, event->pos(), candela, &resizedImage, &image, converter, mainWindow);
                drawabels.push_back(tmp);
                otherCanvas->getDrawabels().push_back(tmp);
            }else{
                otherCanvas->resize();
                MessureBox* tmp = new MessureBox(*pressedLocation, event->pos(), candela, &otherCanvas->resizedImage, &otherCanvas->image, converter, mainWindow);
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

#include <QLocale>

void Canvas::saveDataAsCSV(QString fileName)
{
    QFile csvFile(fileName);
    csvFile.open(QFile::WriteOnly);
    QTextStream stream(&csvFile);

    // Set the locale to German
    QLocale german(QLocale::German);
    stream.setLocale(german);

    if(mainWindow->getMode() == normalMode){
        stream << "ID" << ";" << "Candela" << ";\n";
        for(int i = 0; i < drawabels.size(); i++){
            MessureBox* messureBox = dynamic_cast<MessureBox*>(drawabels.at(i));
            if(messureBox == NULL)
                continue;
            stream << messureBox->getId() << ";" << german.toString(messureBox->getAvgCandala()) << ";\n";
        }
    } else {
        stream << "ID" << ";" << "Red" << ";" << "Green" << ";" << "Blue" << ";\n";
        for(int i = 0; i < drawabels.size(); i++){
            MessureBox* messureBox = dynamic_cast<MessureBox*>(drawabels.at(i));
            if(messureBox == NULL)
                continue;
            stream << messureBox->getId() << ";" << german.toString(messureBox->getAvgRed()) << ";"
                   << german.toString(messureBox->getAvgGreen()) << ";" << german.toString(messureBox->getAvgBlue()) << ";\n";
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

