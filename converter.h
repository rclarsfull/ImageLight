#ifndef CONVERTER_H
#define CONVERTER_H

#include <QImage>
#include <QColor>

class Converter
{
public:
    Converter() = delete;
    Converter(Converter&) = delete;
    static QVector<QImage*> getAsGreyScale(QImage* originalImage);
    static QImage* combineChannels(QVector<QImage*> greyImageChannels);
    static unsigned int greyToCandela(unsigned int);
    static unsigned int getConversionPresition (unsigned int);
    static void greyImageToColorImage(QImage* greyImage, double modifyer);
    static inline QColor greyToColor(unsigned int grey, double modifyer);
};

#endif // CONVERTER_H
