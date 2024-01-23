#ifndef MESSUREBOX_H
#define MESSUREBOX_H


#include "drawable.h"
class Canvas;
class MessureBox : public Drawable
{
    QPoint end;
    unsigned short (**candela)[Global::X_RESELUTION];
    QImage** resizedImage;
    QImage** originalImage;
    MainWindow *mainWindow;
    int avgCandala;
    float avgRed, avgGreen, avgBlue;
    int id;
    static int counter;
    void calcAvgCanela();
public:
    MessureBox():Drawable(){};
    MessureBox(QPoint origen, QPoint end, unsigned short (**candela)[Global::X_RESELUTION], QImage **resizedImage, QImage **originalImage, Converter *converter, MainWindow * mainWindow);
    ~MessureBox(){};
    void draw(QPainter* painter);
    int getAvgCandala(){return avgCandala;};
    int getId(){return id;};

    float getAvgRed() const;
    float getAvgGreen() const;
    float getAvgBlue() const;
};

#endif // MESSUREBOX_H
