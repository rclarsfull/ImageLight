#ifndef GUIMANEGER_H
#define GUIMANEGER_H
#include <QImage>
#include "converter.h"
#include "canvas.h"

class GuiManeger
{
    Converter converter;
    QImage* image;
    QImage* greyScalePic;
    QVector<QImage*> greyScaledChannels;
    Canvas *orgPic, *newPic;
public:
    GuiManeger();
    ~GuiManeger();
    void init(Canvas* a_orgPic, Canvas* a_newPic);
    void converte(QString fileName);
};

#endif // GUIMANEGER_H
