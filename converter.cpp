#include <QDebug>
#include <QColor>
#include <math.h>
#include <QVector3D>
#include <QThread>
#include "converter.h"
#include "workerthread.h"
#include "perfomancetimer.h"

//#define multithreading
#define THREAD_COUNT 30
#define SLEEP 5

QThreadPool Converter::workerPool = QThreadPool();
QMutex Converter::imageMutex = QMutex();
#ifdef multithreading


QImage* Converter::combineChannels(QVector<QImage*> greyImageChannels)
{
    PerfomanceTimer timer("combineChannels");
    QImage* out = new QImage(*greyImageChannels[0]);
    QList<WorkerThreadCombineChannels*> workers;
    workerPool.setMaxThreadCount(THREAD_COUNT);
    for(int y = 0; y < greyImageChannels[0]->height(); y+=50){
        workers.push_back(new WorkerThreadCombineChannels(&imageMutex,&greyImageChannels,out,y));
    }
    for (QList<WorkerThreadCombineChannels*>::Iterator it = workers.begin(); it != workers.end(); it++) {
        while (!workerPool.tryStart(*it)){
                QThread::usleep(SLEEP);
        }
    }
    workerPool.waitForDone();
    return out;
}



void Converter::greyImageToColorImage(QImage* greyImage, unsigned int minGrey, unsigned int maxGrey)
{
    PerfomanceTimer timer("greyImageToColorImage");
    QList<WorkerThreadGreyToColor*> workers;
    workerPool.setMaxThreadCount(THREAD_COUNT);
    for(int y = 0; y < greyImage->height(); y+=50){
        workers.push_back(new WorkerThreadGreyToColor(&imageMutex, greyImage, minGrey,maxGrey,y));
    }
    //PerfomanceTimer timer2("greyImageToColorImageOnlyRun");
    for (QList<WorkerThreadGreyToColor*>::Iterator it = workers.begin(); it != workers.end(); it++) {
        while (!workerPool.tryStart(*it)){
            QThread::usleep(SLEEP);
        }
    }
    workerPool.waitForDone();
//    for(WorkerThreadGreyToColor* worker:workers){
//        delete worker;
//    }
}



#else

QImage* Converter::combineChannels(QVector<QImage*> greyImageChannels)
{
    PerfomanceTimer timer("combineChannels");
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



void Converter::greyImageToColorImage(QImage* greyImage, unsigned int minGrey, unsigned int maxGrey)
{
    PerfomanceTimer timer("greyImageToColorImage");
    for(int y = 0; y < greyImage->height(); y++){
        for(int x = 0; x < greyImage->width(); x++){
                unsigned int grey = greyImage->pixelColor(x,y).red();
                greyImage->setPixelColor(x,y,Converter::greyToColor(grey,minGrey,maxGrey));
        }
    }
}


#endif

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

inline QColor Converter::greyToColor(unsigned int grey, unsigned int minGrey, unsigned int maxGrey)
{
    double rot = 0, gruen = 0, blau = 0;

    double a = (maxGrey-minGrey)/4;
    double b = (255 + minGrey)/(a*a);

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
