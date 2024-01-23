#include "messurebox.h"
#include "converter.h"
#include "mainwindow.h"
int MessureBox::counter = 1;

MessureBox::MessureBox(QPoint origen, QPoint end, unsigned short (**candela)[Global::X_RESELUTION], QImage **resizedImage, QImage **originalImage, Converter *converter, MainWindow *mainWindow)
    :Drawable(origen,converter)
    , end(end)
    , candela(candela)
    , resizedImage(resizedImage)
    , originalImage(originalImage)
    , mainWindow(mainWindow)
    , avgCandala(-1)
    , avgRed(-1)
    , avgGreen(-1)
    , avgBlue(-1)
    , id(counter)
{
    calcAvgCanela();
    counter++;
}

void MessureBox::draw(QPainter *painter)
{
    int xSize = end.x() - origen.x();
    int ySize = end.y() - origen.y();
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::yellow,2,Qt::DashDotDotLine));
    painter->drawRect(origen.x(),origen.y(), xSize, ySize);
    painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    painter->setPen(QPen(Qt::red,2,Qt::SolidLine));
    painter->drawEllipse(origen,5,5);
    painter->setPen(QPen(Qt::white,2,Qt::SolidLine));
    painter->drawText(origen+QPoint(-4,4),"X");

    calcAvgCanela();
    painter->drawText(QPoint(origen.x() + xSize + 5, origen.y()+5),"ID: "+ QString::number(id));
    if(mainWindow->getMode() == normalMode)
        painter->drawText(QPoint(origen.x() + xSize + 5, origen.y()+15),"Candela: "+ QString::number(avgCandala));
}

float MessureBox::getAvgRed() const
{
    return avgRed;
}

float MessureBox::getAvgGreen() const
{
    return avgGreen;
}

float MessureBox::getAvgBlue() const
{
    return avgBlue;
}

void MessureBox::calcAvgCanela()
{
    int xSize = end.x() - origen.x();
    int ySize = end.y() - origen.y();
    if(candela != NULL){
        long int sumCandela = 0;
        double sumRed = 0;
        double sumGreen = 0;
        double sumBlue = 0;
        for(int i = 0; i<xSize; i++){
            for(int y = 0; y < ySize; y++){
                QPoint point(Converter::scaleCordtoCanvas(origen.x()+i, (*resizedImage)->width(), (*originalImage)->width()), Converter::scaleCordtoCanvas(origen.y()+y, (*resizedImage)->height(), (*originalImage)->height()));
                if(mainWindow->getMode() == normalMode)
                    sumCandela += (*candela)[point.y()][point.x()];
                QColor color = (*originalImage)->pixelColor(point);
                float *rgb = converter->colorToRGBArray(color,point.x(),point.y());
                sumRed += rgb[0];
                sumGreen += rgb[1];
                sumBlue += rgb[2];
                delete[] rgb;
            }
        }
        if(mainWindow->getMode() == normalMode)
            avgCandala = (sumCandela/(float)(xSize*ySize))+0.5;
        avgRed = (sumRed/(float)(xSize*ySize));
        avgGreen = (sumGreen/(float)(xSize*ySize));
        avgBlue = (sumBlue/(float)(xSize*ySize));
    }
}

