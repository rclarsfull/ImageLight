#include "converter.h"
#include "workerthread.h"
#include "perfomancetimer.h"
#include "mainwindow.h"
#include <QDebug>
#include <QColor>
#include <math.h>
#include <QVector3D>
#include <QThread>



void Converter::recolorImage(QImage* image, int minGrey, int maxGrey)
{
    PerfomanceTimer timer("recolorImage");

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

unsigned int Converter::getMinGrey(QImage *greyImage){
    PerfomanceTimer timer("min");
    unsigned int min = 255;
    for(int y = 0; y < greyImage->height(); y++){
        for(int x = 0; x < greyImage->width(); x++){
            unsigned int durchschnitt = colorToGrey(greyImage->pixelColor(x,y));
            if(durchschnitt == 0)
                return 0;
            if(durchschnitt < min)
                min = durchschnitt;
        }
    }
    return min;
}

unsigned int Converter::getMaxGrey(QImage *greyImage)
{
    PerfomanceTimer timer("max");
    unsigned int max = 0;
    for(int y = 0; y < greyImage->height(); y++){
        for(int x = 0; x < greyImage->width(); x++){
            unsigned int durchschnitt = colorToGrey(greyImage->pixelColor(x,y));
            if(durchschnitt == 255)
                return 255;
            if(durchschnitt > max)
                max = durchschnitt;
        }
    }
    return max;
}


unsigned int Converter::greyToCandela(unsigned int grey){
    if(mainWindow->getMode() == withoutReference)
        return 3.352*grey - 13.925;
    else
        return grey+mainWindow->getReferenceValue();
    //PerfomanceTimer timer("greyToCandela");
    //return 0.0185*grey*grey - 0.753*grey + 47.205;
    // return 23.806 * qExp(0.0189*grey);
    // return grey;
}


unsigned int Converter::getConversionPresition(unsigned int grey)
{
    //PerfomanceTimer timer("getConversionPresition");
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
    if(grey > maxGrey)
        return QColor(255,255,255);
    if(grey < minGrey)
        return QColor(0,0,0);

    double rot = 0, gruen = 0, blau = 0;

    double a = (maxGrey-minGrey)/4;
    double b = 255/(a*a);

    if(grey < (minGrey + 3*a)){
        rot = -(b*(grey-3*a-minGrey)*(grey-3*a-minGrey))+255;
        gruen = -(b*(grey-2*a-minGrey)*(grey-2*a-minGrey))+255;
        blau = -(b*(grey-a-minGrey)*(grey-a-minGrey))+255;
    }else if(grey < (minGrey + 3*a)){
        rot = 255;
        gruen = 0;
        blau = -(b*(grey-4*a-minGrey)*(grey-4*a-minGrey))+255;
    }else{
        rot = 255;
        blau = -(b*(grey-4*a-minGrey)*(grey-4*a-minGrey))+255;
        gruen = -(b*(grey-4.5*a-minGrey)*(grey-4*a-minGrey))+255;
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
