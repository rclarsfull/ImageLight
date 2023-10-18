//#ifndef WORKERTHREAD_H
//#define WORKERTHREAD_H
//#include <QImage>
//#include <QRunnable>
//#include <QThread>
//#include "converter.h"
//#include "qmutex.h"


//class WorkerThread: public QRunnable
//{
//protected:
//    QMutex* imageMutex;
//public:
//    WorkerThread(QMutex* a_imageMutex): imageMutex(a_imageMutex){};
//    virtual ~WorkerThread(){};
//    void run() = 0;
//};

//#endif // WORKERTHREAD_H

//#ifndef WORKERTHREADGREYTOCOLOR_H
//#define WORKERTHREADGREYTOCOLOR_H
//#include <QImage>
//#include <QRunnable>
//#include <QThread>

//class WorkerThreadGreyToColor: public WorkerThread
//{
//    QImage* image;
//    unsigned int minGrey;
//    unsigned int maxGrey;
//    unsigned int row;
//public:
//    WorkerThreadGreyToColor(QMutex* a_imageMutex, QImage* a_image, unsigned int a_minGrey, unsigned int a_maxGrey, unsigned int a_row)
//        :WorkerThread(a_imageMutex),image(a_image), minGrey(a_minGrey), maxGrey(a_maxGrey), row(a_row){};
//    ~WorkerThreadGreyToColor(){};
//    void run(){
//        for(int y = row; y < row+50 && y < image->height(); y++){
//            for(int x = 0; x < image->width(); x++){
//                unsigned int grey = image->pixelColor(x,y).red();
//                imageMutex->lock();
//                image->setPixelColor(x,y,Converter::greyToColor(grey, minGrey, maxGrey));
//                imageMutex->unlock();
//            }
//        }
//    };
//};

//#endif // WORKERTHREADGREYTOCOLOR_H

//#ifndef WORKERTHREADCOMBINECHANNELS_H
//#define WORKERTHREADCOMBINECHANNELS_H
//#include <QImage>
//#include <QRunnable>
//#include <QThread>

//class WorkerThreadCombineChannels: public WorkerThread
//{
//    QVector<QImage*>* greyImageChannels;
//    QImage* out;
//    unsigned int row;
//public:
//    WorkerThreadCombineChannels(QMutex* a_imageMutex, QVector<QImage*>* a_greyImageChannels, QImage* a_out, unsigned int a_row)
//        :WorkerThread(a_imageMutex), greyImageChannels(a_greyImageChannels), out(a_out), row(a_row){};
//    ~WorkerThreadCombineChannels(){};
//    void run(){
//        for(int y = row; y < row+50 && y < (*greyImageChannels)[0]->height(); y++){
//            for(int x = 0; x < (*greyImageChannels)[0]->width(); x++){
//                int redGrey = (*greyImageChannels)[0]->pixelColor(x,y).red();
//                int greenGrey = (*greyImageChannels)[1]->pixelColor(x,y).green();
//                int blueGrey = (*greyImageChannels)[2]->pixelColor(x,y).blue();
//                int durchschnitt = (redGrey + greenGrey + blueGrey)/3;
//                imageMutex->lock();
//                out->setPixelColor(x,y,QColor(durchschnitt,durchschnitt,durchschnitt));
//                imageMutex->unlock();
//            }
//        }
//    };
//};

//#endif // WORKERTHREADCOMBINECHANNELS_H
