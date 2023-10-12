#ifndef CANVAS_H
#define CANVAS_H

#include "drawable.h"
#include <QLabel>
#include <QFrame>

class Canvas : public QFrame
{
    bool isOriginalImage;
    int minGrey, maxGrey;
    QImage* image;
    QImage* resizedImage;
    QPoint* pressedLocation;
    Canvas* otherCanvas;
    QLabel* debugLabel;
    QVector<Drawable*> drawabels;
    QImage canvas;
public:
    Canvas(bool isOriginalImage = false);
    ~Canvas();
    void setImage(QImage *newImage);
    void setOtherCanvas(Canvas* other);
    void setPressedLocation(QPoint* point);
    QPoint* getPressedLocation();
    void setDebugLabel(QLabel *newDebugLabel);
    void resize();

    QVector<Drawable *>& getDrawabels();

    bool getIsOriginalImage() const;


    QImage* getImage();

    int getMinGrey() const;
    void setMinGrey(int newMinGrey);

    int getMaxGrey() const;
    void setMaxGrey(int newMaxGrey);

    QImage* getCanvas();

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // CANVAS_H
