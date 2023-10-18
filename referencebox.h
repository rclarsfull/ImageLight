#ifndef REFERENCEBOX_H
#define REFERENCEBOX_H
#include "drawable.h"

class ReferenceBox: public Drawable
{
    QPoint end;
    QImage** image;
    int avgGrey;
public:
    ReferenceBox():Drawable(){};
    ReferenceBox(QPoint origen, QPoint end, QImage** image, Converter* converter):Drawable(origen, converter), end(end), image(image){};
    virtual ~ReferenceBox(){};
    virtual void draw(QPainter* painter);
    int getAvgGrey();
};

#endif // REFERENCEBOX_H
