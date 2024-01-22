#include "converter.h"
#include "perfomancetimer.h"
#include "workerthread.h"
//#include "perfomancetimer.h"
#include "mainwindow.h"
#include <QDebug>
#include <QColor>
#include <math.h>
#include <QVector3D>
#include <QThread>
#include <fstream>


Converter::Converter(MainWindow *mainWindow):mainWindow(mainWindow), pythonServer("127.0.0.1",12345){
    lightCorrectionMatrix = new float[Global::X_RESELUTION][Global::Y_RESELUTION];
    std::ifstream in("correctionFile.dat", std::ios_base::binary);
    if(!in.read((char*)lightCorrectionMatrix,sizeof(float)*Global::X_RESELUTION*Global::Y_RESELUTION)){
        resetLightCorrectionMatrix();
        qDebug() << "cant open Correction file";
    }
}

Converter::~Converter(){
    delete[] lightCorrectionMatrix;
}

void Converter::calibrateLightCorrectionMatrix(unsigned short (*candela)[Global::Y_RESELUTION], QImage *image){
    if(candela != NULL){
        std::ofstream out("correctionFile.dat",std::ios_base::binary);
        unsigned long sum = 0;
        resetLightCorrectionMatrix();
        updateCandela(candela, image);
        for(int y = 0; y < Global::Y_RESELUTION; y++){
            for(int x = 0; x < Global::X_RESELUTION; x++){
                sum += candela[x][y];
            }
        }
        double avg = sum / (Global::Y_RESELUTION*Global::X_RESELUTION);
        double max = 0;
        for(int y = 0; y < Global::Y_RESELUTION; y++){
            for(int x = 0; x < Global::X_RESELUTION; x++){
                lightCorrectionMatrix[x][y] = avg / candela[x][y];
                if(max < lightCorrectionMatrix[x][y])
                    max = lightCorrectionMatrix[x][y];
            }
        }
        for(int y = 0; y < Global::Y_RESELUTION; y++){
            for(int x = 0; x < Global::X_RESELUTION; x++){
                lightCorrectionMatrix[x][y] = lightCorrectionMatrix[x][y] /max;
            }
        }
        out.write((char*)lightCorrectionMatrix,sizeof(float)*Global::X_RESELUTION*Global::Y_RESELUTION);
        out.close();
        qDebug() << "calibrated";
    }else
        qDebug() << "Error: no Image loaded";
}


void Converter::resetLightCorrectionMatrix(){
    for (int i = 0; i< Global::Y_RESELUTION; i++) {
        for (int j = 0; j < Global::X_RESELUTION; ++j) {
            lightCorrectionMatrix[j][i] = 1.0;
        }
    }
}

void Converter::updateCandela(unsigned short (*candela)[Global::Y_RESELUTION], QImage *image)
{

    PerfomanceTimer timer("UpdateCandela");
    float *data = new float[Global::X_RESELUTION*Global::Y_RESELUTION*3];
    for(int y = 0; y < Global::Y_RESELUTION; y++){
        for(int x = 0; x < Global::X_RESELUTION; x++){
            float *tmp = colorToRGBArray(image->pixelColor(x,y),x,y);
            data[y*Global::X_RESELUTION*3 + x*3] = tmp[0];
            data[y*Global::X_RESELUTION*3 + x*3 + 1] = tmp[1];
            data[y*Global::X_RESELUTION*3 + x*3 + 2] = tmp[2];
            //qDebug() << tmp[0];
            delete[] tmp;
        }
    }
    pythonServer.connectAndSendData(data, candela);
    delete[] data;
}

inline double Converter::sRGBToLinear(double value) {
    double linearValue;
    if (value <= 0.04045) {
        linearValue = value / 12.92;
    } else {
        linearValue = qPow((value + 0.055) / 1.055, 2.4);
    }
    //qDebug() << value << " => " << linearValue;
    return linearValue;
}

inline float* Converter::colorToRGBArray(QColor color, unsigned int x, unsigned int y) {
    float red = sRGBToLinear(color.redF()) * lightCorrectionMatrix[x][y];
    float green = sRGBToLinear(color.greenF()) * lightCorrectionMatrix[x][y];
    float blue = sRGBToLinear(color.blueF()) * lightCorrectionMatrix[x][y];
    if(red > 1)
        red = 1;
    if(green > 1)
        green = 1;
    if(blue > 1)
        blue = 1;
    if(red < 0)
        red = 0;
    if(green < 0)
        green = 0;
    if(blue < 0)
        blue = 0;
    float *rgbData = new float[3];
    rgbData[0] = red;
    rgbData[1] = green;
    rgbData[2] = blue;
    return rgbData;
}

