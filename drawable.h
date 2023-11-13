#ifndef DRAWABLE_H
#define DRAWABLE_H


#include "converter.h"
#include <QPainter>
#include <QPoint>
class Drawable
{
protected:
    QPoint origen;
    Converter* converter;
public:
    Drawable(){};
    Drawable(QPoint origen, Converter* converter):origen(origen), converter(converter){};
    virtual ~Drawable(){};
    virtual void draw(QPainter* painter) = 0;
    QPoint getOrigen() const{return origen;};
};

#endif // DRAWABLE_H
