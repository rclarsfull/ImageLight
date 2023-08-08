#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QFrame>

class Canvas : public QFrame
{
    QImage image;
    QPoint* pressedLocation;
    Canvas* otherCanvas;
    QLabel* debugLabel;
public:
    Canvas();
    ~Canvas();
    void setImage(const QImage &newImage);
    void setOtherCanvas(Canvas* other);
    void setPressedLocation(QPoint* point);
    QPoint* getPressedLocation();
    void setDebugLabel(QLabel *newDebugLabel);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // CANVAS_H
