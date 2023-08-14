#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include <QImage>
#include <QRunnable>
#include <QThread>
#include "converter.h"
#include "qmutex.h"


class WorkerThread: public QRunnable
{
protected:
    QImage* image;
    QMutex* imageMutex;
public:
    WorkerThread(QImage* a_image, QMutex* a_imageMutex):image(a_image), imageMutex(a_imageMutex){};
    virtual ~WorkerThread(){};
    void run() = 0;
};

#endif // WORKERTHREAD_H

#ifndef WORKERTHREADGREYTOCOLOR_H
#define WORKERTHREADGREYTOCOLOR_H
#include <QImage>
#include <QRunnable>
#include <QThread>

class WorkerThreadGreyToColor: public WorkerThread
{
    unsigned int minGrey;
    unsigned int maxGrey;
    unsigned int row;
public:
    WorkerThreadGreyToColor(QImage* a_image, QMutex* a_imageMutex, unsigned int a_minGrey, unsigned int a_maxGrey, unsigned int a_row)
        :WorkerThread(a_image,a_imageMutex), minGrey(a_minGrey), maxGrey(a_maxGrey), row(a_row){};
    ~WorkerThreadGreyToColor(){};
    void run(){
        for(int x = 0; x < image->width(); x++){
                imageMutex->lock();
                unsigned int grey = image->pixelColor(x,row).red();
                image->setPixelColor(x,row,Converter::greyToColor(grey, minGrey, maxGrey));
                imageMutex->unlock();
        }
    };
};

#endif // WORKERTHREADGREYTOCOLOR_H
