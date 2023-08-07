#ifndef CONVERTER_H
#define CONVERTER_H

#include <QImage>

class Converter
{
public:
    Converter();
    static QImage** getAsGreyScale(QImage* originalImage);
    static QImage* combineChannels(QImage** greyImageChannels);
};

#endif // CONVERTER_H
