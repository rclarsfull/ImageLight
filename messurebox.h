#ifndef MESSUREBOX_H
#define MESSUREBOX_H


#include "drawable.h"
class MessureBox : public Drawable
{
    QPoint end;
    QImage** image;
    int avgCandala;
    void calcAvgCanela();
public:
    MessureBox():Drawable(){};
    MessureBox(QPoint origen, QPoint end, QImage** image, Converter* converter);;
    ~MessureBox(){};
    void draw(QPainter* painter);
    int getAvgCandala(){return avgCandala;};

};

#endif // MESSUREBOX_H
