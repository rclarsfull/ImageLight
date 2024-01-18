#include "messurebox.h"
#include "canvas.h"
#include "converter.h"
#include "mainwindow.h"
int MessureBox::counter = 1;

MessureBox::MessureBox(QPoint origen, QPoint end, unsigned short (**candela)[Global::Y_RESELUTION], Canvas* canvas, QImage* originalImage, Converter *converter, MainWindow *mainWindow)
    :Drawable(origen,converter)
    , end(end)
    , candela(candela)
    , canvas(canvas)
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

int MessureBox::getAvgRed() const
{
    return avgRed;
}

int MessureBox::getAvgGreen() const
{
    return avgGreen;
}

int MessureBox::getAvgBlue() const
{
    return avgBlue;
}

void MessureBox::calcAvgCanela()
{
    int xSize = end.x() - origen.x();
    int ySize = end.y() - origen.y();
    if(candela != NULL){
        long int sumCandela = 0;
        long int sumRed = 0;
        long int sumGreen = 0;
        long int sumBlue = 0;
        for(int i = 0; i<xSize; i++){
            for(int y = 0; y < ySize; y++){
                if(mainWindow->getMode() == normalMode)
                    sumCandela += (*candela)[Converter::scaleCordtoCanvas(origen.x()+i, canvas->width(), originalImage->width())][Converter::scaleCordtoCanvas(origen.y()+y, canvas->height(), originalImage->height())];
                QColor color = originalImage->pixelColor(Converter::scaleCordtoCanvas(origen.x()+i, canvas->width(), originalImage->width()), Converter::scaleCordtoCanvas(origen.y()+y, canvas->height(), originalImage->height()));
                sumRed += color.red();
                sumGreen += color.green();
                sumBlue += color.blue();
            }
        }
        if(mainWindow->getMode() == normalMode)
            avgCandala = (sumCandela/(float)(xSize*ySize))+0.5;
        avgRed = (sumRed/(float)(xSize*ySize))+0.5;
        avgGreen = (sumGreen/(float)(xSize*ySize))+0.5;
        avgBlue = (sumBlue/(float)(xSize*ySize))+0.5;
    }
}

