#ifndef MESSUREBOX_H
#define MESSUREBOX_H


#include "drawable.h"
class Canvas;
class MessureBox : public Drawable
{
    QPoint end;
    unsigned short (**candela)[Global::Y_RESELUTION];
    Canvas* canvas;
    QImage* originalImage;
    int avgCandala;
    int id;
    static int counter;
    void calcAvgCanela();
public:
    MessureBox():Drawable(){};
    MessureBox(QPoint origen, QPoint end, unsigned short (**candela)[Global::Y_RESELUTION], Canvas* canvas, QImage *originalImage);
    ~MessureBox(){};
    void draw(QPainter* painter);
    int getAvgCandala(){return avgCandala;};
    int getId(){return id;};

};

#endif // MESSUREBOX_H
