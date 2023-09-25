#ifndef MESSUREBOX_H
#define MESSUREBOX_H


#include "drawable.h"
class MessureBox : public Drawable
{
    QPoint end;
    QImage* image;
public:
    MessureBox():Drawable(){};
    MessureBox(QPoint origen, QPoint end, QImage* image):Drawable(origen), end(end), image(image){};
    virtual ~MessureBox(){};
    virtual void draw(QPainter* painter);
};

#endif // MESSUREBOX_H