void Converter::updateFalschfarbenBild(unsigned short (*candela)[Global::Y_RESELUTION],QImage* falschfarbenBild,int minGrey, int maxGrey)
{
    for(int y = 0; y < Global::Y_RESELUTION; y++){
        for(int x = 0; x < Global::X_RESELUTION; x++){
            falschfarbenBild->setPixelColor(x,y,Converter::candelaToColor(candela[x][y],minGrey,maxGrey));
        }
    }
}

inline QColor Converter::candelaToColor(unsigned short candela, unsigned int minCandela, unsigned int maxCandela)
{
    //PerfomanceTimer timer("greyToColor");
    if(candela > maxCandela)
        return QColor(255,255,255);
    if(candela < minCandela)
        return QColor(0,0,0);
    if(minCandela == maxCandela)
        return QColor(0, 100, 0);

    double rot = 0, gruen = 0, blau = 0;

    double a = (maxCandela-minCandela)/4;
    double b = Global::MAX_CANDELA/(a*a);

    if(candela < (minCandela + 3*a)){
        rot = -(b*(candela-3*a-minCandela)*(candela-3*a-minCandela))+255;
        gruen = -(b*(candela-2*a-minCandela)*(candela-2*a-minCandela))+255;
        blau = -(b*(candela-a-minCandela)*(candela-a-minCandela))+255;
    }else if(candela < (minCandela + 3*a)){
        rot = 255;
        gruen = 0;
        blau = -(b*(candela-4*a-minCandela)*(candela-4*a-minCandela))+255;
    }else{
        rot = 255;
        blau = -(b*(candela-4*a-minCandela)*(candela-4*a-minCandela))+255;
        gruen = -(b*(candela-4.5*a-minCandela)*(candela-4*a-minCandela))+255;
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

unsigned int Converter::getMinCandela(unsigned short (*candela)[Global::Y_RESELUTION]){
    unsigned int min = Global::MAX_CANDELA;
    for(int y = 0; y < Global::Y_RESELUTION; y++){
        for(int x = 0; x < Global::X_RESELUTION; x++){
            if(candela[x][y] == 0)
                return 0;
            if(candela[x][y] < min)
                min = candela[x][y];
        }
    }
    return min;
}

unsigned int Converter::getMaxCandela(unsigned short (*candela)[Global::Y_RESELUTION])
{
    unsigned int max = 0;
    for(int y = 0; y < Global::Y_RESELUTION; y++){
        for(int x = 0; x < Global::X_RESELUTION; x++){
            if(candela[x][y] == Global::MAX_CANDELA)
                return Global::MAX_CANDELA;
            if(candela[x][y] > max)
                max = candela[x][y];
        }
    }
    return max;
}


int Converter::scaleCordtoCanvas(int cord, int sizeCanvas, int sizeImage)
{
    double tmp = double(cord)/double(sizeCanvas);
    int value = tmp * sizeImage;
    return value;
}
//unsigned int Converter::colorToGrey(QColor color, unsigned int x, unsigned int y){
//    float red = (pow(color.red(),2.2));
//    float green = (pow(color.green(),2.2));
//    float blue = (pow(color.blue(),2.2));
//    float luminance =  (red * redModifer * 0.686*0.2126 + green * greenModifer *0.878* 0.7152 + blue * blueModifer *1.616* 0.0722)/ pow(255,2.2) * 255;
//    if(luminance<0)
//        luminance=0;
//    return qRound(luminance * lightCorrectionMatrix[x][y]);
//}


//unsigned int Converter::greyToCandela(unsigned int grey){
//    if(mainWindow->getMode() == withoutReference){
//        //return 2.9852*grey - 12.087;
//        int tmp = 3.1916*grey - 10.193;
//        if(tmp < 0){
//            return 0;
//        }else
//            return tmp;
//        return grey+mainWindow->getReferenceValue();
//    }
//    return grey;
//    //PerfomanceTimer timer("greyToCandela");
//    //return 0.0185*grey*grey - 0.753*grey + 47.205;
//    // return 23.806 * qExp(0.0189*grey);
//    // return grey;
//}




