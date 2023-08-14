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
    static inline QColor greyToColor(unsigned int grey, unsigned int minGrey, unsigned int maxGrey);
    static unsigned int getMinGrey(QImage* greyImage);
    static unsigned int getMaxGrey(QImage* greyImage);
};

#endif // CONVERTER_H
