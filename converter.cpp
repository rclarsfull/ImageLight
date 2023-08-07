#include <QtGui/QRgb>
#include <QDebug>
#include <QColor>
#include "converter.h"

Converter::Converter()
{

}

QVector<QImage*> Converter::getAsGreyScale(QImage* originalImage)
{
    QVector<QImage*> greyImageChannels(3);
    if(originalImage == NULL)
        qDebug() << "No Image Found !!\n";
    else{
        int imageSize = originalImage->height() * originalImage->width();
        QColor color = originalImage->pixelColor(0,0);
        qDebug() << imageSize << color.red() << color.yellow() <<color.blue();


        greyImageChannels[0] = new QImage(*originalImage);
        greyImageChannels[1] = new QImage(*originalImage);
        greyImageChannels[2] = new QImage(*originalImage);

        qDebug() << "init ferig\n";

        for(int y = 0; y < originalImage->height(); y++){
            for(int x = 0; x < originalImage->width(); x++){
                QColor color = originalImage->pixelColor(x,y);
                greyImageChannels[0]->setPixelColor(x,y,QColor(color.red(),color.red(),color.red()));
                greyImageChannels[1]->setPixelColor(x,y,QColor(color.green(),color.green(),color.green()));
                greyImageChannels[2]->setPixelColor(x,y,QColor(color.blue(),color.blue(),color.blue()));
            }
        }
        qDebug() << "ferig\n";
    }
    return greyImageChannels;
}

QImage* Converter::combineChannels(QVector<QImage*> greyImageChannels)
{
    QImage* out = new QImage(*greyImageChannels[0]);
    for(int y = 0; y < greyImageChannels[0]->height(); y++){
        for(int x = 0; x < greyImageChannels[0]->width(); x++){
            int redGrey = greyImageChannels[0]->pixelColor(x,y).red();
            int greenGrey = greyImageChannels[1]->pixelColor(x,y).green();
            int blueGrey = greyImageChannels[2]->pixelColor(x,y).blue();
            int durchschnitt = (redGrey + greenGrey + blueGrey)/3;
            out->setPixelColor(x,y,QColor(durchschnitt,durchschnitt,durchschnitt));
        }
    }
    return out;
}
