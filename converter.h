#ifndef CONVERTER_H
#define CONVERTER_H

#include "perfomancetimer.h"
#include <QImage>
#include <QColor>
//#include <QThreadPool>
//#include <QMutex>

class MainWindow;
class Converter
{
    //static QThreadPool workerPool;
   // static QMutex imageMutex;
    MainWindow* mainWindow ;
public:
    Converter(MainWindow* mainWindow):mainWindow(mainWindow){};
    Converter(Converter&);

    unsigned int greyToCandela(unsigned int);
    unsigned int getConversionPresition (unsigned int);
    void recolorImage(QImage* image, int minGrey, int maxGrey);
    inline QColor greyToColor(unsigned int grey, unsigned int minGrey, unsigned int maxGrey);
    unsigned int colorToGrey(QColor color);
    unsigned int getMinGrey(QImage* greyImage){
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
    };
    unsigned int getMaxGrey(QImage* greyImage)
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
        ;
};

#endif // CONVERTER_H
