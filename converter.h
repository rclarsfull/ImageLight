#ifndef CONVERTER_H
#define CONVERTER_H

#include <QImage>
#include <QColor>
#include <QThreadPool>
#include <QMutex>

class Converter
{
    static QThreadPool workerPool;
    static QMutex imageMutex;
public:
    Converter() = delete;
    Converter(Converter&) = delete;
    static QImage* getAsGreyScale(QImage* originalImage);
    static unsigned int greyToCandela(unsigned int);
    static unsigned int getConversionPresition (unsigned int);
    static void recolorImage(QImage* image);
    static QColor greyToColor(unsigned int grey, unsigned int minGrey, unsigned int maxGrey);
    static unsigned int getMinGrey(QImage* greyImage){
        unsigned int min = 255;
        for(int y = 0; y < greyImage->height(); y++){
            for(int x = 0; x < greyImage->width(); x++){
                QColor color = greyImage->pixelColor(x,y);
                int durchschnitt = (color.red() + color.green() + color.blue())/3;
                if(durchschnitt == 0)
                    return 0;
                if(durchschnitt < min)
                    min = durchschnitt;
            }
        }
        return min;
    };
    static unsigned int getMaxGrey(QImage* greyImage)
    {
        unsigned int max = 0;
        for(int y = 0; y < greyImage->height(); y++){
            for(int x = 0; x < greyImage->width(); x++){
                QColor color = greyImage->pixelColor(x,y);
                int durchschnitt = (color.red() + color.green() + color.blue())/3;
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
