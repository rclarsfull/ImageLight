#include "messurebox.h"
#include "converter.h"


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
    double sum = 0;
    for(int i = 0; i<xSize; i++){
        for(int y = 0; y < ySize; y++){
            sum += Converter::greyToCandela(Converter::greyToCandela(Converter::colorToGrey(image->pixelColor(origen.x()+i,origen.y()+y))));
        }
    }
    painter->drawText(QPoint(origen.x() + xSize + 5, origen.y()+5),QString::number(qRound(sum/(xSize*ySize))));
}
