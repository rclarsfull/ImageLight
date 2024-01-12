#ifndef REFERENCEBOX_H
#define REFERENCEBOX_H
#include "drawable.h"

class ReferenceBox: public Drawable
{
    QPoint end;
    unsigned short *((*candela)[Global::Y_RESELUTION]);
    unsigned short avgGrey;
public:
    ReferenceBox():Drawable(){};
    ReferenceBox(QPoint origen, QPoint end, unsigned short *((*candela)[Global::Y_RESELUTION]), Converter* converter):Drawable(origen, converter), end(end), candela(candela){};
    ~ReferenceBox(){};
    void draw(QPainter* painter);
    unsigned short getAvgGrey();
};

#endif // REFERENCEBOX_H
