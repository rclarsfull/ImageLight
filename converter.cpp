#include "converter.h"
#include "mainwindow.h"
#include <QDebug>
#include <QColor>
#include <math.h>
#include <QVector3D>
#include <QThread>
#include <fstream>


Converter::Converter(MainWindow *mainWindow):mainWindow(mainWindow), pythonServer(mainWindow){
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

void Converter::calibrateLightCorrectionMatrix(unsigned short (*candela)[Global::X_RESELUTION], QImage *image){
    if(candela != NULL){
        std::ofstream out("correctionFile.dat",std::ios_base::binary);
        unsigned long sum = 0;
        resetLightCorrectionMatrix();
        updateCandela(candela, image);
        for(int y = 0; y < Global::Y_RESELUTION; y++){
            for(int x = 0; x < Global::X_RESELUTION; x++){
                sum += candela[y][x];
            }
        }
        double avg = sum / (Global::Y_RESELUTION*Global::X_RESELUTION);
        double max = 0;
        for(int y = 0; y < Global::Y_RESELUTION; y++){
            for(int x = 0; x < Global::X_RESELUTION; x++){
                lightCorrectionMatrix[x][y] = avg / candela[y][x];
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
        qDebug() << "Calibrated";
    }else
        qDebug() << "Error: No Image loaded";
}


void Converter::resetLightCorrectionMatrix(){
    for (int i = 0; i< Global::Y_RESELUTION; i++) {
        for (int j = 0; j < Global::X_RESELUTION; ++j) {
            lightCorrectionMatrix[j][i] = 1.0;
        }
    }
}

void Converter::updateCandela(unsigned short (*candela)[Global::X_RESELUTION], QImage *image)
{
    float *data = new float[Global::X_RESELUTION*Global::Y_RESELUTION*3];
    for(int y = 0; y < Global::Y_RESELUTION; y++){
        for(int x = 0; x < Global::X_RESELUTION; x++){
            float *tmp = colorToRGBArray(image->pixelColor(x,y),x,y);
            data[y*Global::X_RESELUTION*3 + x*3] = tmp[0];
            data[y*Global::X_RESELUTION*3 + x*3 + 1] = tmp[1];
            data[y*Global::X_RESELUTION*3 + x*3 + 2] = tmp[2];
            delete[] tmp;
        }
    }
    pythonServer.connectAndSendData(data, candela);
    if(mainWindow->getSettingsWindow()->getexposureTime() != 1){
        float factor = 1/mainWindow->getSettingsWindow()->getexposureTime();
        for(int y = 0; y < Global::Y_RESELUTION; y++){
            for(int x = 0; x < Global::X_RESELUTION; x++){
                candela[y][x] = candela[y][x] * factor;
            }
        }
    }
    delete[] data;
}

inline double Converter::sRGBToLinear(double value) {
    double linearValue;
    if (value <= 0.04045) {
        linearValue = value / 12.92;
    } else {
        linearValue = qPow((value + 0.055) / 1.055, 2.4);
    }
    return linearValue;
}

float* Converter::colorToRGBArray(QColor color, unsigned int x, unsigned int y) {

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

void Converter::updateFalschfarbenBild(unsigned short (*candela)[Global::X_RESELUTION],QImage* falschfarbenBild,int minGrey, int maxGrey)
{
    for(int y = 0; y < Global::Y_RESELUTION; y++){
        for(int x = 0; x < Global::X_RESELUTION; x++){
            falschfarbenBild->setPixelColor(x,y,Converter::candelaToColor(candela[y][x],minGrey,maxGrey));
        }
    }
}

QColor Converter::candelaToColor(unsigned short candela, unsigned int minCandela, unsigned int maxCandela)
{
    if(candela > maxCandela)
        return QColor(255,255,255);
    if(candela < minCandela)
        return QColor(0,0,0);
    if(minCandela == maxCandela)
        return QColor(0, 100, 0);

    double rot = 0, gruen = 0, blau = 0;
    double a = (maxCandela-minCandela)/4;
    double b = 255/(a*a);

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
    return QColor(rot , gruen, blau);
}

unsigned int Converter::getMinCandela(unsigned short (*candela)[Global::X_RESELUTION]){
    unsigned int min = mainWindow->getSettingsWindow()->getMaxCandela();
    for(int y = 0; y < Global::Y_RESELUTION; y++){
        for(int x = 0; x < Global::X_RESELUTION; x++){
            if(candela[y][x] == 0)
                return 0;
            if(candela[y][x] < min)
                min = candela[y][x];
        }
    }
    return min;
}

unsigned int Converter::getMaxCandela(unsigned short (*candela)[Global::X_RESELUTION])
{
    unsigned int max = 0;
    int maxCandela = mainWindow->getSettingsWindow()->getMaxCandela();
    for(int y = 0; y < Global::Y_RESELUTION; y++){
        for(int x = 0; x < Global::X_RESELUTION; x++){
            if(candela[y][x] == maxCandela)
                return maxCandela;
            if(candela[y][x] > max)
                max = candela[y][x];
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



