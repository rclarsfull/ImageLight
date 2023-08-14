#include <QDebug>
#include <QColor>
#include <math.h>
#include <QVector3D>
#include "converter.h"


QVector<QImage*> Converter::getAsGreyScale(QImage* originalImage)
{
    QVector<QImage*> greyImageChannels(3);

        greyImageChannels[0] = new QImage(*originalImage);
        greyImageChannels[1] = new QImage(*originalImage);
        greyImageChannels[2] = new QImage(*originalImage);

        for(int y = 0; y < originalImage->height(); y++){
            for(int x = 0; x < originalImage->width(); x++){
                QColor color = originalImage->pixelColor(x,y);
                if(color.red() == 255 && color.green() == 255 && color.blue() == 255){
                    //qDebug() << "Achtung Pixel [" << x << "; " << y << "] ist überbelichtet. Verlust von Genauigkeit!";
                    originalImage->setPixelColor(QPoint(x,y),QColor(255,105,180)); //HotPink
                }
                if(color.red() == 0 && color.green() == 0 && color.blue() == 0){
                    //qDebug() << "Achtung Pixel [" << x << "; " << y << "] ist unterbelichtet. Verlust von Genauigkeit!";
                    originalImage->setPixelColor(QPoint(x,y),QColor(255,105,180)); //HotPink
                }
                greyImageChannels[0]->setPixelColor(x,y,QColor(color.red(),color.red(),color.red()));
                greyImageChannels[1]->setPixelColor(x,y,QColor(color.green(),color.green(),color.green()));
                greyImageChannels[2]->setPixelColor(x,y,QColor(color.blue(),color.blue(),color.blue()));
            }
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

unsigned int Converter::greyToCandela(unsigned int grey){
    int candela = 0.002*grey*grey*grey - 0.0382*grey*grey + 4.224*grey - 4.073;
    if (candela < 0)
        candela = 0;
    if (candela > 4000)
        candela = 4000;
    return candela;
}

unsigned int Converter::getConversionPresition(unsigned int grey)
{
    int candela = 0.002*grey*grey*grey - 0.0382*grey*grey + 4.224*grey - 4.073;
    int before, after;
    if(grey > 0){
        unsigned int tmp = grey - 1;
        before = 0.002*tmp*tmp*tmp - 0.0382*tmp*tmp + 4.224*tmp - 4.073;
    }else
        before = candela;
    if(grey > 255){
        unsigned int tmp = grey + 1;
        after = 0.002*tmp*tmp*tmp - 0.0382*tmp*tmp + 4.224*tmp - 4.073;
    }else
        after = candela;
    return (after-before)/2;
}

void Converter::greyImageToColorImage(QImage* greyImage, double modifyer)
{
    for(int y = 0; y < greyImage->height(); y++){
        for(int x = 0; x < greyImage->width(); x++){
                unsigned int grey = greyImage->pixelColor(x,y).red();
                greyImage->setPixelColor(x,y,Converter::greyToColor(grey,modifyer));
        }
    }
}

QColor Converter::greyToColor(unsigned int grey, double modifyer)
{
    double r = 0, g = 0, b = 0;
    if(grey == 0)
        grey = 1;
    if(grey < (255* 0.5))
        b = 255 * cos(0.006184 * modifyer * grey);

    if(grey > (255* 0.5))
        r = 255 * cos(0.006184 * modifyer * (grey-255));

    g =  255 *  sin(2* 0.006184 * 1/modifyer * grey);
    if(r > 255)
        r = 255;
    if(g > 255)
        g = 255;
    if(b > 255)
        b = 255;
    if(r < 0)
        r = 0;
    if(g < 0)
        g = 0;
    if(b < 0)
        b = 0;
    //qDebug() << r << g << b;
    return QColor(r , g, b);
}
