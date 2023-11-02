#ifndef MESSUREBOX_H
#define MESSUREBOX_H


#include "drawable.h"
class MessureBox : public Drawable
{
    QPoint end;
    QImage** image;
    double avgCandala;
    int id;
    static int counter;
    void calcAvgCanela();
public:
    MessureBox():Drawable(){};
    MessureBox(QPoint origen, QPoint end, QImage** image, Converter* converter);
    ~MessureBox(){};
    void draw(QPainter* painter);
    int getAvgCandala(){return avgCandala;};
    int getId(){return id;};

};

#endif // MESSUREBOX_H
