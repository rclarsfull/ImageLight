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
    static QVector<QImage*> getAsGreyScale(QImage* originalImage);
    static QImage* combineChannels(QVector<QImage*> greyImageChannels);
    static unsigned int greyToCandela(unsigned int);
    static unsigned int getConversionPresition (unsigned int);
    static void greyImageToColorImage(QImage* greyImage, unsigned int minGrey, unsigned int maxGrey);
    static QColor greyToColor(unsigned int grey, unsigned int minGrey, unsigned int maxGrey);
    static unsigned int getMinGrey(QImage* greyImage){
        unsigned int min = 255;
        for(int y = 0; y < greyImage->height(); y++){
            for(int x = 0; x < greyImage->width(); x++){
                QColor color = greyImage->pixelColor(x,y);
                if(color.red() == 0)
                    return 0;
                if(color.red() < min)
                    min = color.red();
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
                if(color.red() == 255)
                    return 255;
                if(color.red() > max)
                    max = color.red();
            }
        }
        return max;
    }
        ;
};

#endif // CONVERTER_H
