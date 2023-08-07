#ifndef CONVERTER_H
#define CONVERTER_H

#include <QImage>

class Converter
{
public:
    Converter();
    static QVector<QImage*> getAsGreyScale(QImage* originalImage);
    static QImage* combineChannels(QVector<QImage*> greyImageChannels);
};

#endif // CONVERTER_H
