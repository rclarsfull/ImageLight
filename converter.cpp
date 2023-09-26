#include <QDebug>
#include <QColor>
#include <math.h>
#include <QVector3D>
#include <QThread>
#include "converter.h"
#include "workerthread.h"
#include "perfomancetimer.h"



void Converter::recolorImage(QImage* image)
{
    PerfomanceTimer timer("recolorImage");
    int minGrey = 0, maxGrey = 255;
    minGrey = Converter::getMinGrey(image);
    maxGrey = Converter::getMaxGrey(image);
    qDebug() << minGrey << " : " <<maxGrey;

    for(int y = 0; y < image->height(); y++){
        for(int x = 0; x < image->width(); x++){
            QColor color = image->pixelColor(x,y);
            image->setPixelColor(x,y,Converter::greyToColor(Converter::colorToGrey(color),minGrey,maxGrey));
        }
    }
}


unsigned int Converter::colorToGrey(QColor color){
    float red = (pow(color.red(),2.2));
    float green = (pow(color.green(),2.2));
    float blue = (pow(color.blue(),2.2));
    int luminance =  pow((red * 0.2126 + green * 0.7152 + blue * 0.0722) ,1/2.2);
    return luminance;
}

unsigned int Converter::greyToCandela(unsigned int grey){
    //PerfomanceTimer timer("greyToCandela");
    int candela = 0.002*grey*grey*grey - 0.0382*grey*grey + 4.224*grey - 4.073;
//    if (candela < 0)
//        candela = 0;
//    if (candela > 4000)
//        candela = 4000;
    //return candela;
    return grey;
}

unsigned int Converter::getConversionPresition(unsigned int grey)
{
    PerfomanceTimer timer("getConversionPresition");
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

inline QColor Converter::greyToColor(unsigned int grey, unsigned int minGrey, unsigned int maxGrey)
{
    //PerfomanceTimer timer("greyToColor");
    double rot = 0, gruen = 0, blau = 0;

    double a = (maxGrey-minGrey)/4;
    double b = 255/(a*a);

    if(grey < 3.5*a){
        rot = -(b*(grey-3*a-minGrey)*(grey-3*a-minGrey))+255;
        gruen = -(b*(grey-2*a-minGrey)*(grey-2*a-minGrey))+255;
        blau = -(b*(grey-a-minGrey)*(grey-a-minGrey))+255;
    }else{
        rot = blau = gruen = -(b*(grey-4*a-minGrey)*(grey-4*a-minGrey))+255;
    }


    if(rot > 255)
        rot = 255;
    if(gruen > 255)
        gruen = 255;
    if(blau > 255)
        blau = 255;
    if(rot < 0)
        rot = 0;
    if(gruen < 0)
        gruen = 0;
    if(blau < 0)
        blau = 0;

    //qDebug() << "a: " << a << "b: " << b << "rot: " <<rot << "gruen: " << gruen << "blau: "<< blau;
    return QColor(rot , gruen, blau);
}
