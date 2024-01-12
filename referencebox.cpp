#include "referencebox.h"

void ReferenceBox::draw(QPainter *painter)
{
    int xSize = end.x() - origen.x();
    int ySize = end.y() - origen.y();
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::red,2,Qt::DashDotDotLine));
    painter->drawRect(origen.x(),origen.y(), xSize, ySize);
    painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    painter->setPen(QPen(Qt::red,2,Qt::SolidLine));
    painter->drawEllipse(origen,5,5);
    painter->setPen(QPen(Qt::white,2,Qt::SolidLine));
    painter->drawText(origen+QPoint(-4,4),"X");

    if(candela != NULL){
        long double sum = 0;
        for(int i = 0; i<xSize; i++){
            for(int y = 0; y < ySize; y++){
                sum += (*candela)[origen.x()+i][origen.y()+y];
            }
        }
        avgGrey = sum/(xSize*ySize);
        painter->drawText(QPoint(origen.x() + xSize + 5, origen.y()+5),QString::number(avgGrey));
    }
}

unsigned short ReferenceBox::getAvgGrey()
{
    return avgGrey;
}
