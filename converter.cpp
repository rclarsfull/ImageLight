#include "converter.h"
#include "workerthread.h"
//#include "perfomancetimer.h"
#include "mainwindow.h"
#include <QDebug>
#include <QColor>
#include <math.h>
#include <QVector3D>
#include <QThread>
#include <fstream>

double Converter::redModifer = 1;
double Converter::greenModifer = 1;
double Converter::blueModifer = 1;


Converter::Converter(MainWindow *mainWindow):mainWindow(mainWindow){
    lightCorrectionMatrix = new float[xReselution][yReselution];
    std::ifstream in("correctionFile.dat", std::ios_base::binary);
    if(! in.read((char*)lightCorrectionMatrix,sizeof(float)*xReselution*yReselution)){
        resetLightCorrectionMatrix();
        qDebug() << "cant open Correction file";
    }
}

Converter::~Converter(){
    delete[] lightCorrectionMatrix;
}

void Converter::recolorImage(QImage* greyImage,QImage* falschfarbenBild, int minGrey, int maxGrey)
{
    for(int y = 0; y < greyImage->height(); y++){
        for(int x = 0; x < greyImage->width(); x++){
            int grey = Converter::colorToGrey(greyImage->pixelColor(x,y),x,y);
            QColor color(grey,grey,grey);
            //qDebug() << color.red() << color.green() << color.blue();
            greyImage->setPixelColor(x, y, color);
            falschfarbenBild->setPixelColor(x,y,Converter::greyToColor(greyImage->pixelColor(x,y).red(),minGrey,maxGrey));
        }
    }
}

void Converter::updateFalschfarbenBild(QImage* greyImage,QImage* falschfarbenBild,int minGrey, int maxGrey)
{
    for(int y = 0; y < greyImage->height(); y++){
        for(int x = 0; x < greyImage->width(); x++){
            falschfarbenBild->setPixelColor(x,y,Converter::greyToColor(greyImage->pixelColor(x,y).red(),minGrey,maxGrey));
        }
    }
}

unsigned int Converter::colorToGrey(QColor color, unsigned int x, unsigned int y){
    float red = (pow(color.red(),2.2));
    float green = (pow(color.green(),2.2));
    float blue = (pow(color.blue(),2.2));
    float luminance =  pow((red * redModifer * 0.686*0.2126 + green * greenModifer *0.878* 0.7152 + blue * blueModifer *1.616* 0.0722) ,1/2.2);
    if(luminance<0)
        luminance=0;
    return qRound(luminance * lightCorrectionMatrix[x][y]);
}

unsigned int Converter::getMinGrey(QImage *greyImage){
    unsigned int min = 255;
    for(int y = 0; y < greyImage->height(); y++){
        for(int x = 0; x < greyImage->width(); x++){
            unsigned int durchschnitt = greyImage->pixelColor(x,y).red();
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
    unsigned int max = 0;
    for(int y = 0; y < greyImage->height(); y++){
        for(int x = 0; x < greyImage->width(); x++){
            unsigned int durchschnitt = greyImage->pixelColor(x,y).red();
            if(durchschnitt == 255)
                return 255;
            if(durchschnitt > max)
                max = durchschnitt;
        }
    }
    return max;
}

void Converter::calibrateLightCorrectionMatrix(QImage *image){
    if(image != NULL){
        std::ofstream out("correctionFile.dat",std::ios_base::binary);
        unsigned long sum = 0;
        resetLightCorrectionMatrix();
        for(int y = 0; y < yReselution; y++){
            for(int x = 0; x < xReselution; x++){
                sum += colorToGrey(image->pixelColor(QPoint(x,y)), x, y);
            }
        }
        double avg = sum / (yReselution*xReselution);
        double max = 0;
        for(int y = 0; y < yReselution; y++){
            for(int x = 0; x < xReselution; x++){
                lightCorrectionMatrix[x][y] = avg / colorToGrey(image->pixelColor(QPoint(x,y)), x, y);
                if(max < lightCorrectionMatrix[x][y])
                    max = lightCorrectionMatrix[x][y];
            }
        }
        for(int y = 0; y < yReselution; y++){
            for(int x = 0; x < xReselution; x++){
                lightCorrectionMatrix[x][y] = lightCorrectionMatrix[x][y] /max;
            }
        }
        out.write((char*)lightCorrectionMatrix,sizeof(float)*xReselution*yReselution);
        out.close();
        qDebug() << "calibrated";
    }else
        qDebug() << "Error: no Image loaded";
}

void Converter::resetLightCorrectionMatrix(){
    for (int i = 0; i< yReselution; i++) {
        for (int j = 0; j < xReselution; ++j) {
            lightCorrectionMatrix[j][i] = 1.0;
        }
    }
}




unsigned int Converter::greyToCandela(unsigned int grey){
    if(mainWindow->getMode() == withoutReference){
        //return 2.9852*grey - 12.087;
        int tmp = 3.1916*grey - 10.193;
        if(tmp < 0){
            return 0;
        }else
            return tmp;
        return grey+mainWindow->getReferenceValue();
    }
    return grey;
    //PerfomanceTimer timer("greyToCandela");
    //return 0.0185*grey*grey - 0.753*grey + 47.205;
    // return 23.806 * qExp(0.0189*grey);
    // return grey;
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
